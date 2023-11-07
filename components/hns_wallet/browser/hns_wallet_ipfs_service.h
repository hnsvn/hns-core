/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_IPFS_SERVICE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_IPFS_SERVICE_H_

#include <string>
#include <vector>

#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

#include "base/memory/raw_ptr.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/prefs/pref_service.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"

namespace hns_wallet {

class HnsWalletIpfsService : public KeyedService, public mojom::IpfsService {
 public:
  explicit HnsWalletIpfsService(PrefService* pref_service);
  ~HnsWalletIpfsService() override;

  HnsWalletIpfsService(const HnsWalletIpfsService&) = delete;
  HnsWalletIpfsService& operator=(const HnsWalletIpfsService&) = delete;

  mojo::PendingRemote<mojom::IpfsService> MakeRemote();
  void Bind(mojo::PendingReceiver<mojom::IpfsService> receiver);

  void TranslateToNFTGatewayURL(
      const std::string& url,
      TranslateToNFTGatewayURLCallback callback) override;
  void TranslateToGatewayURL(const std::string& url,
                             TranslateToGatewayURLCallback callback) override;
  void ExtractIPFSUrlFromGatewayLikeUrl(
      const std::string& url,
      ExtractIPFSUrlFromGatewayLikeUrlCallback callback) override;

  void GetNFTGatewayURL(GetNFTGatewayURLCallback override) override;
  void GetGatewayURL(GetGatewayURLCallback callback) override;

  void SetGateway(const std::string& url, SetGatewayCallback callback) override;
  void SetNFTGateway(const std::string& url,
                     SetNFTGatewayCallback callback) override;

  void ContentHashToCIDv1URL(const std::vector<uint8_t>& content_hash,
                             ContentHashToCIDv1URLCallback callback) override;

 private:
  mojo::ReceiverSet<mojom::IpfsService> receivers_;

  raw_ptr<PrefService> pref_service_;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_IPFS_SERVICE_H_
