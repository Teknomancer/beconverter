// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

#include <malloc.h>
#include <string.h>
#include <ctype.h>

#include <Clipboard.h>
#include <Bitmap.h>
#include <Window.h>

#include "StaticLabel.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StaticLabel::StaticLabel (BRect frame, const char *name, const char *label,
                uint32 rmask, uint32 flags)
    : BView (frame, name, rmask, flags),

    fText (strdup (label)),
    fOldText (NULL),
    fSelected (false),
    fForeground ((rgb_color){0,0,0,255}),
    fBackground ((rgb_color){242,242,242,255}),
    fSelForeground ((rgb_color){255,255,255,255}),
    fSelBackground ((rgb_color){0,0,0,255}),
    fDarkEdge ((rgb_color){170,170,170,255}),
    fLiteEdge ((rgb_color){255,255,255,255})
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StaticLabel::~StaticLabel()
{
    if (fText != NULL)
        free (fText);

    if (fOldText != NULL)
        free (fOldText);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StaticLabel::Draw (BRect frame)
{
    RenderView ();
    BView::Draw (frame);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StaticLabel::Invalidate (BRect invalRect)
{
    RenderView ();
    BView::Invalidate (invalRect);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StaticLabel::RenderView (bool firstTime)
{
    // Buffered draw, lock the bitmap, draw to the bitmap, then render bitmap once
    backBitmap->Lock();

    BRect bounds (backView->Bounds());
    backView->SetDrawingMode (B_OP_COPY);

    if (firstTime == true)
    {
        // Fill up the background and draw the borders
        // When the selection changes, this section is executed for complete redraw
        backView->SetHighColor (fBackground);
        backView->FillRect (bounds);

        backView->SetHighColor (fLiteEdge);
        backView->StrokeRect (bounds);

        backView->SetHighColor (fDarkEdge);
        backView->StrokeLine (BPoint(0, 0), BPoint (bounds.right, 0));
        backView->StrokeLine (BPoint(0, 0), BPoint(0, bounds.bottom));
    }


    // Erase old text using backcolor
    float oldStringWidth = StringWidth (fOldText);
    float newStringWidth = StringWidth (fText);
    if (fOldText != NULL && firstTime == false) // firstTime check is CRITICAL (bugfix)
    {
        backView->MovePenTo (bounds.Width() / 2.0 - oldStringWidth / 2.0,
                        bounds.bottom - 3);
        backView->SetHighColor (fSelected ? fSelBackground : fBackground);
        backView->SetLowColor (fSelected ? fSelBackground : fBackground);
        backView->DrawString (fOldText);
    }

    if (fSelected)
    {
        backView->SetHighColor (0,0,0,255);
        backView->FillRect (BRect (bounds.Width() / 2.0 -
                StringWidth (fText) / 2.0 - 1, 2,
                bounds.Width() / 2.0 + StringWidth (fText) / 2.0,
                bounds.bottom - 2));
    }

    // Write new text
    backView->MovePenTo (bounds.Width() / 2.0 - newStringWidth / 2.0,
                    bounds.bottom - 3);
    backView->SetHighColor (fSelected ? fSelForeground : fForeground);
    backView->SetLowColor (fSelected ? fSelBackground : fBackground);
    backView->DrawString (fText);

    // Redraw dark left-side borders if needed
    if (newStringWidth >= bounds.Width()
        || oldStringWidth >= bounds.Width())
    {
        backView->SetHighColor (fDarkEdge);
        backView->StrokeLine (BPoint (0,0), BPoint (0,bounds.bottom));
    }

    // Sync the view, its very important we do this
    backView->Sync();
    DrawBitmapAsync (backBitmap, Bounds(), Bounds());

    backBitmap->Unlock();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StaticLabel::SetText (const char *text)
{
    if (fSelected == true)
    {
        fSelected = false;
        RenderView (true);
    }

    // Delete the previous text if any
    if (fOldText)
        free (fOldText);

    // Hold old text into fOldText, re-allocate fText for the new text
    fOldText = fText;
    fText = strdup (text);

    RenderView ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* StaticLabel::Text ()
{
    return fText;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StaticLabel::AttachedToWindow ()
{
    backBitmap = new BBitmap (Bounds(), B_RGB32, true);
    backView = new BView (Bounds(), "internal_view", B_FOLLOW_ALL_SIDES,
                        B_WILL_DRAW);
    backView->SetViewColor (fBackground);

    backBitmap->Lock();
    backBitmap->AddChild (backView);
    RenderView (true);
    backBitmap->Unlock();

    BView::AttachedToWindow();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StaticLabel::MouseDown (BPoint point)
{
    MakeFocus (true);
    fSelected = !fSelected;
    RenderView (true);

    BView::MouseDown (point);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StaticLabel::KeyDown (const char *bytes, int32 numBytes)
{
    // Copy to clipboard if C is pressed
    if (toupper (bytes[0]) == 'C' && fSelected == true)
    {
        BMessage *clipdata (NULL);

        if (be_clipboard->Lock() == true)
        {
            be_clipboard->Clear();
            clipdata = be_clipboard->Data();
            if (clipdata != NULL)
            {
                clipdata->AddData ("text/plain", B_MIME_TYPE, fText, strlen (fText));
                be_clipboard->Commit ();
            }
            be_clipboard->Unlock ();
        }
    }

    BView::KeyDown (bytes, numBytes);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool StaticLabel::IsSelected () const
{
    return fSelected;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
