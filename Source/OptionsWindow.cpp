// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// Copyright (c) 2023 OscarL.
// All rights reserved.

#include "message_labels.h"
#include "OptionsWindow.h"
#include "OptionsView.h"

#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <Application.h>
#include <Alert.h>
#include <Path.h>
#include <FindDirectory.h>
#include <Font.h>
#include <TextView.h>
#include <String.h>
#include <Directory.h>
#include <Rect.h>
#include <TextControl.h>
#include <CheckBox.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OptionsWindow::OptionsWindow ()
 : BWindow (BRect (250, 100+100, 381+150+30, 278+10+25), "Options",
        B_TITLED_WINDOW,
        B_NOT_CLOSABLE | B_NOT_MINIMIZABLE | B_NOT_ZOOMABLE | B_NOT_RESIZABLE,
        B_CURRENT_WORKSPACE)
{
    // Make the Options Window a modal one which blocks
    // all other windows of this app.
    SetFeel (B_MODAL_APP_WINDOW_FEEL);

    optionsView = new OptionsView();
    AddChild (optionsView);

    // Load the window position from Options_frame
    BPath path;
    find_directory (B_USER_SETTINGS_DIRECTORY, &path, true);

    char settings_path[B_PATH_NAME_LENGTH];
    sprintf (settings_path, "%s/BeConverter/Options_frame", path.Path());

    fstream f (settings_path, ios::in);

    long left_coord = 250;      // Default position
    long top_coord = 200;       // Default position

    if (f)
    {
        char setting[10];
        char value[30];

        while (! f.eof ())
        {
            f.getline (setting, 10, '=');
            f.getline (value, 30, '\n');

            if (strcasecmp (setting, "Left") == 0)
                left_coord = atoi (value);

            if (strcasecmp (setting, "Top") == 0)
                top_coord = atoi (value);

        }
    }

    MoveTo (left_coord, top_coord);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OptionsWindow::~OptionsWindow ()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool OptionsWindow::QuitRequested ()
{
    Hide();
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionsWindow::Validate_Precision ()
{
    // This procedure saves the settings in a
    // settings file in the user's settings folder

    // Its made as a member of the class so that it can
    // access the OptionsView and its child controls.

    int _precision = atoi (optionsView->TxtPrecision->Text ());

    if (_precision < 0)
    {
        BAlert *precision_alert;
        precision_alert = new BAlert ("Error",
                                "Error\n\nThe precision cannot be a negative value."
                                "\nBeConverter will take the precision as zero.\n\n"
                                "Is this acceptable?", "No", "Yes", NULL, B_WIDTH_AS_USUAL,
                                B_EVEN_SPACING, B_STOP_ALERT);

        BTextView *styleview;
        styleview = precision_alert->TextView ();
        styleview->SetStylable (true);

        BFont errfont;
        errfont.SetFace (B_BOLD_FACE);

        rgb_color errcolour;
        errcolour.red = 220;
        errcolour.green = 0;
        errcolour.blue = 0;
        errcolour.alpha = 255;

        styleview->SetFontAndColor (0, 5, &errfont, B_FONT_ALL, &errcolour);

        int32 button_index = precision_alert->Go ();

        if (button_index == 1)
        {
            // Create a dir called "BeConverter" in settings dir
            // if it doesn't exist and write settings in General_settings
            BPath path;
            find_directory (B_USER_SETTINGS_DIRECTORY, &path, true);

            char settings_path[B_PATH_NAME_LENGTH];

            char beconverter_settings[B_PATH_NAME_LENGTH];
            sprintf (beconverter_settings, "%s/BeConverter", path.Path ());

            BDirectory dir(path.Path());
            BDirectory settings_dir (path.Path ());

            dir.CreateDirectory (beconverter_settings, &settings_dir);
            sprintf (settings_path, "%s/General_settings", beconverter_settings);

            fstream f (settings_path, ios::out);

            if (!f)
            {
                BAlert *fatal_alert;
                char error[B_PATH_NAME_LENGTH + 1000];
                sprintf ("Fatal Error\n\nAn unknown error occured while trying to "
                            "write to \n\n%s\n\nCannot save the settings.",
                            settings_path);

                fatal_alert = new BAlert ("Error", error, "Damn!", NULL, NULL,
                                    B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
                fatal_alert->Go ();
                return;
            }

            f << "Precision=0" << endl;
            f << "Scientific Notation=" << optionsView->ChkSciNotation->Value() << endl;
            f << "Use Radians=" << optionsView->ChkUseRadians->Value() << endl;
            f << "Register Mime Types=" << optionsView->ChkRegMime->Value() << endl;

            f.close ();
            Hide ();
        }
    }
    else
    {
        // Create a dir called "BeConverter" in settings dir
        // if it doesn't exist and write settings in General_settings

        BPath path;
        find_directory (B_USER_SETTINGS_DIRECTORY, &path, true);

        char settings_path[B_PATH_NAME_LENGTH];

        char beconverter_settings[B_PATH_NAME_LENGTH];
        sprintf (beconverter_settings, "%s/BeConverter", path.Path ());

        BDirectory dir(path.Path());
        BDirectory settings_dir (path.Path ());

        dir.CreateDirectory (beconverter_settings, &settings_dir);
        sprintf (settings_path, "%s/General_settings", beconverter_settings);

        // Use fstream object to open and write to the file
        fstream f (settings_path, ios::out);

        if (!f)
        {
            BAlert *fatal_alert;
            char error[B_PATH_NAME_LENGTH + 1000];
            sprintf ("Fatal Error\n\nAn unknown error occured while trying to write to"
                        "\n\n%s\n\nCannot save the settings.", settings_path);

            fatal_alert = new BAlert ("Error", error, "Damn!", NULL, NULL,
                                B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
            fatal_alert->Go ();
            return;
        }

        f << "Precision=" << atoi (optionsView->TxtPrecision->Text()) << endl;
        f << "Scientific Notation=" << optionsView->ChkSciNotation->Value() << endl;
        f << "Use Radians=" << optionsView->ChkUseRadians->Value() << endl;
        f << "Register Mime Types=" << optionsView->ChkRegMime->Value() << endl;
        f.close ();

        // Now that everything is done close (hide) the window.
        Hide ();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionsWindow::SavePosition ()
{
    // Create a dir called "BeConverter" in settings dir
    // if it doesn't exist and write settings in General_settings

    BPath path;
    find_directory (B_USER_SETTINGS_DIRECTORY, &path, true);

    char winsettings[B_PATH_NAME_LENGTH];

    char beconverter_settings[B_PATH_NAME_LENGTH];
    sprintf (beconverter_settings, "%s/BeConverter", path.Path ());

    BDirectory dir(path.Path());
    BDirectory settings_dir (path.Path ());

    dir.CreateDirectory (beconverter_settings, &settings_dir);
    sprintf (winsettings, "%s/Options_frame", beconverter_settings);


    // Use fstream object to open a file and write the window
    // positions to it
    fstream f2 (winsettings, ios::out);

    if (!f2)
    {
        BAlert *fatal_alert;
        char error[B_PATH_NAME_LENGTH + 1000];
        sprintf ("Fatal Error\n\nAn unknown error occured while trying to write to"
                    "\n\n%s\n\nCannot save the settings.", winsettings);

        fatal_alert = new BAlert ("Error", error, "Damn!", NULL, NULL, B_WIDTH_AS_USUAL,
                            B_EVEN_SPACING, B_STOP_ALERT);
        fatal_alert->Go ();
        return;
    }

    // Get the window's frame (BRect) then save only the
    // left and top co-ords because its a non-resizable window
    BRect rect;
    rect = this->Frame ();

    f2 << "Left=" << rect.left << endl;
    f2 << "Top=" << rect.top << endl;

    f2.close ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionsWindow::MessageReceived (BMessage *message)
{
    switch (message->what)
    {
        case ID_OPTIONS_CANCEL :
            SavePosition ();
            Hide ();
            break;

        case ID_OPTIONS_OK :
            SavePosition ();
            Validate_Precision();
            break;

        default :
            BWindow::MessageReceived (message);
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
