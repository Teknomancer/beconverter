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

#ifndef _CONVERTER_VIEW_H
#define _CONVERTER_VIEW_H

#include <View.h>
#include "XEvaluator.h"

class MenuField;
class PopUpMenu;
class RadioButton;
class Button;
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

