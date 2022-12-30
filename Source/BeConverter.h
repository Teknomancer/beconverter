// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

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

