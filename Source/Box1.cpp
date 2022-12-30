// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// Copyright (c) 2023 OscarL.
// All rights reserved.

#include <Application.h>

#include "message_labels.h"
#include "Box1.h"
#include "CategoryItem.h"
#include "CatListView.h"

#include <Alert.h>
#include <Font.h>
#include <TextControl.h>
#include <ListItem.h>
#include <ScrollView.h>
#include <Directory.h>
#include <Entry.h>
#include <Path.h>
#include <String.h>
#include <iostream>
#include <Roster.h>
#include <stdio.h>
#include <Bitmap.h>
#include <TranslationUtils.h>
#include <ListView.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Box1::Box1()
 : BBox (BRect (10, 10, 203, 400-50), "box", B_FOLLOW_LEFT | B_FOLLOW_TOP,
        B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE_JUMP, B_FANCY_BORDER)
{
    catList = new CatListView (BRect (14, 21, 178-14, 377-50), "CatList");
    catList->SetSelectionMessage (new BMessage(ID_CAT_SEL_CHANGED));
    catList->SetInvocationMessage (new BMessage(ID_BTN_CONVERTER));

    SetLabel ("Categories");

    scroller = new BScrollView ("Scroll", catList, B_FOLLOW_LEFT, 0, false,
                        true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Box1::~Box1 ()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Box1::AttachedToWindow ()
{
    AddChild (scroller);
    SetLabel("Categories");

    BBox::AttachedToWindow();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Box1::AllAttached ()
{
    // Read all categories into the catlist BListView object.

    // Getting the current dir of the app is a big headache!
    app_info appInfo;
    be_app->GetAppInfo (&appInfo);

    BEntry appEntry (&appInfo.ref);
    appEntry.GetParent (&appEntry);

    BPath CurrentPath (&appEntry);      // Got it!
    CurrentPath.Append ("Units/");      // Add "Units" folder to it

    BDirectory CatDir(CurrentPath.Path());

    long no_of_categories = CatDir.CountEntries ();

    BEntry entry;
    BPath path;

    for (long c = 0; c <= no_of_categories; c++)
    {
        if (CatDir.GetNextEntry (&entry, false) != B_ENTRY_NOT_FOUND)
        {
            entry.GetPath (&path);

            //CatList->AddItem (new BStringItem (path.Leaf()));
            CategoryItem *item = new CategoryItem (path.Leaf());
            catList->AddItem (item);
            item->SetHeight (item->Height() + 3);
        }
    }


    // Check if there is atleast one category. If not flag an error
    if (catList->CountItems() == 0)
    {
        BAlert *myalert;
        myalert = new BAlert ("error",
                        "Error\n\nNo categories were found...\n", "OK", NULL,
                        NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);

        BTextView *altview;
        altview = myalert->TextView ();
        altview->SetStylable (true);

        rgb_color errcolour;
        errcolour.red = 210;
        errcolour.green = 0;
        errcolour.blue = 0;
        errcolour.alpha = 255;

        BFont headfont;
        headfont.SetFace (B_BOLD_FACE);

        altview->SetFontAndColor (0, 5, &headfont, B_FONT_ALL, &errcolour);
        myalert->Go ();
    }

    BBox::AllAttached();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

