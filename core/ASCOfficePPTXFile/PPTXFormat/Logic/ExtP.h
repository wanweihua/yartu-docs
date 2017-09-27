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
#ifndef PPTX_LOGIC_EXTP_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_EXTP_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/RId.h"

//<p:ext uri="{DAA4B4D4-6D71-4841-9C94-3DE7FCFB9230}">
//<p14:media xmlns:p14="http://schemas.microsoft.com/office/powerpoint/2010/main" r:embed="rId2">
//<p14:trim st="93333.0816" end="96583.4816"/>
//</p14:media>
//</p:ext>

//<ext uri="{05C60535-1F16-4fd2-B633-F4F36F0B64E0}" xmlns:x14="http://schemas.microsoft.com/office/spreadsheetml/2009/9/main">
// <x14:sparklineGroups xmlns:xm="http://schemas.microsoft.com/office/excel/2006/main">
//  <x14:sparklineGroup displayEmptyCellsAs="gap">
//   <x14:colorSeries theme="5" tint="-0.499984740745262"/>
//   <x14:colorNegative theme="6"/>
//   <x14:colorAxis rgb="FF000000"/>
//   <x14:colorMarkers theme="5" tint="-0.499984740745262"/>
//   <x14:colorFirst theme="5" tint="0.39997558519241921"/>
//   <x14:colorLast theme="5" tint="0.39997558519241921"/>
//   <x14:colorHigh theme="5"/>
//   <x14:colorLow theme="5"/>
//   <x14:sparklines>
//    <x14:sparkline>
//     <xm:f>Лист1!D10:D12</xm:f>
//     <xm:sqref>H12</xm:sqref>
//    </x14:sparkline>
//   </x14:sparklines>
//  </x14:sparklineGroup>
// </x14:sparklineGroups>
//</ext>

//<a:ext uri="{63B3BB69-23CF-44E3-9099-C40C66FF867C}">
// <a14:compatExt spid="_x0000_s1025"/>
//</a:ext>

namespace PPTX
{
	namespace Logic
	{
		class Ext : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Ext)

			Ext()
			{
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);
				
				if ( oReader.IsEmptyNode() )
					return;
						
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (strName == L"media")
					{
						ReadAttributes1(oReader);

						int nParentDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
						{
							std::wstring strName1 = XmlUtils::GetNameNoNS(oReader.GetName());

							if (strName1 == L"trim")
							{
								ReadAttributes2(oReader);
							}
						}
					}
					else if (strName == L"compatExt")
					{
						ReadAttributes3(oReader);
					}
				}
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle ( oReader, _T("uri"),	uri)
				WritingElement_ReadAttributes_End( oReader )
			}
			void ReadAttributes1(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("r:embed"),	link)
				WritingElement_ReadAttributes_End( oReader )
			}
			void ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("st"),	st)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("end"),	end)
				WritingElement_ReadAttributes_End( oReader )
			}
			void ReadAttributes3(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle ( oReader, _T("spid"),	spid)
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{//todooo списком .. 
				XmlUtils::CXmlNode media = node.ReadNodeNoNS(_T("media"));
				if (media.IsValid())
				{
					link = media.GetAttribute(_T("r:embed"));

					XmlUtils::CXmlNode trim = media.ReadNodeNoNS(_T("trim"));
					if (trim.IsValid())
					{
						trim.ReadAttributeBase(_T("st"), st);
						trim.ReadAttributeBase(_T("end"), end);
					}
					return;
				}
				XmlUtils::CXmlNode compatExt = node.ReadNodeNoNS(_T("compatExt"));
				if (compatExt.IsValid())
				{
					spid = media.GetAttribute(_T("spid"));
					return;
				}
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				XmlUtils::CNodeValue oValue;

				return XmlUtils::CreateNode(L"p:ext", oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (link.IsInit() && uri.IsInit())
				{
					pWriter->StartNode(L"p:ext");
						pWriter->StartAttributes();
						pWriter->WriteAttribute(L"uri", *uri);
						pWriter->EndAttributes();

						pWriter->StartNode(L"p14:media");
							pWriter->StartAttributes();
							pWriter->WriteAttribute(L"xmlns:p14", std::wstring(L"http://schemas.microsoft.com/office/powerpoint/2010/main"));
							pWriter->WriteAttribute(L"r:embed", link->get());
							pWriter->EndAttributes();
						pWriter->EndNode(L"p14:media");
					pWriter->EndNode(L"p:ext");
				}
			}
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				if (link.IsInit())
					link->toPPTY(0, pWriter);
				pWriter->WriteDouble2(1, st);
				pWriter->WriteDouble2(2, end);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
//-------------------------------------------------------
			nullable_string			uri;
//media
			nullable<OOX::RId>		link;
			nullable_double			st;
			nullable_double			end;
			
//compatExt
			nullable_string			spid;
//dataModelExt
//table
//sparklineGroups
		protected:
			virtual void FillParentPointersForChilds()
			{

			}
		};
	} 
} 

#endif 