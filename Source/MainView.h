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

#ifndef _MAIN_VIEW_H
#define _MAIN_VIEW_H

#include <View.h>

class Box1;
class BButton;

class MainView : public BView
{
    public:
        MainView ();
        ~MainView ();

        virtual void        AttachedToWindow ();
        virtual void        AllAttached ();

        Box1                *box1;
        BButton             *BtnConverter,
                            *BtnOptions,
                            *BtnAbout,
                            *BtnHelp,
                            *BtnClose;
};

#endif  /* _MAIN_VIEW_H */
