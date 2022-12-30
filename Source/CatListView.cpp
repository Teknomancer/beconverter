// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

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
