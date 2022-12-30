// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

#ifndef _STATIC_LABEL_H
#define _STATIC_LABEL_H

#include <View.h>

class BBitmap;

class StaticLabel : public BView
{
    public:
        StaticLabel (BRect frame, const char *name, const char *label,
                uint32 rmask = B_FOLLOW_LEFT, uint32 flags = B_WILL_DRAW);
        virtual ~StaticLabel ();

        virtual void        Draw (BRect frame);
        virtual void        Invalidate (BRect rect);
        virtual void        AttachedToWindow ();
        virtual void        MouseDown (BPoint point);
        virtual void        KeyDown (const char *bytes, int32 numBytes);

        virtual void        SetText (const char *text);
        virtual char        *Text ();
        virtual bool        IsSelected () const;

    protected:
        virtual void        RenderView (bool firstTime = false);

        BView               *backView;
        BBitmap             *backBitmap;
        char                *fText,
                            *fOldText;
        bool                fSelected;
        rgb_color           fForeground,
                            fBackground,
                            fSelForeground,
                            fSelBackground,
                            fDarkEdge,
                            fLiteEdge;
};

#endif  /* _STATIC_LABEL_H */
