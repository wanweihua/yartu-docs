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

#include "OOXDrawingGraphicReader.h"
#include "OOXShapeReader.h"

#include "../../../../ASCOfficePPTXFile/ASCOfficeDrawingConverter.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Theme.h"

int OOXGraphicReader::Parse( ReaderParameter oParam , RtfShapePtr & pOutput)
{
	if (m_ooxGraphic == NULL) return 0;
	
	if (m_ooxGraphic->element.is_init())
	{
		if (m_ooxGraphic->element.getType() == OOX::et_p_ShapeTree)
		{
			OOXShapeReader shapeReader(m_ooxGraphic->element.GetElem().operator->());
			return (shapeReader.Parse(oParam, pOutput) ? 1 : 0);
		}
		else
		{
			OOXShapeGroupReader groupReader(dynamic_cast<PPTX::Logic::SpTree*>(m_ooxGraphic->element.GetElem().operator->()));
			return (groupReader.Parse(oParam, pOutput) ? 1 : 0);
		}
	}
	if (m_ooxGraphic->olePic.IsInit())
	{
		pOutput->m_nShapeType = 75;
	
		OOXShapeReader::Parse(oParam, pOutput, &m_ooxGraphic->olePic->blipFill); // тут если false приходит - картинка-потеряшка
		return 1;
	}
	if (m_ooxGraphic->smartArt.IsInit())
	{
		m_ooxGraphic->smartArt->LoadDrawing();
		if (m_ooxGraphic->smartArt->m_diag.IsInit())
		{
			OOXShapeGroupReader groupReader(dynamic_cast<PPTX::Logic::SpTree*>(m_ooxGraphic->smartArt->m_diag.GetPointer()));
			return (groupReader.Parse(oParam, pOutput) ? 1 : 0);
		}
	}
//nullable_string			spid;
//nullable<Table>			table;
//nullable<ChartRec>		chartRec;
	return 0;
}


