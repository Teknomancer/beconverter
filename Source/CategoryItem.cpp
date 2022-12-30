// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

#include <View.h>
#include <Region.h>

#include <string.h>
#include <malloc.h>

#include "CategoryItem.h"

CategoryItem::CategoryItem (const char *text)
    : BListItem ()
{
    label = strdup (text);
    selBackColor = (rgb_color){0,0,0,255};
    selTextColor = (rgb_color){255,255,255,255};
}

CategoryItem::~CategoryItem()
{
    if (label)
        free (label);
}

void CategoryItem::DrawItem (BView *owner, BRect frame, bool complete)
{
    if (IsSelected() || complete)
    {
        rgb_color color;
        color = IsSelected() ? selBackColor : owner->ViewColor();
        owner->SetHighColor (color);
        owner->FillRect (frame);
        owner->SetLowColor (color);
    }
    else
        owner->SetLowColor (owner->ViewColor());

    owner->MovePenTo (frame.left + 5, frame.bottom - 4);

    if (IsEnabled())
    {
        if (IsSelected())
            owner->SetHighColor (selTextColor);
        else
            owner->SetHighColor (0,0,0,255);
    }
    else
        owner->SetHighColor (182,182,182,255);

    owner->DrawString (label);
}

