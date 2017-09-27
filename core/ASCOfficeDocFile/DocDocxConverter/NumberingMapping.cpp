﻿/*
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

#include "NumberingMapping.h"

namespace DocFileFormat
{
	NumberingMapping::NumberingMapping(ConversionContext* context) : AbstractOpenXmlMapping(new XMLTools::CStringXmlWriter()), m_context(context), m_document(NULL), m_xmldocument(NULL)
	{
		if (m_context)
		{
			m_document		=	m_context->GetDocument();
			m_xmldocument	=	m_context->GetXmlDocument();
		}
	}

	NumberingMapping::~NumberingMapping()
	{
		RELEASEOBJECT(m_pXmlWriter);
	}
}

namespace DocFileFormat
{
	void NumberingMapping::Apply(IVisitable* visited)
	{
		if ((NULL == m_document) || (NULL == m_xmldocument))
			return;

		ListTable* rglst = static_cast<ListTable*>(visited);

		if ((rglst != NULL) && (!rglst->listData.empty() || !rglst->listNumbering.empty()))
		{
			m_xmldocument->RegisterNumbering();

			//start the document
            m_pXmlWriter->WriteNodeBegin( L"?xml version=\"1.0\" encoding=\"UTF-8\"?" );
            m_pXmlWriter->WriteNodeBegin( L"w:numbering", TRUE );

			//write namespaces
            m_pXmlWriter->WriteAttribute( L"xmlns:w", OpenXmlNamespaces::WordprocessingML );
            m_pXmlWriter->WriteAttribute( L"xmlns:v", OpenXmlNamespaces::VectorML );
            m_pXmlWriter->WriteAttribute( L"xmlns:o", OpenXmlNamespaces::Office );
            m_pXmlWriter->WriteAttribute( L"xmlns:w10", OpenXmlNamespaces::OfficeWord );
            m_pXmlWriter->WriteAttribute( L"xmlns:r", OpenXmlNamespaces::Relationships );

            m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

			PictureBulletsMapping();

			int i = 0;

			for (std::list<ListData*>::iterator iter = rglst->listData.begin(); iter != rglst->listData.end(); ++iter, ++i)
			{
				//start abstractNum
                m_pXmlWriter->WriteNodeBegin( L"w:abstractNum", TRUE );
                m_pXmlWriter->WriteAttribute( L"w:abstractNumId", FormatUtils::IntToWideString( i ));
                m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

				//nsid
                m_pXmlWriter->WriteNodeBegin( L"w:nsid", TRUE );
                m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToFormattedWideString( (*iter)->lsid, L"%08x" ));
                m_pXmlWriter->WriteNodeEnd( L"", TRUE );

				//multiLevelType
                m_pXmlWriter->WriteNodeBegin( L"w:multiLevelType", TRUE );

				if ( (*iter)->fHybrid )
				{
                    m_pXmlWriter->WriteAttribute( L"w:val", L"hybridMultilevel" );
				}
				else if ( (*iter)->fSimpleList )
				{
                    m_pXmlWriter->WriteAttribute( L"w:val", L"singleLevel" );
				}
				else
				{
                    m_pXmlWriter->WriteAttribute( L"w:val", L"multilevel" );
				}

                m_pXmlWriter->WriteNodeEnd( L"", TRUE );

				//template
                m_pXmlWriter->WriteNodeBegin( L"w:tmpl", TRUE );
                m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToFormattedWideString( (*iter)->tplc, L"%08x"));
                m_pXmlWriter->WriteNodeEnd( L"", TRUE );

				// writes the levels
				size_t length = (*iter)->rglvl->size();

				for (size_t j = 0; j < length; ++j)
				{
					ListLevel* lvl = (*iter)->rglvl->at(j);
					LevelMapping(lvl, j, (*iter)->rgistd[j]);
				}

				//end abstractNum
                m_pXmlWriter->WriteNodeEnd( L"w:abstractNum" );
			}
			//write old style numbering (сложносоставных не сущестует)
			for (std::list<NumberingDescriptor>::iterator iter = rglst->listNumbering.begin(); iter != rglst->listNumbering.end(); ++iter, ++i)
			{
				//start abstractNum
                m_pXmlWriter->WriteNodeBegin( L"w:abstractNum", TRUE );
                m_pXmlWriter->WriteAttribute( L"w:abstractNumId", FormatUtils::IntToWideString( i ));
                m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

				////nsid
                //m_pXmlWriter->WriteNodeBegin( L"w:nsid", TRUE );
                //m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToFormattedWideString( (*iter)->lsid, L"%08x" ) ));
                //m_pXmlWriter->WriteNodeEnd( L"", TRUE );

				//multiLevelType
                m_pXmlWriter->WriteNodeBegin( L"w:multiLevelType", TRUE );
				{
                    m_pXmlWriter->WriteAttribute( L"w:val", L"singleLevel" );
				}

                m_pXmlWriter->WriteNodeEnd( L"", TRUE );

				LevelMapping(*iter, 0);

				//end abstractNum
                m_pXmlWriter->WriteNodeEnd( L"w:abstractNum" );
			}

			//write the overrides
			for (unsigned int i = 0; i < m_document->listFormatOverrideTable->size(); ++i)
			{
				ListFormatOverride* lfo = m_document->listFormatOverrideTable->at(i);

				//start num
                m_pXmlWriter->WriteNodeBegin( L"w:num", TRUE );
                m_pXmlWriter->WriteAttribute( L"w:numId", FormatUtils::IntToWideString(i + 1));
                m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

				int index = FindIndexbyId( rglst->listData, lfo->lsid );

                m_pXmlWriter->WriteNodeBegin( L"w:abstractNumId", TRUE );
                m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToWideString( index ) );
                m_pXmlWriter->WriteNodeEnd( L"", TRUE );

				for (std::vector<ListFormatOverrideLevel*>::const_iterator iter = lfo->rgLfoLvl.begin(); iter != lfo->rgLfoLvl.end(); ++iter)
				{
                    m_pXmlWriter->WriteNodeBegin( L"w:lvlOverride", TRUE );

                    m_pXmlWriter->WriteAttribute( L"w:ilvl", FormatUtils::IntToWideString( (*iter)->ilvl ));
                    m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

					if ( ( (*iter)->fStartAt ) && ( !(*iter)->fFormatting ) )
					{
                        m_pXmlWriter->WriteNodeBegin( L"w:startOverride", TRUE );
                        m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToWideString( (*iter)->iStartAt ));
                        m_pXmlWriter->WriteNodeEnd( L"", TRUE );
					}

					LevelMapping((*iter)->lvl, 0, ListData::ISTD_NIL);

                    m_pXmlWriter->WriteNodeEnd(L"w:lvlOverride");
				}

                m_pXmlWriter->WriteNodeEnd(L"w:num");
			}

			if (m_document->listFormatOverrideTable->empty() &&  !rglst->listNumbering.empty())
			{
				i = 0;
				for (std::list<NumberingDescriptor>::iterator iter = rglst->listNumbering.begin(); iter != rglst->listNumbering.end(); ++iter, ++i)
				{
                    m_pXmlWriter->WriteNodeBegin( L"w:num", TRUE );
                    m_pXmlWriter->WriteAttribute( L"w:numId", FormatUtils::IntToWideString(i+1));
                    m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );


                    m_pXmlWriter->WriteNodeBegin( L"w:abstractNumId", TRUE );
                    m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToWideString( i ));
                    m_pXmlWriter->WriteNodeEnd( L"", TRUE );

                    m_pXmlWriter->WriteNodeEnd(L"w:num");
				}
			}

            m_pXmlWriter->WriteNodeEnd(L"w:numbering");

			m_xmldocument->NumberingXML = std::wstring(m_pXmlWriter->GetXmlString());
		}
	}

	int NumberingMapping::FindIndexbyId(const std::list<ListData*>& listData, int id)
	{
		int ret = -1;
		int i = 0;

		for (std::list<ListData*>::const_iterator iter = listData.begin(); iter != listData.end(); ++iter, ++i)
		{
			if ((*iter)->lsid == id)
			{
				ret = i;
				break;
			}
		}

		return ret;
	}

	// Converts the number text of the binary format to the number text of OOXML.
	// OOXML uses different placeholders for the numbers.

	std::wstring NumberingMapping::GetLvlText(const ListLevel* lvl, bool bIsSymbol) const
	{
		std::wstring ret;

		if (lvl != NULL)
		{
			if (lvl->nfc == 0x17)
			{
				if (!lvl->xst.empty())
				{
					wchar_t xchBullet = lvl->xst[0];

					// В символьном шрифте обрезать надо, в других случаях - нет
					if (true == bIsSymbol && (xchBullet & 0xF000) != 0)
					{
						xchBullet &= 0x0FFF;
					}

					if (!FormatUtils::IsControlSymbol(xchBullet))
					{
						ret.push_back(lvl->xst[0]);//??? xchBullet
					}
					else
					{
					}
				}
			}
			else
			{
				ret = lvl->xst;

				std::wstring::const_iterator result		=	lvl->xst.begin();
				std::wstring::const_iterator newResult	=	lvl->xst.begin();

				newResult								=	find_if(lvl->xst.begin(), lvl->xst.end(), &NumberingMapping::IsPlaceholder);
				ret										=	std::wstring(lvl->xst.begin(), newResult);
				result									=	newResult;

				while (result != lvl->xst.end())
				{
					newResult = find_if((result + 1), lvl->xst.end(), &NumberingMapping::IsPlaceholder);

                    ret += L"%";
					ret += FormatUtils::IntToWideString(*result + 1);
					ret += std::wstring((result + 1), newResult);
					result = newResult;
				}
			}
		}

		return ret;
	}
	
	std::wstring NumberingMapping::GetLvlText(const NumberingDescriptor& lvl, bool bIsSymbol, int Before, int After) const
	{
		std::wstring ret;

		if (lvl.nfc == 0xff)
		{
			if (!lvl.xst.empty())
			{
				wchar_t xchBullet = lvl.xst[0];

				// В символьном шрифте обрезать надо, в других случаях - нет
				if (bIsSymbol && (xchBullet & 0xF000) != 0)
				{
					xchBullet &= 0x0FFF;
				}

				if (!FormatUtils::IsControlSymbol(xchBullet))
				{
					ret.push_back(lvl.xst[0]);
				}
			}
			else
			{
				ret.push_back(L'\xF0B7');
			}
		}
		else
		{
			std::wstring strBefore		=	lvl.xst.substr(0, Before);
			std::wstring strAfter		=	lvl.xst.substr(Before, After);

            ret = strBefore + L"%1" + strAfter ;
		}

		return ret;
	}

	bool NumberingMapping::IsPlaceholder(wchar_t symbol)
	{
		if ((symbol == (wchar_t)0x0000) || (symbol == (wchar_t)0x0001) || (symbol == (wchar_t)0x0002) || 
			(symbol == (wchar_t)0x0003) || (symbol == (wchar_t)0x0004) || (symbol == (wchar_t)0x0005) || 
			(symbol == (wchar_t)0x0006) || (symbol == (wchar_t)0x0007) || (symbol == (wchar_t)0x0008))
		{
			return true;  
		}

		return false;
	}


	std::wstring NumberingMapping::GetNumberFormatWideString(int nfc, bool bOlderVersion)
	{
		if (bOlderVersion && nfc > 5)
		{
            if (nfc == 0xff)	return std::wstring( L"bullet");
            else				return std::wstring( L"none");
		}
		
		switch ( nfc )
		{
		case 0:
            return std::wstring( L"decimal" );
		case 1:
            return std::wstring( L"upperRoman" );
		case 2:
            return std::wstring( L"lowerRoman" );
		case 3:
            return std::wstring( L"upperLetter");
		case 4:
            return std::wstring( L"lowerLetter");
		case 5:
            return std::wstring( L"ordinal" );
		case 6:
            return std::wstring( L"cardinalText" );
		case 7:
            return std::wstring( L"ordinalText" );
		case 8:
            return std::wstring( L"hex" );
		case 9:
            return std::wstring( L"chicago" );
		case 10:
            return std::wstring( L"ideographDigital" );
		case 11:
            return std::wstring( L"japaneseCounting" );
		case 12:
            return std::wstring( L"aiueo" );
		case 13:
            return std::wstring( L"iroha" );
		case 14:
            return std::wstring( L"decimalFullWidth" );
		case 15:
            return std::wstring( L"decimalHalfWidth" );
		case 16:
            return std::wstring( L"japaneseLegal" );
		case 17:
            return std::wstring( L"japaneseDigitalTenThousand" );
		case 18:
            return std::wstring( L"decimalEnclosedCircle" );
		case 19:
            return std::wstring( L"decimalFullWidth2" );
		case 20:
            return std::wstring( L"aiueoFullWidth" );
		case 21:
            return std::wstring( L"irohaFullWidth" );
		case 22:
            return std::wstring( L"decimalZero" );
		case 23:
            return std::wstring( L"bullet" );
		case 24:
            return std::wstring( L"ganada" );
		case 25:
            return std::wstring( L"chosung" );
		case 26:
            return std::wstring( L"decimalEnclosedFullstop" );
		case 27:
            return std::wstring( L"decimalEnclosedParen" );
		case 28:
            return std::wstring( L"decimalEnclosedCircleChinese" );
		case 29:
            return std::wstring( L"ideographEnclosedCircle" );
		case 30:
            return std::wstring( L"ideographTraditional" );
		case 31:
            return std::wstring( L"ideographZodiac" );
		case 32:
            return std::wstring( L"ideographZodiacTraditional" );
		case 33:
            return std::wstring( L"taiwaneseCounting" );
		case 34:
            return std::wstring( L"ideographLegalTraditional" );
		case 35:
            return std::wstring( L"taiwaneseCountingThousand" );
		case 36:
            return std::wstring( L"taiwaneseDigital" );
		case 37:
            return std::wstring( L"chineseCounting" );
		case 38:
            return std::wstring( L"chineseLegalSimplified" );
		case 39:
            return std::wstring( L"chineseCountingThousand" );
		case 40:
            return std::wstring( L"koreanDigital" );
		case 41:
            return std::wstring( L"koreanCounting" );
		case 42:
            return std::wstring( L"koreanLegal" );
		case 43:
            return std::wstring( L"koreanDigital2" );
		case 44:
            return std::wstring( L"vietnameseCounting" );
		case 45:
            return std::wstring( L"russianLower" );
		case 46:
            return std::wstring( L"russianUpper" );
		case 47:
            return std::wstring( L"none" );
		case 48:
            return std::wstring( L"numberInDash" );
		case 49:
            return std::wstring( L"hebrew1" );
		case 50:
            return std::wstring( L"hebrew2" );
		case 51:
            return std::wstring( L"arabicAlpha" );
		case 52:
            return std::wstring( L"arabicAbjad" );
		case 53:
            return std::wstring( L"hindiVowels" );
		case 54:
            return std::wstring( L"hindiConsonants" );
		case 55:
            return std::wstring( L"hindiNumbers" );
		case 56:
            return std::wstring( L"hindiCounting" );
		case 57:
            return std::wstring( L"thaiLetters" );
		case 58:
            return std::wstring( L"thaiNumbers" );
		case 59:
            return std::wstring( L"thaiCounting" );
		default:
            return std::wstring( L"decimal" );
		}
	}

	void NumberingMapping::LevelMapping(const NumberingDescriptor& lvl, unsigned int level)
	{
		std::wstring fontFamily;
		bool isSymbol = false;
		
		if( lvl.ftc < m_document->FontTable->Data.size() )
		{
			FontFamilyName* ffn = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( lvl.ftc ) );
			isSymbol = (ffn->chs == 2);
			fontFamily = FormatUtils::XmlEncode(ffn->xszFtn);
		}

//--------------------------------------------------------------------------------
        m_pXmlWriter->WriteNodeBegin( L"w:lvl", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:ilvl", FormatUtils::IntToWideString(level));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

        m_pXmlWriter->WriteNodeBegin( L"w:start", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToWideString(lvl.iStartAt));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

        m_pXmlWriter->WriteNodeBegin( L"w:numFmt", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:val", GetNumberFormatWideString(lvl.nfc, true));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

//// suffix
//		m_pXmlWriter->WriteNodeBegin( L"w:suff", TRUE );
//		m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::MapValueToWideString(lvl.ixchFollow, &FollowingCharMap[0][0], 3, 8));
//		m_pXmlWriter->WriteNodeEnd( L"", TRUE );

// Number level text
		std::wstring lvlText = GetLvlText(lvl, isSymbol, lvl.cbTextBefore, lvl.cbTextAfter);

		//if (lvlText.empty() && lvl.ftc == 0)//auto
		//{
		//	lvlText.push_back(L'\xF0B7');
		//	lvlText.push_back(L'\0');

		//	fontFamily = L"Wingdings";
		//}
		if (!lvlText.empty())
		{
            m_pXmlWriter->WriteNodeBegin(L"w:lvlText",TRUE);
                m_pXmlWriter->WriteAttribute(L"w:val",lvlText);
            m_pXmlWriter->WriteNodeEnd(L"", TRUE);
		}

// jc
        m_pXmlWriter->WriteNodeBegin( L"w:lvlJc", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::MapValueToWideString(lvl.jc, &LevelJustificationMap[0][0], 3, 7));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );
// pPr
        m_pXmlWriter->WriteNodeBegin( L"w:pPr", FALSE );
        m_pXmlWriter->WriteNodeEnd(L"w:pPr");

//  rPr
        m_pXmlWriter->WriteNodeBegin( L"w:rPr", FALSE );

		if (!fontFamily.empty())
		{
            m_pXmlWriter->WriteNodeBegin( L"w:rFonts", TRUE );
				// w:hint="default"
                m_pXmlWriter->WriteAttribute(L"w:hAnsi",fontFamily);
                m_pXmlWriter->WriteAttribute(L"w:ascii",fontFamily);
            m_pXmlWriter->WriteNodeEnd( L"", TRUE );
		}
        m_pXmlWriter->WriteNodeEnd(L"w:rPr");
		
        m_pXmlWriter->WriteNodeEnd(L"w:lvl");
	}

	void NumberingMapping::LevelMapping(const ListLevel* lvl, unsigned int level, short styleIndex)
	{
		if (!lvl) return;

		XMLTools::CStringXmlWriter oWriterTemp;	//Временный writer,что не нарушать последовательность записи
//rPr
		RevisionData rev(lvl->grpprlChpx);
		CharacterPropertiesMapping cpMapping(&oWriterTemp, m_document, &rev, lvl->grpprlPapx, false);
		lvl->grpprlChpx->Convert(&cpMapping);

// Проверяем шрифт

        m_pXmlWriter->WriteNodeBegin( L"w:lvl", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:ilvl", FormatUtils::IntToWideString(level));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

// starts at

        m_pXmlWriter->WriteNodeBegin( L"w:start", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::IntToWideString(lvl->iStartAt));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

// number format

        m_pXmlWriter->WriteNodeBegin( L"w:numFmt", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:val", GetNumberFormatWideString(lvl->nfc));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

// suffix

        m_pXmlWriter->WriteNodeBegin( L"w:suff", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::MapValueToWideString(lvl->ixchFollow, &FollowingCharMap[0][0], 3, 8));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );

// style
// The style id is used for a reverse reference. 
// It can happen that the reference points to the wrong style.

		if (styleIndex != ListData::ISTD_NIL)
		{
            m_pXmlWriter->WriteNodeBegin( L"w:pStyle", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::XmlEncode(StyleSheetMapping::MakeStyleId(m_document->Styles->Styles->at(styleIndex))));
            m_pXmlWriter->WriteNodeEnd( L"", TRUE );
		}

// Number level text

        m_pXmlWriter->WriteNodeBegin(L"w:lvlText",TRUE);
        m_pXmlWriter->WriteAttribute(L"w:val",GetLvlText(lvl, cpMapping.CheckIsSymbolFont()));
        m_pXmlWriter->WriteNodeEnd(L"",TRUE);

		WriteLevelPictureBullet(lvl->grpprlChpx);

// legacy
		if (lvl->fWord6)
		{
            m_pXmlWriter->WriteNodeBegin( L"w:legacy", TRUE );
            m_pXmlWriter->WriteAttribute( L"w:legacy", L"1" );
            m_pXmlWriter->WriteNodeEnd( L"", TRUE );
		}
// jc
        m_pXmlWriter->WriteNodeBegin( L"w:lvlJc", TRUE );
        m_pXmlWriter->WriteAttribute( L"w:val", FormatUtils::MapValueToWideString(lvl->jc, &LevelJustificationMap[0][0], 3, 7));
        m_pXmlWriter->WriteNodeEnd( L"", TRUE );
// pPr
		bool isBidi = false;
		ParagraphPropertiesMapping oppMapping(m_pXmlWriter, m_context, m_document, NULL, isBidi);
		lvl->grpprlPapx->Convert(&oppMapping);

// пишем rPr

		m_pXmlWriter->WriteString(oWriterTemp.GetXmlString());

        m_pXmlWriter->WriteNodeEnd(L"w:lvl");
	}

	void NumberingMapping::PictureBulletsMapping()
	{
		for (std::map<int, int>::const_iterator iter = m_document->PictureBulletsCPsMap.begin(); iter != m_document->PictureBulletsCPsMap.end(); ++iter)
		{
			int fc		=	m_document->FindFileCharPos(iter->second);
			int fcEnd	=	m_document->FindFileCharPos(iter->second + 1);

			if (fc < 0 || fcEnd < 0 ) break;

			std::list<CharacterPropertyExceptions*>* chpxs = m_document->GetCharacterPropertyExceptions(fc, fcEnd);

			if ((chpxs != NULL) && (!chpxs->empty()))
			{
				PictureDescriptor pict(chpxs->front(), m_document->DataStream, fcEnd - fc, m_document->FIB->m_bOlderVersion);

				if ((pict.mfp.mm > 98) && (pict.shapeContainer != NULL))
				{
                    m_pXmlWriter->WriteNodeBegin( L"w:numPicBullet", TRUE );
                    m_pXmlWriter->WriteAttribute( L"w:numPicBulletId", FormatUtils::IntToWideString( iter->first ));
                    m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
                    m_pXmlWriter->WriteNodeBegin( L"w:pict" );

					//inline picture + bullete props
					if (pict.blipStoreEntry != NULL)
					{
						VMLPictureMapping oPicture(m_context, m_pXmlWriter, false, this, true);
						oPicture.m_isBullete = true;

						pict.Convert(&oPicture);
					}
					else
					{
						VMLShapeMapping oShape (m_context, m_pXmlWriter, NULL, &pict, this, true);
						oShape.m_isBullete = true;
						pict.shapeContainer->Convert(&oShape);
					}

                    m_pXmlWriter->WriteNodeEnd(L"w:pict");
                    m_pXmlWriter->WriteNodeEnd(L"w:numPicBullet");
				}  
			}

			RELEASEOBJECT( chpxs );
		}
	}

	void NumberingMapping::WriteLevelPictureBullet(const CharacterPropertyExceptions* grpprlChpx)
	{
		if (grpprlChpx)
		{
			unsigned int index = 0;
			bool isPictureBullet = false;

			for (std::list<SinglePropertyModifier>::const_iterator iter = grpprlChpx->grpprl->begin(); iter != grpprlChpx->grpprl->end(); ++iter)
			{
				switch(iter->OpCode)
				{
					case sprmCPbiIBullet:
					{
						index = FormatUtils::BytesToUInt32(iter->Arguments, 0, iter->argumentsSize);
					}break;

					case sprmCPbiGrf:
					{
						isPictureBullet = FormatUtils::BitmaskToBool(FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize), 0x1);  
					}break;
				}
			}

			if (isPictureBullet)
			{
                m_pXmlWriter->WriteNodeBegin(L"w:lvlPicBulletId",TRUE);
                m_pXmlWriter->WriteAttribute(L"w:val",FormatUtils::IntToWideString(index));
                m_pXmlWriter->WriteNodeEnd(L"",TRUE);
			}
		}
	}
}
