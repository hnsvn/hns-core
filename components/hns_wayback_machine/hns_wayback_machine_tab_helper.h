/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WAYBACK_MACHINE_HNS_WAYBACK_MACHINE_TAB_HELPER_H_
#define HNS_COMPONENTS_HNS_WAYBACK_MACHINE_HNS_WAYBACK_MACHINE_TAB_HELPER_H_

#include <memory>

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ref.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class HnsWaybackMachineDelegate;
class PrefService;

class HnsWaybackMachineTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<HnsWaybackMachineTabHelper> {
 public:
  explicit HnsWaybackMachineTabHelper(content::WebContents* contents);
  ~HnsWaybackMachineTabHelper() override;

  HnsWaybackMachineTabHelper(const HnsWaybackMachineTabHelper&) = delete;
  HnsWaybackMachineTabHelper& operator=(
      const HnsWaybackMachineTabHelper&) = delete;

  void set_delegate(std::unique_ptr<HnsWaybackMachineDelegate> delegate);

  WEB_CONTENTS_USER_DATA_KEY_DECL();
 private:
  FRIEND_TEST_ALL_PREFIXES(HnsWaybackMachineTest, InfobarAddTest);

  // content::WebContentsObserver overrides:
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;

  void CreateInfoBar();
  bool IsWaybackMachineEnabled() const;

  // virtual for test.
  virtual bool ShouldAttachWaybackMachineInfoBar(int response_code) const;

  raw_ref<PrefService> pref_service_;
  std::unique_ptr<HnsWaybackMachineDelegate> delegate_;

  base::WeakPtrFactory<HnsWaybackMachineTabHelper> weak_factory_;
};

#endif  // HNS_COMPONENTS_HNS_WAYBACK_MACHINE_HNS_WAYBACK_MACHINE_TAB_HELPER_H_
