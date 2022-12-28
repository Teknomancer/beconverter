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

#include "message_labels.h"
#include "MainWindow.h"
#include "MainView.h"
#include "Box1.h"
#include "CatListView.h"    // new

#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Application.h>
#include <Alert.h>
#include <TextView.h>
#include <Font.h>
#include <Roster.h>
#include <Path.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Directory.h>
#include <InterfaceDefs.h>
#include <ScrollBar.h>
#include <ScrollView.h>
#include <View.h>
#include <ListView.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow ()
 : BWindow (BRect (250, 100, 416+150, 511-50), "BeConverter", B_TITLED_WINDOW,
    B_NOT_ZOOMABLE | B_NOT_H_RESIZABLE | B_NOT_V_RESIZABLE, B_CURRENT_WORKSPACE)
{
    mainView = new MainView ();
    AddChild (mainView);

    BPath path;
    find_directory (B_USER_SETTINGS_DIRECTORY, &path, true);

    char settings_path[B_PATH_NAME_LENGTH];
    sprintf (settings_path, "%s/BeConverter/BeConverter_frame", path.Path());

    fstream f (settings_path, ios::in);

    long left_coord = 250;
    long top_coord = 100;
    float scrollval = 0;
    long selectindex = 0;

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

            if (strcasecmp (setting, "Scroll") == 0)
                scrollval = atof (value);

            if (strcasecmp (setting, "Select") == 0)
                selectindex = atoi (value);
        }
    }

    this->MoveTo (left_coord, top_coord);
    BScrollBar *br = NULL;
    br = mainView->box1->scroller->ScrollBar(B_VERTICAL);
    br->SetValue (scrollval);

    mainView->box1->catList->Select (selectindex, false);

    Show();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow ()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool MainWindow::QuitRequested ()
{
    // Create a dir called "BeConverter" in the settings folder
    // if it doesn't already exist.

    BPath path;
    find_directory (B_USER_SETTINGS_DIRECTORY, &path, true);

    char settings_path[B_PATH_NAME_LENGTH];

    char beconverter_settings[B_PATH_NAME_LENGTH];
    sprintf (beconverter_settings, "%s/BeConverter", path.Path ());

    BDirectory dir(path.Path());
    BDirectory settings_dir (path.Path ());

    dir.CreateDirectory (beconverter_settings, &settings_dir);
    sprintf (settings_path, "%s/BeConverter_frame", beconverter_settings);

    fstream f (settings_path, ios::out);

    // If fstream init failed
    if (!f)
    {
        BAlert *fatal_alert;
        char error[B_PATH_NAME_LENGTH + 1000];
        sprintf ("Fatal Error\n\nAn unknown error occured while trying to write "
                "to\n\n%s\n\nCannot save the settings.", settings_path);

        fatal_alert = new BAlert ("Error", error, "Damn!", NULL, NULL, B_WIDTH_AS_USUAL,
                            B_EVEN_SPACING, B_STOP_ALERT);
        fatal_alert->Go ();

        // Close all windows and quit
        be_app->PostMessage (B_QUIT_REQUESTED);
        return true;
    }

    BRect rect;
    rect = this->Frame ();

    // Save window position, scrollbar position (value) and
    // currently selected category
    f << "Left=" << rect.left << endl;
    f << "Top=" << rect.top << endl;
    BScrollBar *br;
    br = mainView->box1->scroller->ScrollBar(B_VERTICAL);
    f << "Scroll=" << br->Value() << endl;
    f << "Select=" << mainView->box1->catList->CurrentSelection() << endl;

    f.close ();


    // Close all windows and quit
    be_app->PostMessage (B_QUIT_REQUESTED);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::MessageReceived (BMessage *message)
{
    BFont headfont;
    BTextView *vw;
    BAlert *aboutalert;
    rgb_color col;

    switch (message->what)
    {
        case ID_BTN_ABOUT:
            // Style the about box and display it

            headfont.SetSize (18);
            headfont.SetFace (B_BOLD_FACE);

            aboutalert = new BAlert("About", "BeConverter\nFreeware, Version 1.3\n\n"
                                            "Written by Ramshankar\n"
                                            "(ramshankar@themail.com)\n\nCopyright © "
                                            "2001-2003 Ramshankar.\nAll rights reserved.\n",
                                            "OK");

            vw = aboutalert->TextView();
            vw->SetStylable (true);
            vw->SetFontAndColor (0, 11, &headfont);

            col.red = 0;
            col.green = 0;
            col.blue = 220;
            col.alpha = 255;

            vw->SetFontAndColor (0, 1, &headfont, B_FONT_ALL, &col);

            col.red = 220;
            col.green = 0;
            col.blue = 0;
            vw->SetFontAndColor (1, 2, &headfont, B_FONT_ALL, &col);

            aboutalert->Go();
            break;

        case ID_BTN_OPTIONS :
            be_app->PostMessage (message);
            break;

        case ID_BTN_CONVERTER :
            be_app->PostMessage (message);
            break;

        case ID_BTN_HELP :
            // Launch the help file Index.htm
            {
                app_info appInfo;
                be_app->GetAppInfo (&appInfo);

                BEntry appEntry (&appInfo.ref);
                appEntry.GetParent (&appEntry);

                BPath docsPath (&appEntry);

                if (docsPath.Append("Docs/Index.htm") == B_OK)
                {
                    const char *ConstChars = docsPath.Path();
                    char *help_url = const_cast<char *>(ConstChars);

                    be_roster->Launch ("application/x-vnd.Be.URL.http", 1, &help_url);
                }
            }
            break;

        default :
            BWindow::MessageReceived (message);
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

