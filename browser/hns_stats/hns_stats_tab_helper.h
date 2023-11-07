/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_STATS_HNS_STATS_TAB_HELPER_H_
#define HNS_BROWSER_HNS_STATS_HNS_STATS_TAB_HELPER_H_

#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class NavigationHandle;
class WebContents;
}  // namespace content

namespace hns_stats {

class HnsStatsUpdater;

class HnsStatsTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<HnsStatsTabHelper> {
 public:
  explicit HnsStatsTabHelper(content::WebContents*);
  ~HnsStatsTabHelper() override;
  HnsStatsTabHelper(const HnsStatsTabHelper&) = delete;
  HnsStatsTabHelper& operator=(const HnsStatsTabHelper&) = delete;

  void NotifyStatsUpdater();

 private:
  void DidStartNavigation(content::NavigationHandle*) override;

  friend class content::WebContentsUserData<HnsStatsTabHelper>;
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace hns_stats
#endif  // HNS_BROWSER_HNS_STATS_HNS_STATS_TAB_HELPER_H_
