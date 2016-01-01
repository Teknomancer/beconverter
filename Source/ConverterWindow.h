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

#ifndef _CONVERTER_WINDOW_H
#define _CONVERTER_WINDOW_H

#include <Window.h>

class ConverterView;

class ConverterWindow : public BWindow
{
    public:
        ConverterWindow ();
        ~ConverterWindow ();

        virtual bool        QuitRequested ();
        virtual void        MessageReceived (BMessage *message);
        virtual void        DispatchMessage (BMessage *msg, BHandler *hnd);

        long                category_index;
        ConverterView       *converterView;
};

#endif  /* _CONVERTER_WINDOW_H */

