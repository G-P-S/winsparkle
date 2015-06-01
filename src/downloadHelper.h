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

#ifndef _download_h_
#define _download_h_

#include <string>
#include "winsparkle-downloader.h"

namespace winsparkle
{

	/**
		IDownloadSink imlementation for storing data in a string.
	 */
	struct StringDownloadSink : public IDownloadSink
	{
		virtual void SetLength(size_t) {}

		virtual void SetFilename(const std::wstring&) {}

		virtual void Add(const void *data, size_t len)
		{
			this->data.append(reinterpret_cast<const char*>(data), len);
		}

		/// Downloaded data, as a string.
		std::string data;
	};

	/**
		Downloads a HTTP resource.

		Throws on error.

		@param url   URL of the resource to download.
		@param sink  Where to put downloaded data.
		@param flags Or-combination of DownloadFlag values.

		@see CheckConnection()
	 */

	class DownloadHelper
	{
	public:
		static void SetDownloaderOverride(WinSparkleDownloader *downloader);

		static void DownloadFile(const std::string& url, IDownloadSink *sink, int flags = 0);

	private:
		static WinSparkleDownloader* _downloader;
	};


} // namespace winsparkle

#endif // _download_h_
