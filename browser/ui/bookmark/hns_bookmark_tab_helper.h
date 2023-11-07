/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_BOOKMARK_HNS_BOOKMARK_TAB_HELPER_H_
#define HNS_BROWSER_UI_BOOKMARK_HNS_BOOKMARK_TAB_HELPER_H_

#include "chrome/browser/ui/bookmarks/bookmark_tab_helper.h"
#include "content/public/browser/web_contents_user_data.h"

class BookmarkTabHelperObserver;

// This proxies BookmarkTabHelper apis that used by Browser.
class HnsBookmarkTabHelper
    : public content::WebContentsUserData<HnsBookmarkTabHelper> {
 public:
  HnsBookmarkTabHelper(const HnsBookmarkTabHelper&) = delete;
  HnsBookmarkTabHelper& operator=(const HnsBookmarkTabHelper&) = delete;
  ~HnsBookmarkTabHelper() override;

  bool ShouldShowBookmarkBar();
  void AddObserver(BookmarkTabHelperObserver* observer);
  void RemoveObserver(BookmarkTabHelperObserver* observer);

 private:
  friend class content::WebContentsUserData<HnsBookmarkTabHelper>;

  explicit HnsBookmarkTabHelper(content::WebContents* web_contents);

  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

#endif  // HNS_BROWSER_UI_BOOKMARK_HNS_BOOKMARK_TAB_HELPER_H_
