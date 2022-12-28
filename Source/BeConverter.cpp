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
#include "BeConverter.h"
#include "MainWindow.h"
#include "OptionsWindow.h"
#include "ConverterWindow.h"
#include "MainView.h"
#include "Box1.h"
#include "CatListView.h"

#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <Application.h>
#include <ListView.h>
#include <Roster.h>
#include <Path.h>
#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Alert.h>
#include <Font.h>
#include <TextView.h>
#include <String.h>
#include <Mime.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BeConverter::BeConverter()
 : BApplication("application/x-vnd.BeConverter")
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BeConverter::~BeConverter ()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool BeConverter::quitapp = false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BeConverter::ReadyToRun ()
{
    if (quitapp == false)
    {
        // Load settings file to retrieve "register mime" or not
        BPath path;
        find_directory (B_USER_SETTINGS_DIRECTORY, &path, true);

        char settings_path[B_PATH_NAME_LENGTH];
        sprintf (settings_path, "%s/BeConverter/General_settings", path.Path());

        fstream f (settings_path, ios::in);

        if (f)
        {
            char tempbuf[50];
            char value[100];

            while (! f.eof ())
            {
                f.getline (tempbuf, 50, '=');
                f.getline (value, 100, '\n');

                // Yes... user wants to register file-types on startup
                if (strcasecmp(tempbuf, "Register Mime Types") == 0)
                {
                    this->RegFileTypes ();
                    break;
                }
            }
            f.close ();
        }

        // Create the main window
        mainWindow = new MainWindow();
    }
    else
    {
        be_app->PostMessage (B_QUIT_REQUESTED);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BeConverter::MessageReceived (BMessage *message)
{
    BPath path;
    char tempfile[B_PATH_NAME_LENGTH];

    switch (message->what)
    {
        case ID_BTN_OPTIONS:
            optionsWindow = new OptionsWindow();
            optionsWindow->Show();
            break;

        case ID_BTN_CONVERTER:
            {
                // Open a temp file and write the index to it
                // Then that index will be accessed using a BDirectory's
                // GetNextEntry method.
                find_directory (B_SYSTEM_TEMP_DIRECTORY, &path, true);
                sprintf (tempfile, "%s/Bec937373866", path.Path ());

                fstream f (tempfile, ios::out);

                long x = -1;
                if (f)
                {
                    x = (long)mainWindow->mainView->box1->catList->CurrentSelection ();

                    f << x << endl;
                }
                f.close ();

                if (x < 0)
                {
                    BAlert *myalert;
                    myalert = new BAlert ("Error", "Please select one category "
                                    "from the list of available categories, then "
                                    "click the \"Converter\" button.\n", "OK",
                                    NULL, NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING,
                                    B_STOP_ALERT);
                    myalert->Go ();
                }
                else
                {
                    app_info appInfo;
                    be_app->GetAppInfo (&appInfo);

                    BEntry appEntry (&appInfo.ref);
                    appEntry.GetParent (&appEntry);

                    BPath CurrentPath (&appEntry);
                    char units_path[B_PATH_NAME_LENGTH];

                    sprintf (units_path, "%s/Units", CurrentPath.Path ());

                    BDirectory udir;
                    udir.SetTo (units_path);

                    // Now get the path
                    BEntry entry;
                    BPath unitpath;
                    for (long c = 0; c <= x; c++)
                    {
                        if (udir.GetNextEntry (&entry, false) != B_ENTRY_NOT_FOUND)
                        {
                            if (c == x)
                                entry.GetPath (&unitpath);
                        }
                    }

                    bool winAlreadyOpen = false;
                    int32 i;
                    for (i = 0; i < CountWindows(); i++)
                    {
                        if (strcasecmp (WindowAt(i)->Title(), unitpath.Leaf()) == 0)
                        {
                            winAlreadyOpen = true;
                            break;
                        }
                    }

                    if (winAlreadyOpen == false)
                    {
                        converterWindow = new ConverterWindow();
                        converterWindow->Show();
                    }
                    else
                    {
                        WindowAt(i)->Activate ();
                    }
                }
            }
            break;

        case ID_CLOSE_CONVERTER_WINDOW :
            converterWindow->Hide();
            delete converterWindow;
            converterWindow = NULL;
            break;

        default:
            break;
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BeConverter::RefsReceived (BMessage *message)
{
    // This is where the control comes when the user
    // tries to open a unit file from BeConverter
    static bool shownInfo;
    if (shownInfo == true)
        return;

    shownInfo = true;
    BAlert *refalert;
    refalert = new BAlert ("Title", "BeConverter\n\nThis is a BeConverter's "
                        "unit descriptor file which helps you make your own "
                        "units in such a way BeConverter can understand them."
                        "\n\nFor more information read the \"Custom categories\" "
                        "section of the user's guide.\n", "OK");

    BFont headfont;
    headfont.SetFace (B_BOLD_FACE);
    headfont.SetSize (18);

    BTextView *txtvw;
    txtvw = refalert->TextView ();
    txtvw->SetStylable (true);
    txtvw->SetFontAndColor (0, 11, &headfont);

    rgb_color be_blue;
    be_blue.red = 0;
    be_blue.green = 0;
    be_blue.blue = 210;
    be_blue.alpha = 255;

    rgb_color be_red;
    be_red.red = 210;
    be_red.green = 0;
    be_red.blue = 0;
    be_red.alpha = 255;

    txtvw->SetFontAndColor (0, 1, &headfont, B_FONT_ALL, &be_blue);
    txtvw->SetFontAndColor (1, 2, &headfont, B_FONT_ALL, &be_red);

    refalert->Go ();

    quitapp = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool BeConverter::QuitRequested ()
{
    // Delete the temporary file created by BeConverter
    BPath path;
    find_directory (B_SYSTEM_TEMP_DIRECTORY, &path, false);

    char temp_file [B_PATH_NAME_LENGTH];
    sprintf (temp_file, "%s/Bec937373866", path.Path());

    BEntry temp_entry (temp_file);
    temp_entry.Remove ();

    // Quit the app and all running BLoopers
    be_app->PostMessage (B_QUIT_REQUESTED);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BeConverter::AboutRequested ()
{
    mainWindow->PostMessage (ID_BTN_ABOUT);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BeConverter::RegFileTypes ()
{
    // Declare the mime-type
    BMimeType factype("text/x-BeConverter");

    // Add attributes for the mime-type
    BMessage msg;
    msg.AddString ("attr:name", "Unit");
    msg.AddString ("attr:public_name", "Unit");
    msg.AddInt32 ("attr:type", B_STRING_TYPE);
    msg.AddBool ("attr:public", true);
    msg.AddBool ("attr:viewable", true);
    msg.AddBool ("attr:editable", true);
    msg.AddInt32 ("attr:width", 90);
    msg.AddBool ("attr:extra", false);
    msg.AddInt32 ("attr:alignment", B_ALIGN_LEFT);

    // Symbol
    msg.AddString ("attr:name", "Symbol");
    msg.AddString ("attr:public_name", "Symbol");
    msg.AddInt32 ("attr:type", B_STRING_TYPE);
    msg.AddBool ("attr:public", true);
    msg.AddBool ("attr:viewable", true);
    msg.AddBool ("attr:editable", true);
    msg.AddInt32 ("attr:width", 60);
    msg.AddBool ("attr:extra", false);
    msg.AddInt32 ("attr:alignment", B_ALIGN_LEFT);

    // Value
    msg.AddString ("attr:name", "Value");
    msg.AddString ("attr:public_name", "Value");
    msg.AddInt32 ("attr:type", B_STRING_TYPE);
    msg.AddBool ("attr:public", true);
    msg.AddBool ("attr:viewable", true);
    msg.AddBool ("attr:editable", true);
    msg.AddInt32 ("attr:width", 80);
    msg.AddBool ("attr:extra", false);
    msg.AddInt32 ("attr:alignment", B_ALIGN_LEFT);

    // Register the attribs and set description info for the mime-type
    factype.SetShortDescription ("BeConverter Factor Unit");
    factype.SetLongDescription ("BeConverter Factor Unit");
    factype.SetPreferredApp ("application/x-vnd.BeConverter", B_OPEN);
    factype.SetAttrInfo (&msg);

    //
    // Declare the next mime-type
    //
    BMimeType fortype("text/x-BeConverterFormula");

    // Unit
    BMessage msg2;
    msg2.AddString ("attr:name", "Unit");
    msg2.AddString ("attr:public_name", "Unit");
    msg2.AddInt32 ("attr:type", B_STRING_TYPE);
    msg2.AddBool ("attr:public", true);
    msg2.AddBool ("attr:viewable", true);
    msg2.AddBool ("attr:editable", true);
    msg2.AddInt32 ("attr:width", 90);
    msg2.AddBool ("attr:extra", false);
    msg2.AddInt32 ("attr:alignment", B_ALIGN_LEFT);

    // Symbol
    msg2.AddString ("attr:name", "Symbol");
    msg2.AddString ("attr:public_name", "Symbol");
    msg2.AddInt32 ("attr:type", B_STRING_TYPE);
    msg2.AddBool ("attr:public", true);
    msg2.AddBool ("attr:viewable", true);
    msg2.AddBool ("attr:editable", true);
    msg2.AddInt32 ("attr:width", 60);
    msg2.AddBool ("attr:extra", false);
    msg2.AddInt32 ("attr:alignment", B_ALIGN_LEFT);

    // To Equation
    msg2.AddString ("attr:name", "To Equation");
    msg2.AddString ("attr:public_name", "To Equation");
    msg2.AddInt32 ("attr:type", B_STRING_TYPE);
    msg2.AddBool ("attr:public", true);
    msg2.AddBool ("attr:viewable", true);
    msg2.AddBool ("attr:editable", true);
    msg2.AddInt32 ("attr:width", 90);
    msg2.AddBool ("attr:extra", false);
    msg2.AddInt32 ("attr:alignment", B_ALIGN_LEFT);

    // From Equation
    msg2.AddString ("attr:name", "From Equation");
    msg2.AddString ("attr:public_name", "From Equation");
    msg2.AddInt32 ("attr:type", B_STRING_TYPE);
    msg2.AddBool ("attr:public", true);
    msg2.AddBool ("attr:viewable", true);
    msg2.AddBool ("attr:editable", true);
    msg2.AddInt32 ("attr:width", 90);
    msg2.AddInt32 ("attr:alignment", B_ALIGN_LEFT);
    msg2.AddBool ("attr:extra", false);

    // Constant
    msg2.AddString ("attr:name", "Constant (k)");
    msg2.AddString ("attr:public_name", "Constant (k)");
    msg2.AddInt32 ("attr:type", B_STRING_TYPE);
    msg2.AddBool ("attr:public", true);
    msg2.AddBool ("attr:viewable", true);
    msg2.AddBool ("attr:editable", true);
    msg2.AddInt32 ("attr:width", 60);
    msg2.AddBool ("attr:extra", false);
    msg2.AddInt32 ("attr:alignment", B_ALIGN_LEFT);

    // Set attribs for registered type
    fortype.SetShortDescription ("BeConverter Formula Unit");
    fortype.SetLongDescription ("BeConverter Formula Unit");
    fortype.SetPreferredApp ("application/x-vnd.BeConverter", B_OPEN);
    fortype.SetAttrInfo (&msg2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    new BeConverter();
    be_app->Run();
    delete be_app;

    return B_OK;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

