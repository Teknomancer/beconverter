// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// Copyright (c) 2023 OscarL.
// All rights reserved.

#include "message_labels.h"
#include "OptionsView.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include <Application.h>
#include <TextView.h>
#include <TextControl.h>
#include <Button.h>
#include <CheckBox.h>
#include <String.h>
#include <Path.h>
#include <FindDirectory.h>
#include <Roster.h>
#include <Bitmap.h>
#include <TranslationUtils.h>
#include <Entry.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OptionsView::OptionsView()
 : BView(BRect(0, 0, 281+30, 78+10+25), "OptionsView",
        B_FOLLOW_TOP_BOTTOM | B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW)
{
    TxtPrecision = new BTextControl(BRect(10, 11, 138, 31), "TxtPrecision",
                        "Precision:", "6", new BMessage(DEFAULT_MSG),
                        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    TxtPrecision->SetAlignment(B_ALIGN_LEFT, B_ALIGN_RIGHT);
    TxtPrecision->SetDivider(80);

    BTextView *precision_view;
    precision_view = TxtPrecision->TextView ();
    precision_view->DisallowChar ('.');

    ChkSciNotation = new BCheckBox(BRect(10, 36+8, 210, 52+8), "checkbox",
                            "Use scientific notation", new BMessage(DEFAULT_MSG),
                            B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    ChkSciNotation->SetValue(B_CONTROL_ON);

    ChkUseRadians = new BCheckBox(BRect(10, 55+8, 210, 53+19+8), "checkbox",
                            "Use radian values in expressions",
                            new BMessage(DEFAULT_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP,
                            B_WILL_DRAW | B_NAVIGABLE);
    ChkUseRadians->SetValue(B_CONTROL_OFF);

    ChkRegMime = new BCheckBox(BRect(10, 75+8, 210, 100), "checkbox",
                            "Register Mime types on start-up",
                            new BMessage(DEFAULT_MSG),
                            B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    ChkRegMime->SetValue(B_CONTROL_ON);

    BtnOptionsOK = new BButton (BRect(191+25, 14, 269+30, 36+4), "BtnOK", "OK",
                            new BMessage(ID_OPTIONS_OK), B_FOLLOW_LEFT | B_FOLLOW_TOP,
                            B_WILL_DRAW | B_NAVIGABLE);
    BtnOptionsOK->MakeDefault (true);

    BtnOptionsCancel = new BButton (BRect (191+25, 44+4, 269+30, 71+4), "BtnCancel",
                            "Cancel", new BMessage (ID_OPTIONS_CANCEL),
                            B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

    SetViewColor ((rgb_color) {219, 219, 219, 255});


    // Load the settings file
    BPath path;
    find_directory (B_USER_SETTINGS_DIRECTORY, &path, true);

    char settings_path[B_PATH_NAME_LENGTH];
    sprintf (settings_path, "%s/BeConverter/General_settings", path.Path());

    fstream f (settings_path, ios::in);

    if (f)
    {
        char tempbuf[50];
        char value[100];
        BString fullbuf;

        while (! f.eof ())
        {
            f.getline (tempbuf, 50, '=');
            f.getline (value, 100, '\n');

            if (strcasecmp(tempbuf, "Precision") == 0)
            {
                // Sanity check... in case the user
                // fiddles with the settings file
                if (atoi(value) < 0)
                    sprintf (value, "%s", "0");

                if (atoi(value) > 100)
                    sprintf (value, "%s", "100");

                TxtPrecision->SetText (value);
            }

            if (strcasecmp(tempbuf, "Scientific Notation") == 0)
                ChkSciNotation->SetValue (atoi(value));

            if (strcasecmp(tempbuf, "Use Radians") == 0)
                ChkUseRadians->SetValue (atoi(value));

            if (strcasecmp(tempbuf, "Register Mime Types") == 0)
                ChkRegMime->SetValue (atoi(value));

        }
        f.close ();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OptionsView::~OptionsView ()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionsView::AttachedToWindow ()
{
    // Get pictures from rsrc & apply it as back pics
    BBitmap *image = BTranslationUtils::GetBitmap ('PNG ', "Main Background");
    SetViewBitmap (image);

    AddChild (TxtPrecision);
    AddChild (ChkSciNotation);
    AddChild (BtnOptionsOK);
    AddChild (BtnOptionsCancel);
    AddChild (ChkUseRadians);
    AddChild (ChkRegMime);

    // Apply the background image
    TxtPrecision->SetViewBitmap (image);
    ChkSciNotation->SetViewBitmap (image);
    ChkUseRadians->SetViewBitmap (image);
    ChkRegMime->SetViewBitmap (image);

    SetViewColor ((rgb_color) {219,219,219,255});
    BView::AttachedToWindow ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionsView::AllAttached ()
{
    TxtPrecision->SetAlignment (B_ALIGN_LEFT, B_ALIGN_RIGHT);
    TxtPrecision->SetDivider (80L);

    BView::AllAttached ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

