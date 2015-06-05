#ifndef _winsparkle_downloader_h_
#define _winsparkle_downloader_h_

#include <string>

/**
Abstraction for storing downloaded data.
*/
struct IDownloadSink
{
	virtual ~IDownloadSink()
	{
	}

	/**
	Inform the sink of total data size.

	Note that this is not guaranteed to be called.
	*/
	virtual void SetLength(size_t len) = 0;

	/**
	Inform the sink of detected filename
	*/
	virtual void SetFilename(const std::wstring& filename) = 0;

	/// Add chunk of downloaded data
	virtual void Add(const void *data, size_t len) = 0;
};

/// Flags for DownloadFile().
enum DownloadFlag
{
	/// Don't get resources from cache, always contact the origin server
	Download_NoCached = 1
};

class WinSparkleDownloader
{
public:
	virtual void DownloadFile(const std::string& url, IDownloadSink *sink, int flags = 0) = 0;
};

#endif
