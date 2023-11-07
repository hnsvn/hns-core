/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/toolbar/hns_vpn_menu_model.h"

#include <memory>

#include "base/test/scoped_feature_list.h"
#include "hns/app/hns_command_ids.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "components/prefs/pref_service.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/simple_menu_model.h"

class HnsVPNMenuModelUnitTest : public testing::Test {
 public:
  HnsVPNMenuModelUnitTest() = default;
  ~HnsVPNMenuModelUnitTest() override = default;

  PrefService* prefs() { return &prefs_; }

  void SetUp() override {
    local_state_ = std::make_unique<ScopedTestingLocalState>(
        TestingBrowserProcess::GetGlobal());
    hns_vpn::RegisterProfilePrefs(prefs_.registry());
  }

  TestingPrefServiceSimple* local_state() { return local_state_->Get(); }

 private:
  sync_preferences::TestingPrefServiceSyncable prefs_;
  std::unique_ptr<ScopedTestingLocalState> local_state_;
};

#if BUILDFLAG(IS_WIN)
TEST_F(HnsVPNMenuModelUnitTest, TrayIconEnabled) {
  local_state()->SetBoolean(hns_vpn::prefs::kHnsVPNWireguardEnabled, true);

  HnsVPNMenuModel menu_model(nullptr, prefs());

  // Cases with Enabled value.
  menu_model.SetTrayIconEnabledForTesting(true);
  prefs()->SetBoolean(hns_vpn::prefs::kHnsVPNShowButton, true);
  EXPECT_TRUE(menu_model.IsTrayIconEnabled());
  menu_model.Clear();
  EXPECT_EQ(menu_model.GetItemCount(), 0u);
  menu_model.Build();
  EXPECT_NE(menu_model.GetItemCount(), 0u);
  {
    // Don't show toggle menu when tray icon is visible.
    EXPECT_FALSE(
        menu_model.GetIndexOfCommandId(IDC_TOGGLE_HNS_VPN_TRAY_ICON));
  }

  // Wireguard protocol disbled in the setting.
  EXPECT_TRUE(menu_model.IsTrayIconEnabled());
  menu_model.Clear();
  EXPECT_EQ(menu_model.GetItemCount(), 0u);
  menu_model.Build();
  EXPECT_NE(menu_model.GetItemCount(), 0u);
  {
    // Still toggle menu is hidden.
    EXPECT_FALSE(
        menu_model.GetIndexOfCommandId(IDC_TOGGLE_HNS_VPN_TRAY_ICON));
  }

  // Cases with Disabled value.
  menu_model.SetTrayIconEnabledForTesting(false);
  prefs()->SetBoolean(hns_vpn::prefs::kHnsVPNShowButton, false);
  EXPECT_FALSE(menu_model.IsTrayIconEnabled());
  menu_model.Clear();
  EXPECT_EQ(menu_model.GetItemCount(), 0u);
  menu_model.Build();
  EXPECT_NE(menu_model.GetItemCount(), 0u);
  {
    auto tray_index =
        menu_model.GetIndexOfCommandId(IDC_TOGGLE_HNS_VPN_TRAY_ICON);
    EXPECT_TRUE(tray_index);
    EXPECT_EQ(
        menu_model.GetLabelAt(tray_index.value()),
        l10n_util::GetStringUTF16(IDS_HNS_VPN_SHOW_VPN_TRAY_ICON_MENU_ITEM));
  }
}
#endif  // BUILDFLAG(IS_WIN)

TEST_F(HnsVPNMenuModelUnitTest, ToolbarVPNButton) {
  HnsVPNMenuModel menu_model(nullptr, prefs());

  // Cases with Enabled value.
  prefs()->SetBoolean(hns_vpn::prefs::kHnsVPNShowButton, true);

  EXPECT_TRUE(menu_model.IsHnsVPNButtonVisible());
  menu_model.Clear();
  EXPECT_EQ(menu_model.GetItemCount(), 0u);
  menu_model.Build();
  EXPECT_NE(menu_model.GetItemCount(), 0u);
  {
    // Don't show toggle menu when button is visible.
    EXPECT_FALSE(
        menu_model.GetIndexOfCommandId(IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON));
  }

  // Cases with Disabled value.
  prefs()->SetBoolean(hns_vpn::prefs::kHnsVPNShowButton, false);
  EXPECT_FALSE(menu_model.IsHnsVPNButtonVisible());
  menu_model.Clear();
  EXPECT_EQ(menu_model.GetItemCount(), 0u);
  menu_model.Build();
  EXPECT_NE(menu_model.GetItemCount(), 0u);
  {
    auto toolbar_index =
        menu_model.GetIndexOfCommandId(IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON);
    EXPECT_TRUE(toolbar_index);
    EXPECT_EQ(
        menu_model.GetLabelAt(toolbar_index.value()),
        l10n_util::GetStringUTF16(IDS_HNS_VPN_SHOW_VPN_BUTTON_MENU_ITEM));
  }
}
