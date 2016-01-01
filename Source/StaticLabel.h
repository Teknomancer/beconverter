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
