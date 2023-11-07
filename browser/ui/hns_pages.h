/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_PAGES_H_
#define HNS_BROWSER_UI_HNS_PAGES_H_

#include "hns/components/hns_wallet/common/hns_wallet.mojom-forward.h"

class Browser;

namespace hns {

void ShowHnsAdblock(Browser* browser);
void ShowWebcompatReporter(Browser* browser);
void ShowHnsRewards(Browser* browser);
void ShowHnsWallet(Browser* browser);
void ShowHnsWalletOnboarding(Browser* browser);
void ShowHnsWalletAccountCreation(Browser* browser,
                                    hns_wallet::mojom::CoinType coin_type);
void ShowExtensionSettings(Browser* browser);
void ShowWalletSettings(Browser* browser);
void ShowSync(Browser* browser);
void ShowHnsNewsConfigure(Browser* browser);
void ShowShortcutsPage(Browser* browser);
void ShowHnsTalk(Browser* browser);

}  // namespace hns

#endif  // HNS_BROWSER_UI_HNS_PAGES_H_
