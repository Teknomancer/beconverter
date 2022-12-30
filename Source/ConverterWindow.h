// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

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

