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

