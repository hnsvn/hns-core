/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_BOOKMARKS_EXPORTER_BOOKMARK_HTML_WRITER_H_
#define HNS_IOS_BROWSER_API_BOOKMARKS_EXPORTER_BOOKMARK_HTML_WRITER_H_

#include <memory>

#include "base/values.h"

class ChromeBrowserState;

namespace base {
class FilePath;
}  // namespace base

// Observer for bookmark html output. Used only in tests.
class BookmarksExportObserver {
 public:
  enum class Result {
    kSuccess,
    kCouldNotCreateFile,
    kCouldNotWriteHeader,
    kCouldNotWriteNodes,
  };
  // Is invoked on the IO thread.
  virtual void OnExportFinished(Result result) = 0;

 protected:
  virtual ~BookmarksExportObserver() {}
};

namespace bookmark_html_writer {

// Writes the bookmarks out in the 'bookmarks.html' format understood by
// Firefox and IE. The results are written asynchronously to the file at |path|.
// Before writing to the file favicons are fetched on the main thread.
// TODO(sky): need a callback on failure.
void WriteBookmarks(ChromeBrowserState* browser_state,
                    const base::FilePath& path,
                    BookmarksExportObserver* observer);

void WriteBookmarks(base::Value::Dict encoded_bookmarks,
                    const base::FilePath& path,
                    BookmarksExportObserver* observer);

}  // namespace bookmark_html_writer

#endif  // HNS_IOS_BROWSER_API_BOOKMARKS_EXPORTER_BOOKMARK_HTML_WRITER_H_
