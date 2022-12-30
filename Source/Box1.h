// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

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

