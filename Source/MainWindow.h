// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <Window.h>

class MainView;

class MainWindow : public BWindow
{
    public:
        MainWindow ();
        ~MainWindow ();

        virtual bool        QuitRequested ();
        virtual void        MessageReceived (BMessage *message);

        MainView            *mainView;
};

#endif  /* _MAIN_WINDOW_H */

