// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

#ifndef _MAIN_VIEW_H
#define _MAIN_VIEW_H

#include <View.h>

class Box1;
class BButton;

class MainView : public BView
{
    public:
        MainView ();
        ~MainView ();

        virtual void        AttachedToWindow ();
        virtual void        AllAttached ();

        Box1                *box1;
        BButton             *BtnConverter,
                            *BtnOptions,
                            *BtnAbout,
                            *BtnHelp,
                            *BtnClose;
};

#endif  /* _MAIN_VIEW_H */
