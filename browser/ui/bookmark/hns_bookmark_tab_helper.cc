/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/bookmark/hns_bookmark_tab_helper.h"

#include "hns/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search/search.h"
#include "chrome/browser/ui/webui/new_tab_page/new_tab_page_ui.h"
#include "chrome/browser/ui/webui/ntp/new_tab_ui.h"
#include "components/bookmarks/common/bookmark_pref_names.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/web_contents.h"

namespace {

bool IsNTP(content::WebContents* web_contents) {
  // Use the committed entry so the bookmarks bar disappears at the same time
  // the page does.
  content::NavigationEntry* entry =
      web_contents->GetController().GetLastCommittedEntry();
  if (!entry)
    entry = web_contents->GetController().GetVisibleEntry();
  if (!entry)
    return false;
  const GURL& url = entry->GetURL();
  return NewTabUI::IsNewTab(url) || NewTabPageUI::IsNewTabPageOrigin(url) ||
         search::NavEntryIsInstantNTP(web_contents, entry);
}

}  // namespace

HnsBookmarkTabHelper::HnsBookmarkTabHelper(
    content::WebContents* web_contents)
    : content::WebContentsUserData<HnsBookmarkTabHelper>(*web_contents) {}

HnsBookmarkTabHelper::~HnsBookmarkTabHelper() = default;

void HnsBookmarkTabHelper::AddObserver(BookmarkTabHelperObserver* observer) {
  BookmarkTabHelper::FromWebContents(&GetWebContents())->AddObserver(observer);
}

void HnsBookmarkTabHelper::RemoveObserver(
    BookmarkTabHelperObserver* observer) {
  BookmarkTabHelper::FromWebContents(&GetWebContents())
      ->RemoveObserver(observer);
}

bool HnsBookmarkTabHelper::ShouldShowBookmarkBar() {
  BookmarkTabHelper* helper =
      BookmarkTabHelper::FromWebContents(&GetWebContents());
  if (!helper)
    return false;

  if (IsNTP(&GetWebContents())) {
    Profile* profile =
        Profile::FromBrowserContext(GetWebContents().GetBrowserContext());

    if (profile->IsGuestSession())
      return false;

    PrefService* prefs = profile->GetPrefs();
    if (prefs->IsManagedPreference(bookmarks::prefs::kShowBookmarkBar) &&
        !prefs->GetBoolean(bookmarks::prefs::kShowBookmarkBar))
      return false;

    return prefs->GetBoolean(kAlwaysShowBookmarkBarOnNTP);
  }

  return helper->ShouldShowBookmarkBar();
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(HnsBookmarkTabHelper);
