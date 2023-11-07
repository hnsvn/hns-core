// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/views/sidebar/sidebar_container_view.h"

#include "hns/browser/ui/hns_browser.h"
#include "hns/browser/ui/sidebar/sidebar_controller.h"
#include "hns/browser/ui/sidebar/sidebar_service_factory.h"
#include "hns/browser/ui/views/frame/hns_browser_view.h"
#include "hns/browser/ui/views/sidebar/sidebar_button_view.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/sidebar/sidebar_item.h"
#include "hns/components/sidebar/sidebar_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/views/frame/toolbar_button_provider.h"
#include "chrome/browser/ui/views/toolbar/side_panel_toolbar_button.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "testing/gtest/include/gtest/gtest.h"

class SidebarContainerViewBrowserTest : public InProcessBrowserTest {
 public:
  SidebarContainerViewBrowserTest() = default;
  SidebarContainerViewBrowserTest(const SidebarContainerViewBrowserTest&) =
      delete;
  SidebarContainerViewBrowserTest& operator=(
      const SidebarContainerViewBrowserTest&) = delete;
  ~SidebarContainerViewBrowserTest() override = default;

  HnsBrowserView* hns_browser_view() {
    return static_cast<HnsBrowserView*>(
        HnsBrowserView::GetBrowserViewForBrowser(browser()));
  }

  sidebar::SidebarService* GetService() {
    return sidebar::SidebarServiceFactory::GetForProfile(browser()->profile());
  }

  SidebarContainerView* sidebar() {
    auto* controller =
        static_cast<HnsBrowser*>(browser())->sidebar_controller();
    return static_cast<SidebarContainerView*>(controller->sidebar());
  }

  SidePanelToolbarButton* toolbar_button() {
    return BrowserView::GetBrowserViewForBrowser(browser())
        ->toolbar_button_provider()
        ->GetSidePanelButton();
  }
};

IN_PROC_BROWSER_TEST_F(SidebarContainerViewBrowserTest,
                       ButtonIsShownByDefault) {
  EXPECT_LT(0u, GetService()->items().size());
  EXPECT_TRUE(sidebar());
  EXPECT_TRUE(toolbar_button());
  EXPECT_TRUE(toolbar_button()->GetVisible());
}

IN_PROC_BROWSER_TEST_F(SidebarContainerViewBrowserTest, ButtonIsHiddenByPref) {
  EXPECT_TRUE(toolbar_button()->GetVisible());

  // When the pref is false, the button should be hidden.
  browser()->profile()->GetPrefs()->SetBoolean(kShowSidePanelButton, false);
  EXPECT_FALSE(toolbar_button()->GetVisible());

  // Reenabling it should show the button again.
  browser()->profile()->GetPrefs()->SetBoolean(kShowSidePanelButton, true);
  EXPECT_TRUE(toolbar_button()->GetVisible());
}

IN_PROC_BROWSER_TEST_F(SidebarContainerViewBrowserTest,
                       ButtonIsHiddenWithoutPanelItems) {
  EXPECT_TRUE(toolbar_button()->GetVisible());

  // Removing all items should make the button hide.
  auto items_count = GetService()->items().size();
  while (items_count > 0) {
    GetService()->RemoveItemAt(--items_count);
  }
  EXPECT_FALSE(toolbar_button()->GetVisible());

  // Adding a new default item should cause the button to become visible again.
  GetService()->AddItem(sidebar::SidebarItem::Create(
      u"Test", sidebar::SidebarItem::Type::kTypeBuiltIn,
      sidebar::SidebarItem::BuiltInItemType::kReadingList, true));
  EXPECT_EQ(1u, GetService()->items().size());
  EXPECT_TRUE(toolbar_button()->GetVisible());
}
