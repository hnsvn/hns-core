/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_tab_strip_model_delegate.h"

#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/tabs/shared_pinned_tab_service.h"
#include "hns/browser/ui/tabs/shared_pinned_tab_service_factory.h"
#include "chrome/browser/ui/browser.h"

namespace chrome {

bool HnsTabStripModelDelegate::CanMoveTabsToWindow(
    const std::vector<int>& indices) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsSharedPinnedTabs)) {
    return BrowserTabStripModelDelegate::CanMoveTabsToWindow(indices);
  }

  // Shared pinned tabs shouldn't be moved.
  return base::ranges::none_of(indices, [this](const auto& index) {
    return browser_->tab_strip_model()->IsTabPinned(index);
  });
}

void HnsTabStripModelDelegate::CacheWebContents(
    const std::vector<std::unique_ptr<TabStripModel::DetachedWebContents>>&
        web_contents) {
  BrowserTabStripModelDelegate::CacheWebContents(web_contents);
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsSharedPinnedTabs)) {
    return;
  }

  auto* shared_pinned_tab_service =
      SharedPinnedTabServiceFactory::GetForProfile(browser_->profile());
  DCHECK(shared_pinned_tab_service);
  shared_pinned_tab_service->CacheWebContentsIfNeeded(browser_, web_contents);
}

}  // namespace chrome
