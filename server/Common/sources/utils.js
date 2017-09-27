/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

'use strict';

var config = require('config');
var fs = require('fs');
var path = require('path');
var url = require('url');
var request = require('request');
var co = require('co');
var URI = require("uri-js");
const escapeStringRegexp = require('escape-string-regexp');
const ipaddr = require('ipaddr.js');
var configDnsCache = config.get('dnscache');
const dnscache = require('dnscache')({
                                     "enable": configDnsCache.get('enable'),
                                     "ttl": configDnsCache.get('ttl'),
                                     "cachesize": configDnsCache.get('cachesize'),
                                   });
const jwt = require('jsonwebtoken');
const NodeCache = require( "node-cache" );
const ms = require('ms');
const constants = require('./constants');
const logger = require('./logger');
const forwarded = require('forwarded');

var configIpFilter = config.get('services.CoAuthoring.ipfilter');
var cfgIpFilterRules = configIpFilter.get('rules');
var cfgIpFilterErrorCode = configIpFilter.get('errorcode');
const cfgIpFilterEseForRequest = configIpFilter.get('useforrequest');
var cfgExpPemStdTtl = config.get('services.CoAuthoring.expire.pemStdTTL');
var cfgExpPemCheckPeriod = config.get('services.CoAuthoring.expire.pemCheckPeriod');
var cfgTokenOutboxHeader = config.get('services.CoAuthoring.token.outbox.header');
var cfgTokenOutboxPrefix = config.get('services.CoAuthoring.token.outbox.prefix');
var cfgTokenOutboxAlgorithm = config.get('services.CoAuthoring.token.outbox.algorithm');
var cfgTokenOutboxExpires = config.get('services.CoAuthoring.token.outbox.expires');
var cfgSignatureSecretInbox = config.get('services.CoAuthoring.secret.inbox');
var cfgSignatureSecretOutbox = config.get('services.CoAuthoring.secret.outbox');
var cfgVisibilityTimeout = config.get('queue.visibilityTimeout');
var cfgQueueRetentionPeriod = config.get('queue.retentionPeriod');

var ANDROID_SAFE_FILENAME = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._-+,@£$€!½§~\'=()[]{}0123456789';

var g_oIpFilterRules = function() {
  var res = [];
  for (var i = 0; i < cfgIpFilterRules.length; ++i) {
    var rule = cfgIpFilterRules[i];
    var regExpStr = rule['address'].split('*').map(escapeStringRegexp).join('.*');
    var exp = new RegExp('^' + regExpStr + '$', 'i');
    res.push({allow: rule['allowed'], exp: exp});
  }
  return res;
}();
var isEmptySecretTenants = isEmptyObject(cfgSignatureSecretInbox.tenants);
const pemfileCache = new NodeCache({stdTTL: ms(cfgExpPemStdTtl) / 1000, checkperiod: ms(cfgExpPemCheckPeriod) / 1000, errorOnMissing: false, useClones: true});

exports.CONVERTION_TIMEOUT = 1.5 * (cfgVisibilityTimeout + cfgQueueRetentionPeriod) * 1000;

