/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TABS_TAB_MANAGER_OBSERVER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TABS_TAB_MANAGER_OBSERVER_H_

#include <cstdint>
#include <string>
#include <vector>

#include "base/observer_list_types.h"
#include "url/gurl.h"

namespace hns_ads {

struct TabInfo;

class TabManagerObserver : public base::CheckedObserver {
 public:
  // Invoked when the tab specfied by |id| changes focus.
  virtual void OnTabDidChangeFocus(const int32_t tab_id) {}

  // Invoked when the tab specified by |id| is updated.
  virtual void OnTabDidChange(const TabInfo& tab) {}

  // Invoked when a new tab is opened for the specified |id|.
  virtual void OnDidOpenNewTab(const TabInfo& tab) {}

  // Invoked when the text content for the tab specified by |id| did change.
  virtual void OnTextContentDidChange(const int32_t tab_id,
                                      const std::vector<GURL>& redirect_chain,
                                      const std::string& text) {}

  // Invoked when the HTML content for the tab specified by |id| did change.
  virtual void OnHtmlContentDidChange(const int32_t tab_id,
                                      const std::vector<GURL>& redirect_chain,
                                      const std::string& html) {}

  // Invoked when a tab is closed.
  virtual void OnDidCloseTab(const int32_t tab_id) {}

  // Invoked when media starts playing in a tab specified by |id|.
  virtual void OnTabDidStartPlayingMedia(const int32_t tab_id) {}

  // Invoked when media stops playing in a tab specified by |id|.
  virtual void OnTabDidStopPlayingMedia(const int32_t tab_id) {}
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TABS_TAB_MANAGER_OBSERVER_H_
