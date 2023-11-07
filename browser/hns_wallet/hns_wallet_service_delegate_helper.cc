/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <memory>

#include "hns/components/hns_wallet/browser/hns_wallet_service_delegate.h"
#include "build/build_config.h"
#include "content/public/browser/browser_context.h"

#if BUILDFLAG(IS_ANDROID)
#include "hns/browser/hns_wallet/hns_wallet_service_delegate_impl_android.h"
#else
#include "hns/browser/hns_wallet/hns_wallet_service_delegate_impl.h"
#endif

namespace hns_wallet {

// static
std::unique_ptr<HnsWalletServiceDelegate> HnsWalletServiceDelegate::Create(
    content::BrowserContext* browser_context) {
  return std::make_unique<HnsWalletServiceDelegateImpl>(browser_context);
}

}  // namespace hns_wallet
