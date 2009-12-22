/*
 *  This file is part of WinSparkle (http://winsparkle.org)
 *
 *  Copyright (C) 2009 Vaclav Slavik
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _ui_h_
#define _ui_h_

#include "threads.h"

namespace winsparkle
{

class App;

/**
    The main UI thread.

    This is where any user interface (available updates, download progress
    etc.) are shown.

    This thread is only created when needed -- in most cases, it isn't. Once it
    is created, it runs indefinitely (without wasting CPU time -- it sleeps
    waiting for incoming messages).
 */
class UI : public Thread
{
public:
    /**
        Returns the thread if it exists already, otherwise starts it.
     */
    static UI *Get();

    /// Is the UI thread running?
    static bool IsRunning();

    /**
        Shuts the UI thread down.

        @note Currently, this may only be called once, from
              win_sparkle_cleanup().
     */
    static void ShutDown();

    /**
        Notifies the UI that no updates were found.

        If the UI thread is running and WinSparkle window is currently open
        (i.e. the user is manually checking for updates), "no updates found"
        message is shown. Otherwise, does nothing.
     */
    static void NotifyNoUpdates();

    /**
        Shows the WinSparkle window in "checking for updates..." state.
     */
    static void ShowCheckingUpdates();

    /**
        Sets HINSTANCE of the DLL.

        Must be called on DLL initialization.
     */
    static void SetDllHINSTANCE(HINSTANCE h) { ms_hInstance = h; }

protected:
    virtual void Run();
    virtual bool IsJoinable() const { return true; }

private:
    UI();
    static App *GetApp() { return Get()->m_app; }

    App *m_app;

    static HINSTANCE ms_hInstance;
};

} // namespace winsparkle

#endif // _ui_h_
