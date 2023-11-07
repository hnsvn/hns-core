// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/views/toolbar/wallet_button.h"

#include "base/feature_list.h"
#include "base/run_loop.h"
#include "base/test/scoped_feature_list.h"
#include "hns/browser/ui/views/frame/hns_browser_view.h"
#include "hns/components/hns_wallet/common/features.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/test/test_browser_dialog.h"
#include "chrome/browser/ui/views/bubble/webui_bubble_manager.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "content/public/test/browser_test.h"
#include "ui/views/test/button_test_api.h"

namespace {
ui::MouseEvent GetDummyEvent() {
  return ui::MouseEvent(ui::ET_MOUSE_PRESSED, gfx::PointF(), gfx::PointF(),
                        base::TimeTicks::Now(), 0, 0);
}
}  // namespace

namespace hns_wallet {

class WalletButtonButtonBrowserTest : public InProcessBrowserTest {
 public:
  // InProcessBrowserTest:
  void SetUp() override {
    scoped_feature_list_.InitAndEnableFeature(
        features::kNativeHnsWalletFeature);
    InProcessBrowserTest::SetUp();
  }

  BrowserView* browser_view() {
    return BrowserView::GetBrowserViewForBrowser(browser());
  }

  WalletButton* wallet_button() {
    return static_cast<HnsBrowserView*>(browser_view())->GetWalletButton();
  }

 private:
  base::test::ScopedFeatureList scoped_feature_list_;
};

IN_PROC_BROWSER_TEST_F(WalletButtonButtonBrowserTest,
                       ButtonClickCreatesBubble) {
  ASSERT_FALSE(wallet_button()->IsShowingBubble());
  views::test::ButtonTestApi(wallet_button()).NotifyClick(GetDummyEvent());
  ASSERT_TRUE(wallet_button()->IsShowingBubble());

  wallet_button()->CloseWalletBubble();
  ASSERT_TRUE(wallet_button()->IsBubbleClosedForTesting());
}

class WalletButtonBrowserUITest : public DialogBrowserTest {
 public:
  // DialogBrowserTest:
  void SetUp() override {
    feature_list_.InitAndEnableFeature(features::kNativeHnsWalletFeature);
    DialogBrowserTest::SetUp();
  }
  void ShowUi(const std::string& name) override {
    auto* wallet_button = static_cast<HnsBrowserView*>(
                              BrowserView::GetBrowserViewForBrowser(browser()))
                              ->GetWalletButton();
    views::test::ButtonTestApi(wallet_button).NotifyClick(GetDummyEvent());
  }

 private:
  base::test::ScopedFeatureList feature_list_;
};

// Invokes a wallet panel bubble.
IN_PROC_BROWSER_TEST_F(WalletButtonBrowserUITest, InvokeUi_default) {
  ShowAndVerifyUi();
}

}  // namespace hns_wallet
