/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/hns_wallet_provider_delegate_impl_helper.h"

#include <utility>

#include "hns/browser/hns_wallet/hns_wallet_tab_helper.h"
#include "hns/browser/ui/hns_pages.h"
#include "chrome/browser/ui/browser_finder.h"
#include "content/public/browser/web_contents.h"

namespace {

base::OnceCallback<void()> g_new_setup_needed_callback_for_testing;
base::OnceCallback<void()> g_account_creation_callback_for_testing;

}  // namespace

namespace hns_wallet {

void ShowPanel(content::WebContents* web_contents) {
  if (!web_contents) {
    return;
  }

  auto* tab_helper =
      hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents);
  if (tab_helper) {
    tab_helper->ShowBubble();
  }
}

void ShowWalletOnboarding(content::WebContents* web_contents) {
  Browser* browser =
      web_contents ? chrome::FindBrowserWithWebContents(web_contents) : nullptr;

  if (browser) {
    hns::ShowHnsWalletOnboarding(browser);
  } else if (g_new_setup_needed_callback_for_testing) {
    std::move(g_new_setup_needed_callback_for_testing).Run();
  }
}

void ShowAccountCreation(content::WebContents* web_contents,
                         hns_wallet::mojom::CoinType coin_type) {
  Browser* browser =
      web_contents ? chrome::FindBrowserWithWebContents(web_contents) : nullptr;

  if (browser) {
    hns::ShowHnsWalletAccountCreation(browser, coin_type);
  } else if (g_account_creation_callback_for_testing) {
    std::move(g_account_creation_callback_for_testing).Run();
  }
}

void WalletInteractionDetected(content::WebContents* web_contents) {}

// Desktop uses a panel to show all notifications instead of a dialog
// on Android for permissions
bool IsWeb3NotificationAllowed() {
  return true;
}

void SetCallbackForNewSetupNeededForTesting(
    base::OnceCallback<void()> callback) {
  g_new_setup_needed_callback_for_testing = std::move(callback);
}

void SetCallbackForAccountCreationForTesting(
    base::OnceCallback<void()> callback) {
  g_account_creation_callback_for_testing = std::move(callback);
}

}  // namespace hns_wallet
