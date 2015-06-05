/*
 *  This file is part of WinSparkle (http://winsparkle.org)
 *
 *  Copyright (C) 2009-2015 Vaclav Slavik
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

#include "winsparkle.h"

#include "appcontroller.h"
#include "settings.h"
#include "error.h"
#include "ui.h"
#include "DownloadHelper.h"
#include "updatechecker.h"
#include "updatedownloader.h"
#include "UpdateScheduler.h"


#include <ctime>

using namespace winsparkle;

extern "C"
{

/*--------------------------------------------------------------------------*
                       Initialization and shutdown
 *--------------------------------------------------------------------------*/

WIN_SPARKLE_API void __cdecl win_sparkle_init(WinSparkleUI *ui, WinSparkleDownloader *downloader)
{
	UI::SetUiCallbacks(ui);
	DownloadHelper::SetDownloaderOverride(downloader);

	// Force create of registry entries
	bool didRunOnce;
	Settings::ReadConfigValue("DidRunOnce", didRunOnce, false);
	if ( !didRunOnce )
	{
	    // Do nothing on the first execution of the app, for better
	    // first-time impression.
	    Settings::WriteConfigValue("DidRunOnce", true);
	}

//    bool checkUpdates;
//    if ( Settings::ReadConfigValue("CheckForUpdates", checkUpdates) )
//    {
//        if ( checkUpdates )
//        {
//            static const time_t ONE_DAY = 60*60*24;
//
//            time_t lastCheck = 0;
//            Settings::ReadConfigValue("LastCheckTime", lastCheck);
//            const time_t currentTime = time(NULL);
//
//            // Only check for updates in reasonable intervals:
//            const int interval = win_sparkle_get_update_check_interval();
//            if ( currentTime - lastCheck >= interval )
//            {
//                // Run the check in background. Only show UI if updates
//                // are available.
//                UpdateChecker *check = new UpdateChecker();
//                check->Start();
//            }
//        }
//    }
//    else // not yet configured
//    {
//        bool didRunOnce;
//        Settings::ReadConfigValue("DidRunOnce", didRunOnce, false);
//        if ( !didRunOnce )
//        {
//            // Do nothing on the first execution of the app, for better
//            // first-time impression.
//            Settings::WriteConfigValue("DidRunOnce", true);
//        }
//        else
//        {
//            // Only when the app is launched for the second time, ask the
//            // user for their permission to check for updates.
////                UI::AskForPermission();
//        }
//    }
}


WIN_SPARKLE_API void __cdecl win_sparkle_cleanup()
{
	UpdateScheduler::DeInit();
	UI::ShutDown();
}


/*--------------------------------------------------------------------------*
                               Configuration
 *--------------------------------------------------------------------------*/

WIN_SPARKLE_API void __cdecl win_sparkle_set_appcast_url(const char *url)
{
    Settings::SetAppcastURL(url);
}

WIN_SPARKLE_API void __cdecl win_sparkle_set_app_details(const wchar_t *company_name,
                                                        const wchar_t *app_name,
                                                        const wchar_t *app_version)
{
    Settings::SetCompanyName(company_name);
    Settings::SetAppName(app_name);
    Settings::SetAppVersion(app_version);
}

WIN_SPARKLE_API void __cdecl win_sparkle_set_app_build_version(const wchar_t *build)
{
    Settings::SetAppBuildVersion(build);
}

WIN_SPARKLE_API void __cdecl win_sparkle_set_registry_path(const char *path)
{
    Settings::SetRegistryPath(path);
}

WIN_SPARKLE_API void __cdecl win_sparkle_set_automatic_check_for_updates(int state)
{
    Settings::WriteConfigValue("CheckForUpdates", state != 0);
	if (state != 0)
		UpdateScheduler::Init();
}

