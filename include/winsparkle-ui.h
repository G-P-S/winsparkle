#ifndef _winsparkle_ui_h_
#define _winsparkle_ui_h_
#include <string>

struct WinSparkle_AppCastInfo
{
	/// App version fields
	std::string Version;
	std::string ShortVersionString;

	/// URL of the update
	std::string DownloadURL;

	/// URL of the release notes page
	std::string ReleaseNotesURL;

	/// URL to launch in web browser (instead of downloading update ourselves)
	std::string WebBrowserURL;

	/// Title of the update
	std::string Title;

	/// Description of the update
	std::string Description;

	// Operating system
	std::string Os;

	// Minimum OS version required for update
	std::string MinOSVersion;

	// Arguments passed on the the updater executable
	std::string InstallerArguments;
};

class WinSparkleUI
{
public:
	WinSparkleUI() {};
	virtual ~WinSparkleUI() {};

	virtual void ShutdownUI() = 0;
	virtual void NotifyNoUpdates() = 0;
	virtual void NotifyUpdateAvailable(const WinSparkle_AppCastInfo &updateInfo) = 0;
	virtual void NotifyUpdateError(const std::string &errorMessage) = 0;
	virtual void NotifyDownloadProgress(const int progress) = 0;
	virtual void NotifyDownloadComplete() = 0;
	virtual void NotifyDownloadError(const std::string &errorMessage) = 0;
};

#endif
