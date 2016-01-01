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

#include <Application.h>

#include "message_labels.h"
#include "ConverterWindow.h"
#include "ConverterView.h"

#include <Alert.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <Directory.h>
#include <Rect.h>
#include <Path.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Roster.h>
#include <String.h>

long nunits;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConverterWindow::ConverterWindow()
 : BWindow (BRect (80+130, 22+50, 491+130+75-10, 498+50), "BeConverter",
        B_TITLED_WINDOW, B_NOT_MINIMIZABLE | B_NOT_ZOOMABLE | B_NOT_H_RESIZABLE
        | B_NOT_V_RESIZABLE, B_CURRENT_WORKSPACE)
{
    // Getting the current dir of the app is a big headache!
    app_info appInfo;
    be_app->GetAppInfo (&appInfo);

    BEntry appEntry (&appInfo.ref);
    appEntry.GetParent (&appEntry);

    BPath CurrentPath (&appEntry);
    char units_path[B_PATH_NAME_LENGTH];

    sprintf (units_path, "%s/Units", CurrentPath.Path ());

    BDirectory udir;
    udir.SetTo (units_path);

    BPath path, unitpath;
    find_directory (B_COMMON_TEMP_DIRECTORY, &path, true);

    char f[B_PATH_NAME_LENGTH];
    sprintf (f, "%s/Bec937373866", path.Path());

    char ndex[100];
    fstream fname (f, ios::in);
    fname.getline (ndex, 100, '\n');

    fname.close ();


    // Now get the path
    BEntry entry;

    for (long c = 0; c <= atoi(ndex); c++)
    {
        if (udir.GetNextEntry (&entry, false) != B_ENTRY_NOT_FOUND)
        {
            if (c == atoi(ndex))
                entry.GetPath (&unitpath);
        }
    }

    BDirectory unitdir;
    unitdir.SetTo (unitpath.Path ());

    nunits = unitdir.CountEntries();
    BRect r;
    r = Frame ();

    if (nunits <= 23)
        ResizeTo (r.right - r.left, nunits * 18 + 66);
    else
        ResizeTo (r.right - r.left, 23 * 18 + 66);

    SetTitle (unitpath.Leaf ());


    // Load the window position
    BPath settings_path;
    find_directory (B_USER_SETTINGS_DIRECTORY, &settings_path, true);

    char winpos[B_PATH_NAME_LENGTH];
    sprintf (winpos, "%s/BeConverter/%s_frame", settings_path.Path (), unitpath.Leaf());

    fstream fwp (winpos, ios::in);

    long left_coord = 80+130;
    long top_coord = 22+50;

    if (fwp != NULL)
    {
        char setting[10];
        char value[30];

        while (! fwp.eof ())
        {
            fwp.getline (setting, 10, '=');
            fwp.getline (value, 30, '\n');

            if (strcasecmp (setting, "Left") == 0)
                left_coord = atoi (value);

            if (strcasecmp (setting, "Top") == 0)
                top_coord = atoi (value);

        }
    }

    fwp.close ();

    this->MoveTo (left_coord, top_coord);


    BRect rect;
    rect = Frame();

    converterView = new ConverterView (unitpath.Path ());
    converterView->ResizeTo (rect.right - rect.left, nunits * 18 + 66);
    AddChild (converterView);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConverterWindow::~ConverterWindow(void)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ConverterWindow::QuitRequested(void)
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
    sprintf (settings_path, "%s/%s_frame", beconverter_settings, Title());

    fstream f (settings_path, ios::out);

    // If fstream init failed
    if (f == NULL)
    {
        BAlert *fatal_alert;
        char error[B_PATH_NAME_LENGTH + 1000];
        sprintf ("Fatal Error\n\nAn unknown error occured while trying to write to\n\n%s\n\nCannot save the settings.", settings_path);

        fatal_alert = new BAlert ("Error", error, "Damn!", NULL, NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
        fatal_alert->Go ();

        Quit();
        return true;
    }

    BRect rect;
    rect = Frame ();

    f << "Left=" << rect.left << endl;
    f << "Top=" << rect.top << endl;

    f.close ();

    Quit();
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConverterWindow::MessageReceived (BMessage *message)
{
    // Send the messages to the view because
    // the controls aren't accessed from here since some private
    // variables can only be used from the view file
    switch (message->what)
    {
        case ID_CONVERTER_CLOSE:
            // Close the window
            this->QuitRequested ();
            break;

        default:
            // Send all other messages to the view
            converterView->MessageRecd (message);
            break;
    }

    BWindow::MessageReceived (message);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConverterWindow::DispatchMessage (BMessage *message, BHandler *hnd)
{
    switch (message->what)
    {
        case B_KEY_DOWN:
        {
            const char *bytes;
            if (message->FindString ("bytes", &bytes) == B_OK)
                if (bytes != NULL)
                    if (bytes[0] == B_ESCAPE)
                        QuitRequested ();
            break;
        }
    }

    BWindow::DispatchMessage (message, hnd);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

