// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// Copyright (c) 2023 OscarL.
// All rights reserved.

#ifndef _CONVERTER_VIEW_H
#define _CONVERTER_VIEW_H

#include <View.h>
#include "XEvaluator.h"

class BCheckBox;
class BMenuField;
class BPopUpMenu;
class BRadioButton;
class BButton;
class BStringView;
class BTextControl;

class StaticLabel;

class ConverterView : public BView
{
    public:
        ConverterView (const char *dir);
        ~ConverterView ();

        virtual void        AttachedToWindow ();
        virtual void        AllAttached ();

        void                MessageRecd (BMessage *message);
        int                 Calc_New_Base ();
        void                Calculate (int new_base, const char* text = NULL);

        char                tempbuffer[255][25],
                            symbuffer[255][25],
                            valbuffer[255][25],
                            toeqn[255][25],
                            fromeqn[255][25],
                            constant[255][25];

        int                 no_of_units,
                            base_index,
                            precision;
        bool                can_convert,
                            enough_units,
                            fact_conv;

        BStringView         *stringview1;
        BButton             *BtnCopy,
                            *BtnClose;
        BPopUpMenu          *precisionMenu;
        BMenuField          *precisionField;
        BCheckBox           *ChkSciNot;
        BRadioButton        *OptButton[25];
        StaticLabel         *TxtControl[25];
        BTextControl        *TxtNumber;
        XEvaluator          xcalc;
};

#endif  /* _CONVERTER_VIEW_H */

