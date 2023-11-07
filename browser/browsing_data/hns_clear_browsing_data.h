/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_BROWSING_DATA_HNS_CLEAR_BROWSING_DATA_H_
#define HNS_BROWSER_BROWSING_DATA_HNS_CLEAR_BROWSING_DATA_H_

#include <cstdint>

namespace content {
class BrowsingDataRemover;
}
class Profile;
class HnsClearDataOnExitTest;

namespace content {

class HnsClearBrowsingData {
 public:
  HnsClearBrowsingData(const HnsClearBrowsingData&) = delete;
  HnsClearBrowsingData& operator=(const HnsClearBrowsingData&) = delete;

  // Clears browsing data for all loaded non-off-the-record profiles.
  // Profile's *OnExit preferences determine what gets cleared.
  // Note: this method will wait until browsing data has been cleared.
  static void ClearOnExit();

  // Used for testing only.
  struct OnExitTestingCallback {
    // Called from ClearOnExit right before the call to BrowsingDataRemover
    // to remove data.
    virtual void BeforeClearOnExitRemoveData(
        content::BrowsingDataRemover* remover,
        uint64_t remove_mask,
        uint64_t origin_mask) = 0;
  };

 protected:
  friend class ::HnsClearDataOnExitTest;

  // Used for testing only.
  static void SetOnExitTestingCallback(OnExitTestingCallback* callback);

 private:
  static OnExitTestingCallback* on_exit_testing_callback_;
};

}  // namespace content

#endif  // HNS_BROWSER_BROWSING_DATA_HNS_CLEAR_BROWSING_DATA_H_
