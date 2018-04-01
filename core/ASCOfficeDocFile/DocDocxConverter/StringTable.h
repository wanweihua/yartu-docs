﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#pragma once

#include "VirtualStreamReader.h"
#include "WideString.h"
#include "IVisitable.h"

#include "../Common/FormatUtils.h"

namespace DocFileFormat
{
	template<class T> class StringTable: public IVisitable
	{
		friend class WordDocument;
		friend class CharacterPropertiesMapping;
		friend class FontTableMapping;
		friend class StyleSheetMapping;	
		friend class DocumentMapping;
 		friend class NumberingMapping;
 
    private:
		bool						fExtend;
		int							cbData;
		unsigned short				cbExtra;
		
		std::vector<ByteStructure*>	Data;

		int							code_page;
	public:
		virtual ~StringTable()
		{
			for ( std::vector<ByteStructure*>::iterator iter = this->Data.begin(); iter != this->Data.end(); iter++ )
			{
				RELEASEOBJECT( *iter );
			}
		}

		StringTable( VirtualStreamReader *reader, int code_page_ ): 
                            code_page(code_page_), fExtend(false), cbData(0), cbExtra(0)
		{
            parse( reader, (unsigned int)reader->GetPosition() );
		}

		StringTable( POLE::Stream* tableStream, unsigned int fc, unsigned int lcb, bool older ) :
												code_page(1250), fExtend(false), cbData(0), cbExtra(0)
		{
			if ( lcb > 0 )
			{
				VirtualStreamReader reader( tableStream, fc, older);

				parse( &reader, fc, lcb ) ;
			}
		}

		ByteStructure* operator [] ( size_t index ) const
		{
			if ( index < this->Data.size() )
			{
				return this->Data.at( index );
			}
			else
			{
				return NULL;
			}
		}

	private:
		
		void parse( VirtualStreamReader *reader, unsigned int fc, unsigned int lcb = 0 )
		{
			if ( reader == NULL )		return;
			if (fc > reader->GetSize()) return;

			this->cbData = lcb;
			
			int count_elements = 0;
			//read fExtend
			if ( reader->ReadUInt16() == 0xFFFF )
			{
				//if the first 2 bytes are 0xFFFF the STTB contains unicode characters
				this->fExtend = true;
				code_page = ENCODING_UTF16;
			}
			else
			{
				//else the STTB contains 1byte characters and the fExtend field is non-existend
				//seek back to the beginning
				this->fExtend = false;
				code_page = ENCODING_WINDOWS_1250;

				reader->Seek( (int)fc, 0/*STREAM_SEEK_SET*/ );
			}

			//read cData
			long cDataStart = reader->GetPosition();
			unsigned short c = reader->ReadUInt16();

			if ( c != 0xFFFF )
			{
				if (reader->olderVersion)
					this->cbData = (int)c; // all size 
				else
					count_elements = c;
				
			}
			else
			{
				//cData is a 4byte signed Integer, so we need to seek back
				reader->Seek( (int)( fc + cDataStart ), 0/*STREAM_SEEK_SET*/ );

				this->cbData = reader->ReadInt32();
			}
			//read cbExtra
			if (reader->olderVersion == false)
			{
				this->cbExtra = reader->ReadUInt16();
			}

			if (lcb > 0 && (lcb/* - fc*/) != this->cbData + this->cbExtra)
			{
				//???? 
			}

			while(true)	//read the strings and extra datas
			{
				if (reader->GetPosition() >= fc + this->cbData + this->cbExtra && count_elements < 1)
					break; //1995 and older

				if (this->Data.size() == count_elements && count_elements > 0)
					break; //1997 and newer

				int cchData = 0;
				int cbData = 0;
        
				if ( this->fExtend )
				{
					cchData = (int)reader->ReadUInt16();
					cbData = cchData * 2;
				}
				else
				{
					cchData = (int)reader->ReadByte();
					cbData = cchData;
				}

				long posBeforeType = reader->GetPosition();

				this->Data.push_back( T().ConstructObject( reader, cbData ) );

				reader->Seek( (int)( posBeforeType + cbData ), 0/*STREAM_SEEK_SET */);
	        
				
				reader->ReadBytes( cbExtra, false );//skip the extra unsigned char
			}
		}
	};
}
