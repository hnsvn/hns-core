/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_WALLET_HNS_WALLET_CONTEXT_UTILS_H_
#define HNS_BROWSER_HNS_WALLET_HNS_WALLET_CONTEXT_UTILS_H_

namespace content {
class BrowserContext;
}

namespace hns_wallet {

bool IsAllowedForContext(content::BrowserContext* context);

}  // namespace hns_wallet

#endif  // HNS_BROWSER_HNS_WALLET_HNS_WALLET_CONTEXT_UTILS_H_
