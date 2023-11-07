/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_WALLET_WALLET_NOTIFICATION_HELPER_H_
#define HNS_BROWSER_HNS_WALLET_WALLET_NOTIFICATION_HELPER_H_

namespace content {
class BrowserContext;
}  // namespace content

namespace hns_wallet {

class TxService;

void RegisterWalletNotificationService(content::BrowserContext* context,
                                       TxService* service);

}  // namespace hns_wallet

#endif  // HNS_BROWSER_HNS_WALLET_WALLET_NOTIFICATION_HELPER_H_
