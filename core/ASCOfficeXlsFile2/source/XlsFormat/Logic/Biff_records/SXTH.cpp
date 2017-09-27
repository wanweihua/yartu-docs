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

#include "SXTH.h"

namespace XLS
{

SXTH::SXTH()
{
}


SXTH::~SXTH()
{
}

BaseObjectPtr SXTH::clone()
{
	return BaseObjectPtr(new SXTH(*this));
}


void SXTH::readFields(CFRecord& record)
{
	_UINT32 flags1;
	unsigned short flags2;

	record >> frtHeaderOld >> flags1 >> sxaxis >> isxvd >> csxvdXl >> flags2;

	record >> stUnique >> stDisplay >> stDefault >> stAll >> stDimension;

	record >> cisxvd;
	for (int i = 0; i < cisxvd; i++)
	{
		_INT32 val;
		record >> val;
		rgisxvd.push_back(val);
	}
	record >> cHiddenMemberSets;
	for (int i = 0; i < cHiddenMemberSets; i++)
	{
		HiddenMemberSet val;
		record >> val;
		rgHiddenMemberSets.push_back(val);
	}
	fMeasure					= GETBIT(flags1, 0);
	fOutlineMode				= GETBIT(flags1, 2);
	fEnableMultiplePageItems	= GETBIT(flags1, 3);
	fSubtotalAtTop				= GETBIT(flags1, 4);
	fSet						= GETBIT(flags1, 5);
	fDontShowFList				= GETBIT(flags1, 6);
	fAttributeHierarchy			= GETBIT(flags1, 7);
	fTimeHierarchy				= GETBIT(flags1, 8);
	fFilterInclusive			= GETBIT(flags1, 9);
	fKeyAttributeHierarchy		= GETBIT(flags1, 11);
	fKPI						= GETBIT(flags1, 12);	

	fDragToRow					= GETBIT(flags2, 0);
	fDragToColumn				= GETBIT(flags2, 1);
	fDragToPage					= GETBIT(flags2, 2);
	fDragToData					= GETBIT(flags2, 3);
	fDragToHide					= GETBIT(flags2, 4);
}

} // namespace XLS

