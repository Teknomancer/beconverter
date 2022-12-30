// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

#ifndef _OPTIONS_VIEW_H
#define _OPTIONS_VIEW_H

#include <View.h>

class BTextControl;
class BCheckBox;
class BButton;

class OptionsView : public BView
{
    public:
        OptionsView ();
        ~OptionsView ();

        virtual void        AttachedToWindow ();
        virtual void        AllAttached ();

        BTextControl        *TxtPrecision;
        BCheckBox           *ChkSciNotation,
                            *ChkUseRadians,
                            *ChkRegMime;
        BButton             *BtnOptionsOK,
                            *BtnOptionsCancel;
};

#endif  /* _OPTIONS_VIEW_H */
