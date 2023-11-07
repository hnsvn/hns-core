/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/test/scoped_feature_list.h"
#include "hns/app/hns_command_ids.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/skus/common/features.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/test/test_browser_dialog.h"
#include "content/public/test/browser_test.h"

class HnsVPNPanelControllerTest : public DialogBrowserTest {
 public:
  HnsVPNPanelControllerTest() {
    scoped_feature_list_.InitWithFeatures(
        {skus::features::kSkusFeature, hns_vpn::features::kHnsVPN}, {});
  }

  ~HnsVPNPanelControllerTest() override = default;

  // TestBrowserUi:
  void ShowUi(const std::string& name) override {
    browser()->command_controller()->ExecuteCommand(IDC_SHOW_HNS_VPN_PANEL);
  }

  base::test::ScopedFeatureList scoped_feature_list_;
};

IN_PROC_BROWSER_TEST_F(HnsVPNPanelControllerTest, InvokeUi_Dialog) {
  ShowAndVerifyUi();
}
