// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/hns_wallet/browser/hns_wallet_ipfs_service.h"

#include <string>
#include <utility>
#include <vector>

#include "hns/components/ipfs/ipfs_utils.h"

namespace hns_wallet {

HnsWalletIpfsService::HnsWalletIpfsService(PrefService* pref_service)
    : pref_service_(pref_service) {}

HnsWalletIpfsService::~HnsWalletIpfsService() = default;

mojo::PendingRemote<mojom::IpfsService> HnsWalletIpfsService::MakeRemote() {
  mojo::PendingRemote<mojom::IpfsService> remote;
  receivers_.Add(this, remote.InitWithNewPipeAndPassReceiver());
  return remote;
}

void HnsWalletIpfsService::Bind(
    mojo::PendingReceiver<mojom::IpfsService> receiver) {
  receivers_.Add(this, std::move(receiver));
}

void HnsWalletIpfsService::TranslateToNFTGatewayURL(
    const std::string& url,
    TranslateToNFTGatewayURLCallback callback) {
  GURL new_url;
  if (ipfs::TranslateIPFSURI(GURL(url), &new_url,
                             ipfs::GetDefaultNFTIPFSGateway(pref_service_),
                             false)) {
    std::move(callback).Run(new_url.spec());
  } else {
    std::move(callback).Run(absl::nullopt);
  }
}

void HnsWalletIpfsService::TranslateToGatewayURL(
    const std::string& url,
    TranslateToGatewayURLCallback callback) {
  GURL new_url;
  if (ipfs::TranslateIPFSURI(GURL(url), &new_url,
                             ipfs::GetDefaultIPFSGateway(pref_service_),
                             false)) {
    std::move(callback).Run(new_url.spec());
  } else {
    std::move(callback).Run(absl::nullopt);
  }
}

void HnsWalletIpfsService::ExtractIPFSUrlFromGatewayLikeUrl(
    const std::string& url,
    ExtractIPFSUrlFromGatewayLikeUrlCallback callback) {
  auto result = ipfs::ExtractSourceFromGateway(GURL(url));
  if (result.has_value()) {
    std::move(callback).Run(result.value().spec());
  } else {
    std::move(callback).Run(absl::nullopt);
  }
}

void HnsWalletIpfsService::GetNFTGatewayURL(
    GetNFTGatewayURLCallback callback) {
  std::move(callback).Run(ipfs::GetDefaultNFTIPFSGateway(pref_service_).spec());
}

void HnsWalletIpfsService::GetGatewayURL(GetGatewayURLCallback callback) {
  std::move(callback).Run(ipfs::GetDefaultIPFSGateway(pref_service_).spec());
}

void HnsWalletIpfsService::SetGateway(const std::string& url,
                                        SetGatewayCallback callback) {
  std::move(callback).Run(
      ipfs::SetDefaultIPFSGateway(pref_service_, GURL(url)));
}

void HnsWalletIpfsService::SetNFTGateway(const std::string& url,
                                           SetNFTGatewayCallback callback) {
  std::move(callback).Run(
      ipfs::SetDefaultNFTIPFSGateway(pref_service_, GURL(url)));
}

void HnsWalletIpfsService::ContentHashToCIDv1URL(
    const std::vector<uint8_t>& content_hash,
    ContentHashToCIDv1URLCallback callback) {
  std::move(callback).Run(ipfs::ContentHashToCIDv1URL(content_hash).spec());
}

}  // namespace hns_wallet
