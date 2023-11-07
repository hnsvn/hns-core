// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_HNS_WALLET_PAGE_HANDLER_WALLET_PAGE_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_HNS_WALLET_PAGE_HANDLER_WALLET_PAGE_HANDLER_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"

class Profile;

class WalletPageHandler : public hns_wallet::mojom::PageHandler {
 public:
  WalletPageHandler(
      mojo::PendingReceiver<hns_wallet::mojom::PageHandler> receiver,
      Profile* profile);

  WalletPageHandler(const WalletPageHandler&) = delete;
  WalletPageHandler& operator=(const WalletPageHandler&) = delete;
  ~WalletPageHandler() override;

  void ShowApprovePanelUI() override;

 private:
  raw_ptr<Profile> profile_ = nullptr;  // NOT OWNED
  mojo::Receiver<hns_wallet::mojom::PageHandler> receiver_;
  base::WeakPtrFactory<WalletPageHandler> weak_ptr_factory_;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_WALLET_PAGE_HANDLER_WALLET_PAGE_HANDLER_H_
