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

#include <ctype.h>
#include "CatListView.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CatListView::CatListView (BRect frame, const char *name,
                list_view_type type, uint32 resizeMask, uint32 flags)
    : BListView (frame, name, type, resizeMask, flags)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CatListView::KeyDown (const char *bytes, int32 numBytes)
{
    if (isalnum(bytes[0]) != 0)
    {
        BListItem **list = const_cast<BListItem**>(Items());
        int32 nItems = CountItems();
        for (int32 i = nItems - 1; i >= 0; i--)
        {
            BStringItem *item = (BStringItem*)*list;
            if (item)
            {
                if (toupper(*(item->Text())) == toupper(bytes[0]))
                {
                    Select (nItems - i - 1, false);
                    ScrollToSelection();
                    break;
                }
            }
            list++;
        }
    }

    BListView::KeyDown (bytes, numBytes);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
