/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_WALLET_WALLET_COMMON_UI_H_
#define HNS_BROWSER_UI_WEBUI_HNS_WALLET_WALLET_COMMON_UI_H_

#include <stdint.h>

class Profile;
class Browser;

namespace url {
class Origin;
}  // namespace url

namespace content {
class WebContents;
}  // namespace content

namespace hns_wallet {

void AddBlockchainTokenImageSource(Profile* profile);

bool IsHnsWalletOrigin(const url::Origin& origin);

content::WebContents* GetWebContentsFromTabId(Browser** browser,
                                              int32_t tab_id);
content::WebContents* GetActiveWebContents();

}  // namespace hns_wallet

#endif  // HNS_BROWSER_UI_WEBUI_HNS_WALLET_WALLET_COMMON_UI_H_
