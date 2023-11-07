/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_ACCOUNT_RESOLVER_DELEGATE_IMPL_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_ACCOUNT_RESOLVER_DELEGATE_IMPL_H_

#include <string>

#include "hns/components/hns_wallet/browser/account_resolver_delegate.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"

namespace hns_wallet {

class AccountResolverDelegateImpl : public AccountResolverDelegate {
 public:
  explicit AccountResolverDelegateImpl(KeyringService* keyring_service);

  mojom::AccountIdPtr ResolveAccountId(
      const std::string* from_account_id,
      const std::string* from_address) override;
  bool ValidateAccountId(const mojom::AccountIdPtr& account_id) override;

 private:
  raw_ptr<KeyringService> keyring_service_ = nullptr;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_ACCOUNT_RESOLVER_DELEGATE_IMPL_H_
