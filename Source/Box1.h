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

#ifndef _BOX1_H
#define _BOX1_H

#include <Box.h>

class ListView;
class ScrollView;

class CatListView;

class Box1 : public BBox
{
    public:
        Box1 ();
        virtual ~Box1 ();

        virtual void        AttachedToWindow ();
        virtual void        AllAttached ();

        CatListView         *catList;
        BScrollView         *scroller;
};

#endif  /* _BOX1_H */

