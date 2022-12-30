// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

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