WIN_SPARKLE_API int __cdecl win_sparkle_get_automatic_check_for_updates()
{
    try
    {
        bool checkUpdates;
        if ( Settings::ReadConfigValue("CheckForUpdates", checkUpdates, false) )
        return checkUpdates ? 1 : 0;
    }
    CATCH_ALL_EXCEPTIONS

    return 0;
}

WIN_SPARKLE_API void __cdecl win_sparkle_set_automatic_scheduler_frequency(int frequencySeconds)
{
	UpdateScheduler::SetSchedulerFrequency(frequencySeconds);
}

WIN_SPARKLE_API int __cdecl win_sparkle_get_automatic_scheduler_frequency()
{
	return UpdateScheduler::GetSchedulerFrequency();
}


WIN_SPARKLE_API void __cdecl win_sparkle_set_update_check_interval(int interval)
{
//    static const int MIN_CHECK_INTERVAL = 3600; // one hour
//
//    if ( interval < MIN_CHECK_INTERVAL )
//    {
//        winsparkle::LogError("Invalid update interval (min: 3600 seconds)");
//        interval = MIN_CHECK_INTERVAL;
//    }

    Settings::WriteConfigValue("UpdateInterval", interval);
}

WIN_SPARKLE_API int __cdecl win_sparkle_get_update_check_interval()
{
    static const int DEFAULT_CHECK_INTERVAL = 60*60*24; // one day

    try
    {
        int interval;
        Settings::ReadConfigValue("UpdateInterval", interval, DEFAULT_CHECK_INTERVAL);
        return interval;
    }
    CATCH_ALL_EXCEPTIONS

    return DEFAULT_CHECK_INTERVAL;
}

WIN_SPARKLE_API time_t __cdecl win_sparkle_get_last_check_time()
{
    static const time_t DEFAULT_LAST_CHECK_TIME = -1;

    try
    {
        time_t last_check;
        Settings::ReadConfigValue("LastCheckTime", last_check, DEFAULT_LAST_CHECK_TIME);
        return last_check;
    }
    CATCH_ALL_EXCEPTIONS

    return DEFAULT_LAST_CHECK_TIME;
}

WIN_SPARKLE_API void __cdecl win_sparkle_set_can_shutdown_callback(win_sparkle_can_shutdown_callback_t callback)
{
    ApplicationController::SetCanShutdownCallback(callback);
}

WIN_SPARKLE_API void __cdecl win_sparkle_set_shutdown_request_callback(win_sparkle_shutdown_request_callback_t callback)
{
    ApplicationController::SetShutdownRequestCallback(callback);
}

/*--------------------------------------------------------------------------*
                              Manual usage
 *--------------------------------------------------------------------------*/

WIN_SPARKLE_API void __cdecl win_sparkle_check_update_with_ui()
{
	// Initialize UI thread and show progress indicator.
//    UI::ShowCheckingUpdates();

    // Then run the actual check in the background.
    UpdateChecker *check = new ManualUpdateChecker();
    check->Start();
}

WIN_SPARKLE_API void __cdecl win_sparkle_check_update_without_ui()
{
    // Run the check in background. Only show UI if updates
    // are available.
    UpdateChecker *check = new UpdateChecker();
    check->Start();
}

WIN_SPARKLE_API void __cdecl win_sparkle_skip_this_version()
{
	UpdateChecker::SkipCurrentVersion();
}

WIN_SPARKLE_API void __cdecl win_sparkle_install_update()
{
	std::string installerPath;
	std::string installerParams;
	if (!Settings::ReadConfigValue("UpdateInstallerPath", installerPath))
		return;

	if (!Settings::ReadConfigValue("UpdateInstallerParams", installerParams))
		return;

	std::wstring wInstallerPath = AnsiToWide(installerPath);
	std::wstring wInstallerParams = AnsiToWide(installerParams);
	ShellExecute(nullptr, L"open", wInstallerPath.c_str(), wInstallerParams.c_str(), nullptr, SW_SHOWNORMAL);
}

} // extern "C"
