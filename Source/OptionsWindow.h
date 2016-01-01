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

#ifndef _OPTIONS_WINDOW_H
#define _OPTIONS_WINDOW_H

#include <Window.h>

class OptionsView;

class OptionsWindow : public BWindow
{
    public:
        OptionsWindow ();
        ~OptionsWindow ();

        virtual bool        QuitRequested ();
        virtual void        MessageReceived (BMessage *message);

        void                Validate_Precision ();
        void                SavePosition ();

        OptionsView         *optionsView;
};

#endif  /* _OPTIONS_WINDOW_H */
