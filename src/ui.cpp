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

#include "ui.h"

namespace winsparkle
{
	WinSparkleUI* UI::_ui = NULL;

	void UI::SetUiCallbacks(WinSparkleUI* ui)
	{
		_ui = ui;
	}

	void UI::ShutDown()
	{
		if (_ui == nullptr)
			return;

		_ui->ShutdownUI();
	}

	void UI::NotifyNoUpdates()
	{
		if (_ui == nullptr)
			return;

		_ui->NotifyNoUpdates();
	}

	void UI::NotifyUpdateError(const std::string &msg)
	{
		if (_ui == nullptr)
			return;

		_ui->NotifyUpdateError(msg);
	}

	void UI::NotifyUpdateAvailable(const Appcast& info)
	{
		if (_ui == nullptr)
			return;

		WinSparkle_AppCastInfo appcastInfo;
		appcastInfo.Version = info.Version;
		appcastInfo.ShortVersionString = info.ShortVersionString;
		appcastInfo.DownloadURL = info.DownloadURL;
		appcastInfo.ReleaseNotesURL = info.ReleaseNotesURL;
		appcastInfo.WebBrowserURL = info.WebBrowserURL;
		appcastInfo.Title = info.Title;
		appcastInfo.Description = info.Description;
		appcastInfo.Os = info.Os;
		appcastInfo.MinOSVersion = info.MinOSVersion;
		appcastInfo.InstallerArguments = info.InstallerArguments;

		_ui->NotifyUpdateAvailable(appcastInfo);
	}

	void UI::NotifyDownloadProgress(size_t downloaded, size_t total)
	{
		if (_ui == nullptr)
			return;

		int progress = int((double(downloaded) / double(total))*100);
		_ui->NotifyDownloadProgress(progress);
	}

	void UI::NotifyUpdateDownloaded(const std::wstring& updateFile, const Appcast& appcast)
	{
		if (_ui == nullptr)
			return;

		_ui->NotifyDownloadComplete();
	}

} // namespace winsparkle
