#ifndef _defaultdownloader_h_
#define _defaultdownloader_h_

#include "winsparkle-downloader.h"

namespace winsparkle
{
	class DefaultDownloader : public WinSparkleDownloader
	{
	public:
		DefaultDownloader();
		virtual ~DefaultDownloader();

		void DownloadFile(const std::string& url, IDownloadSink* sink, int flags) override;
	};
}

#endif
