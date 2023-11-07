/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_ETHEREUM_REMOTE_CLIENT_ETHEREUM_REMOTE_CLIENT_DELEGATE_IMPL_H_
#define HNS_BROWSER_ETHEREUM_REMOTE_CLIENT_ETHEREUM_REMOTE_CLIENT_DELEGATE_IMPL_H_

#include "hns/browser/ethereum_remote_client/ethereum_remote_client_delegate.h"

class EthereumRemoteClientDelegateImpl : public EthereumRemoteClientDelegate {
 public:
  ~EthereumRemoteClientDelegateImpl() override;
  void MaybeLoadCryptoWalletsExtension(
      content::BrowserContext* context) override;
  void UnloadCryptoWalletsExtension(content::BrowserContext* context) override;
};

#endif  // HNS_BROWSER_ETHEREUM_REMOTE_CLIENT_ETHEREUM_REMOTE_CLIENT_DELEGATE_IMPL_H_
