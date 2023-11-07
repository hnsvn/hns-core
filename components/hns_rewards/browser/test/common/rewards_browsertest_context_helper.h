/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_CONTEXT_HELPER_H_
#define HNS_COMPONENTS_HNS_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_CONTEXT_HELPER_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_rewards/browser/test/common/rewards_browsertest_util.h"
#include "build/build_config.h"
#include "chrome/browser/ui/browser.h"
#include "url/gurl.h"

namespace content {
class WebContents;
}  // namespace content

namespace hns_rewards::test_util {

class RewardsBrowserTestContextHelper {
 public:
  explicit RewardsBrowserTestContextHelper(Browser* browser);
  ~RewardsBrowserTestContextHelper();

  base::WeakPtr<content::WebContents> OpenRewardsPopup();

  base::WeakPtr<content::WebContents> OpenSiteBanner();

  // Visit publisher and verify that the auto-contribution panel on
  // hns://rewards looks correct
  void VisitPublisher(const GURL& url, bool verified);

  void LoadURL(GURL url);

  void LoadRewardsPage();

  void ReloadCurrentSite();

 private:
  void OpenPopup();

  raw_ptr<Browser> browser_ = nullptr;  // NOT OWNED
  base::WeakPtr<content::WebContents> popup_contents_;
};

}  // namespace hns_rewards::test_util

#endif  // HNS_COMPONENTS_HNS_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_CONTEXT_HELPER_H_
