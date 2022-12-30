// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

#ifndef _CATEGORY_ITEM_H
#define _CATEGORY_ITEM_H

#include <ListItem.h>
#include <InterfaceDefs.h>

class BView;

class CategoryItem : public BListItem
{
    public:
        CategoryItem (const char *text);
        ~CategoryItem ();

        virtual void        DrawItem (BView *owner, BRect frame, bool complete=false);

    private:
        char                *label;
        rgb_color           selBackColor,
                            selTextColor;
};

#endif  /* _CATEGORY_ITEM_H */

