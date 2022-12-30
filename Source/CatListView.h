// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

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

