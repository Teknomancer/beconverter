// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// Copyright (c) 2023 OscarL.
// All rights reserved.

#include "message_labels.h"
#include "ConverterView.h"
#include "ConverterWindow.h"
#include "StaticLabel.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include <TextControl.h>
#include <Application.h>
#include <StringView.h>
#include <Alert.h>
#include <RadioButton.h>
#include <List.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <Path.h>
#include <Entry.h>
#include <Node.h>
#include <PopUpMenu.h>
#include <MenuField.h>
#include <String.h>
#include <CheckBox.h>
#include <Clipboard.h>
#include <Roster.h>
#include <Point.h>
#include <MenuItem.h>
#include <Bitmap.h>
#include <TranslationUtils.h>
#include <Button.h>


using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConverterView::ConverterView (const char *dir)
 : BView (BRect (0, 0, 411+75, 476), "ConverterView", B_FOLLOW_TOP_BOTTOM |
            B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW)
{
    // Declare private class variables (for styled BAlerts)
    BFont errfont;
    BTextView *errview;
    rgb_color errcolor;

    base_index = -1;
    can_convert = true;
    enough_units = true;

    TxtNumber = new BTextControl(BRect(8, 41-13-8, 202, 61-13-8), "TxtNumber", NULL,
                        "", new BMessage(ID_EXP_ENTERED),
                        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    TxtNumber->SetModificationMessage(new BMessage(ID_INITVALUE_CHANGED));

    stringview1 = new BStringView (BRect(9, 12-8, 202, 27-8), "StringView",
                        "Initial value :", B_FOLLOW_LEFT | B_FOLLOW_TOP,
                        B_WILL_DRAW | B_NAVIGABLE);

    BtnCopy = new BButton(BRect(218+1, 17, 301-8+1, 43), "BtnCopy", "Copy",
                    new BMessage(ID_CONVERTER_COPY), B_FOLLOW_LEFT | B_FOLLOW_TOP,
                    B_WILL_DRAW | B_NAVIGABLE);

    BtnClose = new BButton(BRect(316-16+1, 17, 399-16-8+1, 43), "BtnClose", "Close",
                    new BMessage(ID_CONVERTER_CLOSE), B_FOLLOW_LEFT | B_FOLLOW_TOP,
                    B_WILL_DRAW | B_NAVIGABLE);


    // Made the precision as a BMenu (Popup actually) instead of a button,
    //  -- requested by users --
    precisionMenu = new BPopUpMenu ("");
    for (int i = 0; i < 16; i++)
    {
        char buf [5];
        sprintf (buf, "%d", i);
        precisionMenu->AddItem (new BMenuItem (buf,
                        new BMessage (ID_PRECISION_CHANGED)));
    }

    precisionField = new BMenuField (BRect (409-20, 17, 465+20, 43), "PrecisionField",
                            ".# ", (BMenu*)precisionMenu,
                            B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    precisionField->SetDivider (StringWidth (".# W"));

    ChkSciNot = new BCheckBox (BRect(410, 53, 465+15, 53 + 19), "ChkSciNot", ".# e",
                        new BMessage (ID_SCINOT_CHANGED));
    SetViewColor((rgb_color) {219,219,219,255});


    // Load the precision, notation settings
    BPath s_path;
    find_directory (B_USER_SETTINGS_DIRECTORY, &s_path, true);

    char settings_path[B_PATH_NAME_LENGTH];
    sprintf (settings_path, "%s/BeConverter/General_settings", s_path.Path());

    fstream f(settings_path, ios::in);

    xcalc.UseDegrees();     // By default set to degrees
    if (f)
    {
        char tempbuf[50];
        char value[100];
        BString fullbuf;
        char prec_lbl[15];

        while (! f.eof ())
        {
            f.getline (tempbuf, 50, '=');
            f.getline (value, 100, '\n');

            if (strcasecmp(tempbuf, "Precision") == 0)
            {
                // Sanity check... in case user fiddles with the settings file
                if (atoi(value) > 100)
                    sprintf (value, "%s", "100");
                if (atoi(value) < 0)
                    sprintf (value, "%s", "0");

                sprintf (prec_lbl, ".# %s", value);
                for (int i = 0; i < 16; i++)
                {
                    BMenuItem *item = (BMenuItem*)precisionMenu->ItemAt(i);
                    if (strcasecmp (((BMenuItem*)precisionMenu->ItemAt (i))->Label(),
                            value) == 0)
                    {
                        item->SetMarked (true);
                        break;
                    }
                }

                precision = atoi (value);
            }
            if (strcasecmp(tempbuf, "Scientific Notation") == 0)
                ChkSciNot->SetValue (atoi(value));

            if (strcasecmp(tempbuf, "Use Radians") == 0)
            {
                if (atoi(value) == 1)
                    xcalc.UseRadians();
            }
        }
        f.close ();
    }


    // Load the units

    BDirectory unitdir;
    unitdir.SetTo (dir);

    base_index = -1;
    can_convert = true;
    enough_units = true;

    no_of_units = unitdir.CountEntries();

    if (no_of_units < 2)        // Too few units. Show error and close.
    {
        BAlert *myalt;
        myalt = new BAlert ("Error", "Error\n\nNumber of units are insufficient...\n",
                        "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING,
                        B_STOP_ALERT);

        errview = myalt->TextView ();
        errview->SetStylable (true);

        errcolor.red = 210; errcolor.green = 0; errcolor.blue = 0;
        errcolor.alpha = 255;

        errfont.SetFace (B_BOLD_FACE);

        errview->SetFontAndColor (0, 5, &errfont, B_FONT_ALL, &errcolor);
        myalt->Go ();

        // Make this global variable false so that the window can be
        // closed from another function
        enough_units = false;
        return;
    }

    if (no_of_units > 23)   // Max. limit... so load only that many
        no_of_units = 23;   // ignore rest

    fact_conv = true;

    long y_pos = 0;
    BEntry entry;
    BString curmime;

    for (int c = 0; c < (int)no_of_units; c++)
    {
        unitdir.GetNextEntry (&entry, false);

        BPath path;
        entry.GetPath (&path);

        BNode node;
        node.SetTo (path.Path());

        void *buffer;

        node.ReadAttr("BEOS:TYPE", B_STRING_TYPE, 0, &buffer, 255);
        curmime << (char *)&buffer;
        if ((strcasecmp((char *)&buffer, "text/x-BeConverterFormula") == 0) && (c == 0))
            fact_conv = false;

        // Read the unit attribute
        node.ReadAttr ("Unit", B_STRING_TYPE, 0, &buffer, 255);
        sprintf (tempbuffer[c], "%s", (char *)&buffer);

        // Read the symbol attribute
        node.ReadAttr ("Symbol", B_STRING_TYPE, 0, &buffer, 255);
        sprintf (symbuffer[c], "%s", (char *)&buffer);

        if (fact_conv == true && curmime == "text/x-BeConverter")
        {
            node.ReadAttr ("Value", B_STRING_TYPE, 0, &buffer, 1024);
            sprintf (valbuffer[c], "%s", (char *)&buffer);

            // Check if this is the base unit in the category
            if (atof(valbuffer[c]) == 1)
                base_index = c;
        }
        else if (fact_conv == false && curmime == "text/x-BeConverterFormula")
        {
            node.ReadAttr ("To Equation", B_STRING_TYPE, 0, &buffer, 1024);
            sprintf (toeqn[c], "%s", (char *)&buffer);

            node.ReadAttr ("From Equation", B_STRING_TYPE, 0, &buffer, 1024);
            sprintf (fromeqn[c], "%s", (char *)&buffer);

            node.ReadAttr ("Constant (k)", B_STRING_TYPE, 0, &buffer, 1024);
            sprintf (constant[c], "%s", (char *)&buffer);

            // Check if this is the base unit in the category
            if (strcasecmp(toeqn[c], fromeqn[c]) == 0)
                base_index = c;
        }
        else
        {
            BAlert *err_alert;
            err_alert = new BAlert ("Error",
                                "Error\n\nOne or more units are of the wrong type...\n",
                                "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING,
                                B_STOP_ALERT);

            errview = err_alert->TextView ();
            errview->SetStylable(true);

            errfont.SetFace (B_BOLD_FACE);

            errcolor.red = 210; errcolor.green = 0; errcolor.blue = 0;
            errcolor.alpha = 255;

            errview->SetFontAndColor (0, 5, &errfont, B_FONT_ALL, &errcolor);

            // This makes sure this alert is not shown several times
            if (can_convert != false)
                err_alert->Go ();

            can_convert = false;
        }
        curmime.SetTo ("");

        OptButton[c] = new BRadioButton (BRect(15, 53 + y_pos, 216, 71 + y_pos),
                                "OptUnit", tempbuffer[c],
                                new BMessage(ID_UNIT_OPTION), B_FOLLOW_LEFT,
                                B_WILL_DRAW | B_NAVIGABLE);

        TxtControl[c] = new StaticLabel (BRect(218, 53 + y_pos, 399, 71 + y_pos - 1),
                                "TxtUnit", symbuffer[c], B_FOLLOW_LEFT,
                                B_WILL_DRAW | B_NAVIGABLE);

        y_pos += 18;
    }

    if (base_index == -1)
    {
        // No base unit was identified. Cannot convert!
        BAlert *myalert;
        myalert = new BAlert ("Error", "Error\n\nThe base unit could not be "
                        "determined...\n", "OK", NULL, NULL, B_WIDTH_AS_USUAL,
                        B_EVEN_SPACING, B_STOP_ALERT);

        errview = myalert->TextView ();
        errview->SetStylable (true);

        errcolor.red = 210; errcolor.green = 0; errcolor.blue = 0;
        errcolor.alpha = 255;

        errfont.SetFace (B_BOLD_FACE);

        errview->SetFontAndColor (0, 5, &errfont, B_FONT_ALL, &errcolor);
        myalert->Go ();

        can_convert = false;
    }

    xcalc.SetPrecision (precision);
    xcalc.SetSciNotation (ChkSciNot->Value());

    if (no_of_units > 0)
        OptButton[0]->SetValue (B_CONTROL_ON);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConverterView::~ConverterView ()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConverterView::AttachedToWindow ()
{
    // Get the picture from rsrc & apply it as back picture
    BBitmap *image = BTranslationUtils::GetBitmap ('PNG ', "Converter Background");
    SetViewBitmap (image);

    AddChild (TxtNumber);
    AddChild (stringview1);
    AddChild (BtnCopy);
    AddChild (BtnClose);
    AddChild (precisionField);
    AddChild (ChkSciNot);

    TxtNumber->SetViewBitmap (image);
    ChkSciNot->SetViewBitmap (image);
    stringview1->SetViewBitmap (image);
    precisionField->SetViewBitmap (image);

    for (long c = 0; c < no_of_units; c++)
    {
        AddChild (OptButton[c]);
        AddChild (TxtControl[c]);
        OptButton[c]->SetViewBitmap (image);
        TxtControl[c]->SetViewBitmap (image);
    }

    SetViewColor((rgb_color) {219,219,219,255});

    BtnClose->SetTarget (this);

    if ((enough_units == false) || (can_convert == false))
        BtnClose->Invoke (new BMessage (ID_CONVERTER_CLOSE));

    BView::AttachedToWindow ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConverterView::AllAttached ()
{
    TxtNumber->SetAlignment(B_ALIGN_LEFT, B_ALIGN_LEFT);
    TxtNumber->SetDivider(0);
    TxtNumber->MakeFocus (true);

    BView::AllAttached ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ConverterView::Calc_New_Base ()
{
    for (int j = 0; j < no_of_units; j++)
    {
        if (OptButton[j]->Value() == B_CONTROL_ON)
            return j;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConverterView::MessageRecd (BMessage *message)
{
    // This isn't the regular MessageReceived function.
    // Its renamed to MessageRecd because we don't want
    // some controls to send messages to the view. For eg. the
    // quit message (window can be closed only from the window class)
    // We want to send them to the window.
    // So all messages are sent to the window's MessageReceived.
    // It will decide which to send here.

    switch (message->what)
    {
        case ID_EXP_ENTERED: case ID_UNIT_OPTION: case ID_SCINOT_CHANGED:
        {
            // User has entered an expression
            BString expression, response;
            int selstart, selstop, error;
            selstart = selstop = 0;

            expression.SetTo (TxtNumber->Text());
            error = xcalc.Calculate(&expression, &response, selstart, selstop);

            if (!error)
                this->Calculate (Calc_New_Base(), response.String());

            break;
        }

        case ID_INITVALUE_CHANGED :
            this->Calculate (Calc_New_Base ());
            break;

        case ID_PRECISION_CHANGED :
        {
            BMenuItem *sourceItem;
            message->FindPointer ("source", reinterpret_cast<void**>(&sourceItem));
            precision = atoi (sourceItem->Label());
            precisionField->Invalidate ();

            xcalc.SetPrecision (precision);
            MessageRecd (new BMessage(ID_EXP_ENTERED));
            break;
        }

        case ID_CONVERTER_COPY :
        {
            // Access the system clipboard, lock, clear, write
            // and commit the data

            BString copystring;
            copystring << TxtNumber->Text() << " " << tempbuffer[Calc_New_Base()];
            copystring << "\n\n";

            // Check if there is atleast one selection made
            bool shouldCopySelection = false;
            for (int j = 0; j < no_of_units; j++)
                if (TxtControl[j]->IsSelected())
                {
                    shouldCopySelection = true;
                    break;
                }

            // If no selections, copy ALL units (to buffer)
            if (shouldCopySelection == false)
            {
                for (int j = 0; j < no_of_units; j++)
                    copystring << "\t= " << TxtControl[j]->Text () << "\n";
            }
            else
            {
                // Copy only those selected units (to buffer)
                for (int j = 0; j < no_of_units; j++)
                    if (TxtControl[j]->IsSelected())
                        copystring << "\t= " << TxtControl[j]->Text () << "\n";
            }

            // Copy buffer to clipboard
            BMessage *clipdata = (BMessage *)NULL;
            if (be_clipboard->Lock () == true)
            {
                be_clipboard->Clear ();
                clipdata = be_clipboard->Data ();
                if (clipdata != NULL)
                {
                    clipdata->AddData ("text/plain", B_MIME_TYPE,
                            copystring.String (), copystring.Length());
                    be_clipboard->Commit ();
                }
                be_clipboard->Unlock ();
            }
            break;
        }

        default:
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConverterView::Calculate (int new_base, const char* text)
{
    // Convert It!!!
    double acv;

    // Remove all commas in the number.
    BString s;
    s << TxtNumber->Text ();
    s.RemoveAll (",");

    double conv;
    if (text == NULL)
        conv = atof(s.String());
    else
        conv = atof(text);

    char strn[1024];
    sprintf (strn, "%.*g", 310, conv);

    char n[1024];
    if (fact_conv == true)      // Factor based conversion
    {
        bool scinot = ChkSciNot->Value () == B_CONTROL_ON;
        double toBaseValue = conv / (atof (valbuffer[new_base]));
        for (int i = 0; i < no_of_units; i++)
        {
            acv = toBaseValue * atof(valbuffer[i]);
            if (scinot)
                sprintf (n, "%.*g %s", precision, acv, symbuffer[i]);
            else
                sprintf (n, "%.*f %s", precision, acv, symbuffer[i]);

            TxtControl[i]->SetText (n);
        }
    }
    else                        // Formula based conversion
    {
        BString t, res, addsym;
        int start, stop, error;
        start = stop = error = 0;

        t = toeqn[new_base];
        t.IReplaceAll (toeqn[base_index], strn);
        t.IReplaceAll ("k", constant[new_base]);

        error = xcalc.Calculate (&t, &res, start, stop);

        TxtControl[base_index]->SetText (res.String());

        for (int i = 0; i < no_of_units; i++)
        {
            if (i != base_index)
            {
                t = fromeqn[i];
                t.IReplaceAll (toeqn[base_index], TxtControl[base_index]->Text());
                t.IReplaceAll ("k", constant[i]);
                error = xcalc.Calculate (&t, &res, start, stop);

                if (! error)    // Is expression calculated correctly
                {
                    char ts[1024];
                    if (ChkSciNot->Value() == B_CONTROL_ON)
                    {
                        sprintf (ts, "%.*g %s", precision, atof(res.String()),
                            symbuffer[i]);
                    }
                    else
                    {
                        sprintf (ts, "%.*f %s", precision, atof(res.String()),
                            symbuffer[i]);
                    }

                    TxtControl[i]->SetText (ts);
                }
                else            // No ? then set the result as zero
                {
                    res.SetTo ("0");
                    res << " " << symbuffer[i];
                    TxtControl[i]->SetText (res.String());
                }
            }
            t.SetTo ("");
        }

        addsym = TxtControl[base_index]->Text ();
        addsym << " " << symbuffer[base_index];
        TxtControl[base_index]->SetText (addsym.String());
    }
    xcalc.SetSciNotation (ChkSciNot->Value ());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

