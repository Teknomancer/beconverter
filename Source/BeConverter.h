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

#ifndef _BECONVERTER_H
#define _BECONVERTER_H

#include <Application.h>

class MainWindow;
class OptionsWindow;
class ConverterWindow;

class BeConverter : public BApplication
{
    public:
        BeConverter ();
        ~BeConverter ();

        virtual void        ReadyToRun ();
        virtual bool        QuitRequested ();
        virtual void        AboutRequested ();

        void                MessageReceived (BMessage *message);
        void                RefsReceived (BMessage *message);

        virtual void        RegFileTypes ();

        MainWindow          *mainWindow;
        OptionsWindow       *optionsWindow;
        ConverterWindow     *converterWindow;

    private:
        static bool quitapp;
};

#endif  /* _BECONVERTER_H */