exports.addSeconds = function(date, sec) {
  date.setSeconds(date.getSeconds() + sec);
};
exports.getMillisecondsOfHour = function(date) {
  return (date.getUTCMinutes() * 60 +  date.getUTCSeconds()) * 1000 + date.getUTCMilliseconds();
};
exports.encodeXml = function(value) {
	return value.replace(/[<>&'"\r\n\t\xA0]/g, function (c) {
		switch (c) {
			case '<': return '&lt;';
			case '>': return '&gt;';
			case '&': return '&amp;';
			case '\'': return '&apos;';
			case '"': return '&quot;';
			case '\r': return '&#xD;';
			case '\n': return '&#xA;';
			case '\t': return '&#x9;';
			case '\xA0': return '&#A0;';
		}
	});
};
function fsStat(fsPath) {
  return new Promise(function(resolve, reject) {
    fs.stat(fsPath, function(err, stats) {
      if (err) {
        reject(err);
      } else {
        resolve(stats);
      }
    });
  });
}
exports.fsStat = fsStat;
function fsReadDir(fsPath) {
  return new Promise(function(resolve, reject) {
    fs.readdir(fsPath, function(err, list) {
      if (err) {
        return reject(err);
      } else {
        resolve(list);
      }
    });
  });
}
function* walkDir(fsPath, results, optNoSubDir, optOnlyFolders) {
  const list = yield fsReadDir(fsPath);
  for (let i = 0; i < list.length; ++i) {
    const file = path.join(fsPath, list[i]);
    const stats = yield fsStat(file);
    if (stats.isDirectory()) {
      if (optNoSubDir) {
        optOnlyFolders && results.push(file);
      } else {
        yield* walkDir(file, results, optNoSubDir, optOnlyFolders);
      }
    } else {
      !optOnlyFolders && results.push(file);
    }
  }
}
exports.listFolders = function(fsPath, optNoSubDir) {
  return co(function* () {
    let stats, list = [];
    try {
      stats = yield fsStat(fsPath);
    } catch (e) {
      //exception if fsPath not exist
      stats = null;
    }
    if (stats && stats.isDirectory()) {
        yield* walkDir(fsPath, list, optNoSubDir, true);
    }
    return list;
  });
};
exports.listObjects = function(fsPath, optNoSubDir) {
  return co(function* () {
    let stats, list = [];
    try {
      stats = yield fsStat(fsPath);
    } catch (e) {
      //exception if fsPath not exist
      stats = null;
    }
    if (stats) {
      if (stats.isDirectory()) {
        yield* walkDir(fsPath, list, optNoSubDir, false);
      } else {
        list.push(fsPath);
      }
    }
    return list;
  });
};
exports.sleep = function(ms) {
  return new Promise(function(resolve) {
    setTimeout(resolve, ms);
  });
};
exports.readFile = function(file) {
  return new Promise(function(resolve, reject) {
    fs.readFile(file, function(err, data) {
      if (err) {
        reject(err);
      } else {
        resolve(data);
      }
    });
  });
};
function makeAndroidSafeFileName(str) {
  for (var i = 0; i < str.length; i++) {
    if (-1 == ANDROID_SAFE_FILENAME.indexOf(str[i])) {
      str[i] = '_';
    }
  }
  return str;
}
function encodeRFC5987ValueChars(str) {
  return encodeURIComponent(str).
    // Note that although RFC3986 reserves "!", RFC5987 does not,
    // so we do not need to escape it
    replace(/['()]/g, escape). // i.e., %27 %28 %29
    replace(/\*/g, '%2A').
    // The following are not required for percent-encoding per RFC5987,
    //  so we can allow for a little better readability over the wire: |`^
    replace(/%(?:7C|60|5E)/g, unescape);
}
function getContentDisposition (opt_filename, opt_useragent, opt_type) {
  //from http://stackoverflow.com/questions/93551/how-to-encode-the-filename-parameter-of-content-disposition-header-in-http
  var contentDisposition = opt_type ? opt_type : constants.CONTENT_DISPOSITION_ATTACHMENT;
  if (opt_filename) {
    contentDisposition += '; filename="';
    if (opt_useragent != null && -1 != opt_useragent.toLowerCase().indexOf('android')) {
      contentDisposition += makeAndroidSafeFileName(opt_filename) + '"';
    } else {
      contentDisposition += opt_filename + '"; filename*=UTF-8\'\'' + encodeRFC5987ValueChars(opt_filename);
    }
  }
  return contentDisposition;
}
function getContentDispositionS3 (opt_filename, opt_useragent, opt_type) {
  var contentDisposition = opt_type ? opt_type : constants.CONTENT_DISPOSITION_ATTACHMENT;
  if (opt_filename) {
    contentDisposition += ';';
    if (opt_useragent != null && -1 != opt_useragent.toLowerCase().indexOf('android')) {
      contentDisposition += ' filename=' + makeAndroidSafeFileName(opt_filename);
    } else {
      if (containsAllAsciiNP(opt_filename)) {
        contentDisposition += ' filename=' + opt_filename;
      } else {
        contentDisposition += ' filename*=UTF-8\'\'' + encodeRFC5987ValueChars(opt_filename);
      }
    }
  }
  return contentDisposition;
}
exports.getContentDisposition = getContentDisposition;
exports.getContentDispositionS3 = getContentDispositionS3;
function downloadUrlPromise(uri, optTimeout, optLimit, opt_Authorization) {
  return new Promise(function (resolve, reject) {
    //IRI to URI
    uri = URI.serialize(URI.parse(uri));
    var urlParsed = url.parse(uri);
    //if you expect binary data, you should set encoding: null
    var options = {uri: urlParsed, encoding: null, timeout: optTimeout};
    if (opt_Authorization) {
      options.headers = {};
      options.headers[cfgTokenOutboxHeader] = cfgTokenOutboxPrefix + opt_Authorization;
    }
    //TODO: Check how to correct handle a ssl link
    urlParsed.rejectUnauthorized = false;
    options.rejectUnauthorized = false;

    request.get(options, function (err, response, body) {
      if (err) {
        reject(err);
      } else {
        var correctSize = (!optLimit || body.length < optLimit);
        if (response.statusCode == 200 && correctSize) {
          resolve(body);
        } else {
          if (!correctSize) {
            var e = new Error('Error response: statusCode:' + response.statusCode + ' ;body.length:' + body.length);
            e.code = 'EMSGSIZE';
            reject(e);
          } else {
            reject(new Error('Error response: statusCode:' + response.statusCode + ' ;body:\r\n' + body));
          }
        }
      }
    })
  });
}
function postRequestPromise(uri, postData, optTimeout, opt_Authorization) {
  return new Promise(function(resolve, reject) {
    //IRI to URI
    uri = URI.serialize(URI.parse(uri));
    var urlParsed = url.parse(uri);
    var headers = {'Content-Type': 'application/json'};
    if (opt_Authorization) {
      headers[cfgTokenOutboxHeader] = cfgTokenOutboxPrefix + opt_Authorization;
    }
    var options = {uri: urlParsed, body: postData, encoding: 'utf8', headers: headers, timeout: optTimeout};

    //TODO: Check how to correct handle a ssl link
    urlParsed.rejectUnauthorized = false;
    options.rejectUnauthorized = false;

    request.post(options, function(err, response, body) {
      if (err) {
        reject(err);
      } else {
        if (200 == response.statusCode || 204 == response.statusCode) {
          resolve(body);
        } else {
          reject(new Error('Error response: statusCode:' + response.statusCode + ' ;body:\r\n' + body));
        }
      }
    })
  });
}
exports.postRequestPromise = postRequestPromise;
exports.downloadUrlPromise = downloadUrlPromise;
exports.mapAscServerErrorToOldError = function(error) {
  var res = -1;
  switch (error) {
    case constants.NO_ERROR :
      res = 0;
      break;
    case constants.TASK_QUEUE :
    case constants.TASK_RESULT :
      res = -6;
      break;
    case constants.CONVERT_DOWNLOAD :
      res = -4;
      break;
    case constants.CONVERT_TIMEOUT :
      res = -2;
      break;
    case constants.CONVERT_PASSWORD :
    case constants.CONVERT_DRM :
    case constants.CONVERT_NEED_PARAMS :
    case constants.CONVERT_PARAMS :
    case constants.CONVERT_LIBREOFFICE :
    case constants.CONVERT_CORRUPTED :
    case constants.CONVERT_UNKNOWN_FORMAT :
    case constants.CONVERT_READ_FILE :
    case constants.CONVERT :
      res = -3;
      break;
    case constants.UPLOAD_CONTENT_LENGTH :
      res = -9;
      break;
    case constants.UPLOAD_EXTENSION :
      res = -10;
      break;
    case constants.UPLOAD_COUNT_FILES :
      res = -11;
      break;
    case constants.VKEY :
      res = -8;
      break;
    case constants.VKEY_ENCRYPT :
      res = -20;
      break;
    case constants.VKEY_KEY_EXPIRE :
      res = -21;
      break;
    case constants.VKEY_USER_COUNT_EXCEED :
      res = -22;
      break;
    case constants.STORAGE :
    case constants.STORAGE_FILE_NO_FOUND :
    case constants.STORAGE_READ :
    case constants.STORAGE_WRITE :
    case constants.STORAGE_REMOVE_DIR :
    case constants.STORAGE_CREATE_DIR :
    case constants.STORAGE_GET_INFO :
    case constants.UPLOAD :
    case constants.READ_REQUEST_STREAM :
    case constants.UNKNOWN :
      res = -1;
      break;
  }
  return res;
};
function fillXmlResponse(val) {
  var xml = '<?xml version="1.0" encoding="utf-8"?><FileResult>';
  if (undefined != val.error) {
    xml += '<Error>' + exports.encodeXml(val.error.toString()) + '</Error>';
  } else {
    if (val.fileUrl) {
      xml += '<FileUrl>' + exports.encodeXml(val.fileUrl) + '</FileUrl>';
    } else {
      xml += '<FileUrl/>';
    }
    xml += '<Percent>' + val.percent + '</Percent>';
    xml += '<EndConvert>' + (val.endConvert ? 'True' : 'False') + '</EndConvert>';
  }
  xml += '</FileResult>';
  return xml;
}
function fillResponse(req, res, uri, error) {
  var data;
  var contentType;
  var output;
  if (constants.NO_ERROR != error) {
    output = {error: exports.mapAscServerErrorToOldError(error)};
  } else {
    output = {fileUrl: uri, percent: (uri ? 100 : 0), endConvert: !!uri};
  }
  var accept = req.get('Accept');
  if (accept && -1 != accept.toLowerCase().indexOf('application/json')) {
    data = JSON.stringify(output);
    contentType = 'application/json';
  } else {
    data = fillXmlResponse(output);
    contentType = 'text/xml';
  }
  var body = new Buffer(data, 'utf-8');
  res.setHeader('Content-Type', contentType + '; charset=UTF-8');
  res.setHeader('Content-Length', body.length);
  res.send(body);
}
exports.fillResponse = fillResponse;
function promiseCreateWriteStream(strPath, optOptions) {
  return new Promise(function(resolve, reject) {
    var file = fs.createWriteStream(strPath, optOptions);
    var errorCallback = function(e) {
      reject(e);
    };
    file.on('error', errorCallback);
    file.on('open', function() {
      file.removeListener('error', errorCallback);
      resolve(file);
    });
  });
};
exports.promiseCreateWriteStream = promiseCreateWriteStream;
function promiseCreateReadStream(strPath) {
  return new Promise(function(resolve, reject) {
    var file = fs.createReadStream(strPath);
    var errorCallback = function(e) {
      reject(e);
    };
    file.on('error', errorCallback);
    file.on('open', function() {
      file.removeListener('error', errorCallback);
      resolve(file);
    });
  });
};
exports.promiseCreateReadStream = promiseCreateReadStream;
exports.compareStringByLength = function(x, y) {
  if (x && y) {
    if (x.length == y.length) {
      return x.localeCompare(y);
    } else {
      return x.length - y.length;
    }
  } else {
    if (null != x) {
      return 1;
    } else if (null != y) {
      return -1;
    }
  }
  return 0;
};
function makeCRCTable() {
  var c;
  var crcTable = [];
  for (var n = 0; n < 256; n++) {
    c = n;
    for (var k = 0; k < 8; k++) {
      c = ((c & 1) ? (0xEDB88320 ^ (c >>> 1)) : (c >>> 1));
    }
    crcTable[n] = c;
  }
  return crcTable;
}
var crcTable;
exports.crc32 = function(str) {
  var crcTable = crcTable || (crcTable = makeCRCTable());
  var crc = 0 ^ (-1);

  for (var i = 0; i < str.length; i++) {
    crc = (crc >>> 8) ^ crcTable[(crc ^ str.charCodeAt(i)) & 0xFF];
  }

  return (crc ^ (-1)) >>> 0;
};
exports.promiseRedis = function(client, func) {
  var newArguments = Array.prototype.slice.call(arguments, 2);
  return new Promise(function(resolve, reject) {
    newArguments.push(function(err, data) {
      if (err) {
        reject(err);
      } else {
        resolve(data);
      }
    });
    func.apply(client, newArguments);
  });
};
exports.containsAllAscii = function(str) {
  return /^[\000-\177]*$/.test(str);
};
function containsAllAsciiNP(str) {
  return /^[\040-\176]*$/.test(str);//non-printing characters
}
exports.containsAllAsciiNP = containsAllAsciiNP;
function getBaseUrl(protocol, hostHeader, forwardedProtoHeader, forwardedHostHeader) {
  var url = '';
  if (forwardedProtoHeader) {
    url += forwardedProtoHeader;
  } else if (protocol) {
    url += protocol;
  } else {
    url += 'http';
  }
  url += '://';
  if (forwardedHostHeader) {
    url += forwardedHostHeader;
  } else if (hostHeader) {
    url += hostHeader;
  } else {
    url += 'localhost';
  }
  return url;
}
function getBaseUrlByConnection(conn) {
  return getBaseUrl('', conn.headers['host'], conn.headers['x-forwarded-proto'], conn.headers['x-forwarded-host']);
}
function getBaseUrlByRequest(req) {
  return getBaseUrl(req.protocol, req.get('host'), req.get('x-forwarded-proto'), req.get('x-forwarded-host'));
}
exports.getBaseUrlByConnection = getBaseUrlByConnection;
exports.getBaseUrlByRequest = getBaseUrlByRequest;
function stream2Buffer(stream) {
  return new Promise(function(resolve, reject) {
    if (!stream.readable) {
      resolve(new Buffer());
    }
    var bufs = [];
    stream.on('data', function(data) {
      bufs.push(data);
    });
    function onEnd(err) {
      if (err) {
        reject(err);
      } else {
        resolve(Buffer.concat(bufs));
      }
    }
    stream.on('end', onEnd);
    stream.on('error', onEnd);
  });
}
exports.stream2Buffer = stream2Buffer;
function changeOnlyOfficeUrl(inputUrl, strPath, optFilename) {
  //onlyoffice file server expects url end with file extension
  if (-1 == inputUrl.indexOf('?')) {
    inputUrl += '?';
  } else {
    inputUrl += '&';
  }
  return inputUrl + constants.ONLY_OFFICE_URL_PARAM + '=' + constants.OUTPUT_NAME + path.extname(optFilename || strPath);
}
exports.changeOnlyOfficeUrl = changeOnlyOfficeUrl;
function pipeStreams(from, to, isEnd) {
  return new Promise(function(resolve, reject) {
    from.pipe(to, {end: isEnd});
    from.on('end', function() {
      resolve();
    });
    from.on('error', function(e) {
      reject(e);
    });
  });
}
exports.pipeStreams = pipeStreams;
function* pipeFiles(from, to) {
  var fromStream = yield promiseCreateReadStream(from);
  var toStream = yield promiseCreateWriteStream(to);
  yield pipeStreams(fromStream, toStream, true);
}
exports.pipeFiles = co.wrap(pipeFiles);
function checkIpFilter(ipString, opt_hostname) {
  var status = 0;
  var ip4;
  var ip6;
  if (ipaddr.isValid(ipString)) {
    var ip = ipaddr.parse(ipString);
    if ('ipv6' == ip.kind()) {
      if (ip.isIPv4MappedAddress()) {
        ip4 = ip.toIPv4Address().toString();
      }
      ip6 = ip.toNormalizedString();
    } else {
      ip4 = ip.toString();
      ip6 = ip.toIPv4MappedAddress().toNormalizedString();
    }
  }
  for (var i = 0; i < g_oIpFilterRules.length; ++i) {
    var rule = g_oIpFilterRules[i];
    if ((opt_hostname && rule.exp.test(opt_hostname)) || (ip4 && rule.exp.test(ip4)) || (ip6 && rule.exp.test(ip6))) {
      if (!rule.allow) {
        status = cfgIpFilterErrorCode;
      }
      break;
    }
  }
  return status;
}
exports.checkIpFilter = checkIpFilter;
function* checkHostFilter(hostname) {
  let status = 0;
  let hostIp;
  try {
    hostIp = yield dnsLookup(hostname);
  } catch (e) {
    status = cfgIpFilterErrorCode;
    logger.error('dnsLookup error: hostname = %s\r\n%s', hostname, e.stack);
  }
  if (0 === status) {
    status = checkIpFilter(hostIp, hostname);
  }
  return status;
}
exports.checkHostFilter = checkHostFilter;
function checkClientIp(req, res, next) {
	let status = 0;
	if (cfgIpFilterEseForRequest) {
		const addresses = forwarded(req);
		const ipString = addresses[addresses.length - 1];
		status = checkIpFilter(ipString);
	}
	if (status > 0) {
		res.sendStatus(status);
	} else {
		next();
	}
}
exports.checkClientIp = checkClientIp;
function dnsLookup(hostname, options) {
  return new Promise(function(resolve, reject) {
    dnscache.lookup(hostname, options, function(err, addresses){
      if (err) {
        reject(err);
      } else {
        resolve(addresses);
      }
    });
  });
}
exports.dnsLookup = dnsLookup;
function isEmptyObject(val) {
  return !(val && Object.keys(val).length);
}
exports.isEmptyObject = isEmptyObject;
function getSecretByElem(secretElem) {
  let secret;
  if (secretElem) {
    if (secretElem.string) {
      secret = secretElem.string;
    } else if (secretElem.file) {
      secret = pemfileCache.get(secretElem.file);
      if (!secret) {
        secret = fs.readFileSync(secretElem.file);
        pemfileCache.set(secretElem.file, secret);
      }
    }
  }
  return secret;
}
exports.getSecretByElem = getSecretByElem;
function getSecret(docId, opt_iss, opt_token) {
  var secretElem = cfgSignatureSecretInbox;
  if (!isEmptySecretTenants) {
    var iss;
    if (opt_token) {
      //look for issuer
      var decodedTemp = jwt.decode(opt_token);
      if (decodedTemp && decodedTemp.iss) {
        iss = decodedTemp.iss;
      }
    } else {
      iss = opt_iss;
    }
    if (iss) {
      secretElem = cfgSignatureSecretInbox.tenants[iss];
      if (!secretElem) {
        logger.error('getSecret unknown issuer: docId = %s iss = %s', docId, iss);
      }
    }
  }
  return getSecretByElem(secretElem);
}
exports.getSecret = getSecret;
function fillJwtForRequest(opt_payload) {
  let data = {};
  if(opt_payload){
    data.payload = opt_payload;
  }

  let options = {algorithm: cfgTokenOutboxAlgorithm, expiresIn: cfgTokenOutboxExpires};
  let secret = getSecretByElem(cfgSignatureSecretOutbox);
  return jwt.sign(data, secret, options);
}
exports.fillJwtForRequest = fillJwtForRequest;
exports.forwarded = forwarded;
