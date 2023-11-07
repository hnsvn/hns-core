// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/views/tabs/hns_tab.h"
#include "chrome/browser/ui/views/tabs/fake_tab_slot_controller.h"
#include "chrome/test/views/chrome_views_test_base.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/views/test/views_test_utils.h"

class HnsTabTest : public ChromeViewsTestBase {
 public:
  HnsTabTest() = default;
  ~HnsTabTest() override = default;

  void LayoutAndCheckBorder(HnsTab* tab,
                            const gfx::Rect& bounds,
                            bool gave_extra_padding) {
    tab->SetBoundsRect(bounds);
    views::test::RunScheduledLayout(tab);

    auto insets = tab->tab_style_views()->GetContentsInsets();
    int left_inset = insets.left();
    if (gave_extra_padding) {
      left_inset += HnsTab::kExtraLeftPadding;
    }
    EXPECT_EQ(left_inset, tab->GetInsets().left());
  }
};

TEST_F(HnsTabTest, ExtraPaddingLayoutTest) {
  FakeTabSlotController tab_slot_controller;
  HnsTab tab(&tab_slot_controller);

  // Smaller width tab will be given extra padding.
  LayoutAndCheckBorder(&tab, {0, 0, 30, 50}, true);
  LayoutAndCheckBorder(&tab, {0, 0, 50, 50}, true);
  LayoutAndCheckBorder(&tab, {0, 0, 100, 50}, false);
  LayoutAndCheckBorder(&tab, {0, 0, 150, 50}, false);
  LayoutAndCheckBorder(&tab, {0, 0, 30, 50}, true);
}
