/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_pages.h"

#include "base/strings/strcat.h"
#include "hns/browser/ui/webui/webcompat_reporter/webcompat_reporter_dialog.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/sidebar/constants.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_navigator_params.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/browser/ui/singleton_tabs.h"
#include "chrome/common/webui_url_constants.h"
#include "url/gurl.h"

namespace hns {

void ShowHnsRewards(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kHnsUIRewardsURL));
}

void ShowHnsAdblock(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kHnsUIAdblockURL));
}

void ShowSync(Browser* browser) {
  ShowSingletonTabOverwritingNTP(
      browser, chrome::GetSettingsUrl(chrome::kSyncSetupSubPage));
}

void ShowHnsNewsConfigure(Browser* browser) {
  ShowSingletonTabOverwritingNTP(
      browser, GURL("hns://newtab/?openSettings=HnsNews"));
}

void ShowShortcutsPage(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kShortcutsURL));
}

void ShowHnsTalk(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(sidebar::kHnsTalkURL));
}

void ShowWebcompatReporter(Browser* browser) {
  content::WebContents* web_contents =
      browser->tab_strip_model()->GetActiveWebContents();
  if (!web_contents) {
    return;
  }

  webcompat_reporter::OpenReporterDialog(web_contents);
}

void ShowHnsWallet(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kHnsUIWalletURL));
}

void ShowHnsWalletOnboarding(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kHnsUIWalletOnboardingURL));
}

void ShowHnsWalletAccountCreation(Browser* browser,
                                    hns_wallet::mojom::CoinType coin_type) {
  // Only solana is supported.
  if (coin_type == hns_wallet::mojom::CoinType::SOL) {
    ShowSingletonTabOverwritingNTP(
        browser,
        GURL(base::StrCat({kHnsUIWalletAccountCreationURL, "Solana"})));
  } else {
    NOTREACHED();
  }
}

void ShowExtensionSettings(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kExtensionSettingsURL));
}

void ShowWalletSettings(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kWalletSettingsURL));
}

void ShowIPFS(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kIPFSWebUIURL));
}

}  // namespace hns
