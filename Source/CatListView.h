//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  This file is part of BeConverter.
//
//  BeConverter is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  BeConverter is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with BeConverter.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CAT_LIST_VIEW_H
#define _CAT_LIST_VIEW_H

#include "ListView.h"

class CatListView : public BListView
{
    public:
        CatListView (BRect frame, const char *name,
            list_view_type type = B_SINGLE_SELECTION_LIST,
            uint32 resizeMask = B_FOLLOW_LEFT, uint32 flags = B_WILL_DRAW);

        virtual void        KeyDown (const char *bytes, int32 numBytes);
};

#endif /* _CATLISTVIEW_H */

