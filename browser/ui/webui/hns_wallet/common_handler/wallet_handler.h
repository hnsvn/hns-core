// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_HNS_WALLET_COMMON_HANDLER_WALLET_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_HNS_WALLET_COMMON_HANDLER_WALLET_HANDLER_H_

#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

class Profile;

namespace hns_wallet {

class KeyringService;

class WalletHandler : public mojom::WalletHandler {
 public:
  WalletHandler(mojo::PendingReceiver<mojom::WalletHandler> receiver,
                Profile* profile);

  WalletHandler(const WalletHandler&) = delete;
  WalletHandler& operator=(const WalletHandler&) = delete;
  ~WalletHandler() override;

  // mojom::WalletHandler:
  void GetWalletInfo(GetWalletInfoCallback) override;

 private:
  void OnGetWalletInfo(GetWalletInfoCallback callback,
                       std::vector<mojom::KeyringInfoPtr> keyring_info);

  mojo::Receiver<mojom::WalletHandler> receiver_;

  const raw_ptr<KeyringService> keyring_service_ = nullptr;
};

}  // namespace hns_wallet

#endif  // HNS_BROWSER_UI_WEBUI_HNS_WALLET_COMMON_HANDLER_WALLET_HANDLER_H_
