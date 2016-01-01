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

