/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IMPL_HELPER_H_
#define HNS_BROWSER_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IMPL_HELPER_H_

#include "base/functional/callback.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom-forward.h"
#include "build/build_config.h"

namespace content {
class WebContents;
}

// UI helper functions which are platform-dependent.
namespace hns_wallet {

// Show wallet panel, which handles permission UI, sign message confirmation,
// ...etc.
void ShowPanel(content::WebContents* web_contents);

// Show wallet onboarding page.
void ShowWalletOnboarding(content::WebContents* web_contents);

// Show account creation page for keyring id
void ShowAccountCreation(content::WebContents* web_contents,
                         hns_wallet::mojom::CoinType coin_type);

// Triggers when any kind interaction from a DApp is detected
void WalletInteractionDetected(content::WebContents* web_contents);

// Check are web3 notifications allowed or not. Used on Android to
// show or not a permissions prompt dialog
bool IsWeb3NotificationAllowed();

void SetCallbackForNewSetupNeededForTesting(base::OnceCallback<void()>);

void SetCallbackForAccountCreationForTesting(base::OnceCallback<void()>);

}  // namespace hns_wallet

#endif  // HNS_BROWSER_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IMPL_HELPER_H_
