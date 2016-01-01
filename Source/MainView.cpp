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
#include <Bitmap.h>
#include <TranslationUtils.h>
#include <Entry.h>
#include <Path.h>
#include <Roster.h>
#include <Button.h>
#include <ListView.h>

#include <stdio.h>

#include "message_labels.h"
#include "MainView.h"
#include "Box1.h"
#include "CatListView.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainView::MainView()
 : BView (BRect (0, 0, 316, 411), "View", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
    box1 = new Box1();

    BtnConverter = new BButton (BRect(218, 30, 305, 59), "BtnConverter",
                            "Converter...", new BMessage(ID_BTN_CONVERTER),
                            B_FOLLOW_LEFT | B_FOLLOW_TOP,
                            B_WILL_DRAW | B_NAVIGABLE);
    BtnConverter->MakeDefault (true);

    BtnOptions = new BButton (BRect (218, 218-83+50, 305, 247-83+50-1),
                        "BtnOptions", "Options...", new BMessage(ID_BTN_OPTIONS),
                        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

    BtnAbout = new BButton (BRect (218, 260-83+50, 305, 289-83+50-1),
                        "BtnAbout", "About...", new BMessage(ID_BTN_ABOUT),
                        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

    BtnHelp = new BButton (BRect (218, 302-83+50, 305, 331-83+50-1), "BtnHelp",
                        "Help...", new BMessage(ID_BTN_HELP),
                        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

    BtnClose = new BButton (BRect (218, 342-83+50, 305, 371-83+50-1), "BtnClose",
                        "Close", new BMessage(B_QUIT_REQUESTED),
                        B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

    SetViewColor ((rgb_color) {219,219,219,255});
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainView::~MainView ()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainView::AttachedToWindow(void)
{
    AddChild (box1);
    AddChild (BtnConverter);
    AddChild (BtnOptions);
    AddChild (BtnAbout);
    AddChild (BtnHelp);
    AddChild (BtnClose);

    SetViewColor((rgb_color) {219,219,219,255});

    BtnAbout->SetTarget (this);
    BtnOptions->SetTarget (this);
    BtnConverter->SetTarget (this);

    BBitmap *image = BTranslationUtils::GetBitmap ('PNG ', "Main Background");
    SetViewBitmap (image);
    box1->SetViewBitmap (image);

    BView::AttachedToWindow ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainView::AllAttached ()
{
    box1->catList->MakeFocus (true);

    BView::AllAttached ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

