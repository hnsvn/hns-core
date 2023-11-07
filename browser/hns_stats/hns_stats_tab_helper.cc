/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_stats/hns_stats_tab_helper.h"

#include "hns/browser/hns_browser_process.h"
#include "hns/browser/hns_stats/hns_stats_updater.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/base/page_transition_types.h"

namespace hns_stats {

HnsStatsTabHelper::HnsStatsTabHelper(content::WebContents* web_contents)
    : content::WebContentsObserver(web_contents),
      content::WebContentsUserData<HnsStatsTabHelper>(*web_contents) {}

HnsStatsTabHelper::~HnsStatsTabHelper() = default;

void HnsStatsTabHelper::DidStartNavigation(
        content::NavigationHandle* handle) {
  if (!handle || !handle->IsInMainFrame() || handle->IsDownload())
    return;

  auto transition = handle->GetPageTransition();

  switch (ui::PageTransitionStripQualifier(transition)) {
    case ui::PAGE_TRANSITION_TYPED:
    case ui::PAGE_TRANSITION_AUTO_BOOKMARK:
    case ui::PAGE_TRANSITION_GENERATED: {
      auto url = handle->GetURL();
      if (!(url.SchemeIs("chrome") || url.SchemeIs("hns")))
        NotifyStatsUpdater();
      break;
    }
    default:
      break;
  }
}

void HnsStatsTabHelper::NotifyStatsUpdater() {
  if (g_hns_browser_process->hns_stats_updater()->MaybeDoThresholdPing(1))
    web_contents()->RemoveUserData(UserDataKey());
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(HnsStatsTabHelper);

}  //  namespace hns_stats
