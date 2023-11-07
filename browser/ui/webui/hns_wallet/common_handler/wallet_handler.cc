// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/hns_wallet/common_handler/wallet_handler.h"

#include <string>
#include <utility>
#include <vector>

#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "chrome/browser/profiles/profile.h"

namespace hns_wallet {

WalletHandler::WalletHandler(
    mojo::PendingReceiver<mojom::WalletHandler> receiver,
    Profile* profile)
    : receiver_(this, std::move(receiver)),
      keyring_service_(KeyringServiceFactory::GetServiceForContext(profile)) {}

WalletHandler::~WalletHandler() = default;

// TODO(apaymyshev): this is the only method in WalletHandler. Should be merged
// into HnsWalletService.
void WalletHandler::GetWalletInfo(GetWalletInfoCallback callback) {
  if (!keyring_service_) {
    std::move(callback).Run(nullptr);
    return;
  }

  auto default_keyring =
      keyring_service_->GetKeyringInfoSync(mojom::kDefaultKeyringId);
  DCHECK_EQ(default_keyring->id, mojom::kDefaultKeyringId);
  std::move(callback).Run(mojom::WalletInfo::New(
      default_keyring->is_keyring_created, default_keyring->is_locked,
      default_keyring->is_backed_up, IsFilecoinEnabled(), IsSolanaEnabled(),
      IsBitcoinEnabled(), IsNftPinningEnabled(), IsPanelV2Enabled()));
}

}  // namespace hns_wallet
