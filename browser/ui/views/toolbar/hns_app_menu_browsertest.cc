/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/test/scoped_feature_list.h"
#include "hns/app/hns_command_ids.h"
#include "hns/browser/ui/hns_browser_command_controller.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/skus/common/features.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/toolbar/app_menu.h"
#include "chrome/browser/ui/views/toolbar/browser_app_menu_button.h"
#include "chrome/browser/ui/views/toolbar/toolbar_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/test_utils.h"
#include "ui/views/controls/button/toggle_button.h"
#include "ui/views/controls/menu/menu_item_view.h"
#include "ui/views/controls/menu/menu_runner.h"
#include "ui/views/view_utils.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/hns_vpn/common/features.h"
#endif

class HnsAppMenuBrowserTest : public InProcessBrowserTest {
 public:
  HnsAppMenuBrowserTest() {
#if BUILDFLAG(ENABLE_HNS_VPN)
    scoped_feature_list_.InitWithFeatures(
        {skus::features::kSkusFeature, hns_vpn::features::kHnsVPN}, {});
#endif
  }

  ~HnsAppMenuBrowserTest() override = default;

  BrowserAppMenuButton* menu_button() {
    return BrowserView::GetBrowserViewForBrowser(browser())
        ->toolbar()
        ->app_menu_button();
  }

#if BUILDFLAG(ENABLE_HNS_VPN)
  void SetPurchasedUserForHnsVPN(Browser* browser, bool purchased) {
    auto* service =
        hns_vpn::HnsVpnServiceFactory::GetForProfile(browser->profile());
    ASSERT_TRUE(!!service);
    auto target_state = purchased
                            ? hns_vpn::mojom::PurchasedState::PURCHASED
                            : hns_vpn::mojom::PurchasedState::NOT_PURCHASED;
    service->SetPurchasedState(skus::GetDefaultEnvironment(), target_state);
    // Call explicitely to update vpn commands status because mojo works in
    // async way.
    static_cast<chrome::HnsBrowserCommandController*>(
        browser->command_controller())
        ->OnPurchasedStateChanged(target_state, absl::nullopt);
  }

  base::test::ScopedFeatureList scoped_feature_list_;
#endif
};

#if BUILDFLAG(ENABLE_HNS_VPN)
// Check toggle menu item has additional toggle button for purchased user.
IN_PROC_BROWSER_TEST_F(HnsAppMenuBrowserTest, PurchasedVPN) {
  SetPurchasedUserForHnsVPN(browser(), true);
  menu_button()->ShowMenu(views::MenuRunner::NO_FLAGS);
  views::MenuItemView* menu_root = menu_button()->app_menu()->root_menu_item();
  auto* toggle_menu_item = menu_root->GetMenuItemByID(IDC_TOGGLE_HNS_VPN);
  ASSERT_TRUE(!!toggle_menu_item);
  const int last_item_index = toggle_menu_item->children().size() - 1;
  auto* toggle_button = views::AsViewClass<views::ToggleButton>(
      toggle_menu_item->children()[last_item_index]);
  ASSERT_NE(nullptr, toggle_button);
}

// Check app menu has show vpn panel menu item for not purchased user.
IN_PROC_BROWSER_TEST_F(HnsAppMenuBrowserTest, NotPurchasedVPN) {
  SetPurchasedUserForHnsVPN(browser(), false);
  menu_button()->ShowMenu(views::MenuRunner::NO_FLAGS);
  views::MenuItemView* menu_root = menu_button()->app_menu()->root_menu_item();
  EXPECT_TRUE(!!menu_root->GetMenuItemByID(IDC_SHOW_HNS_VPN_PANEL));
}
#endif
