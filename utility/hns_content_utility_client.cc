/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/utility/hns_content_utility_client.h"

#include <memory>
#include <utility>

#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/services/bat_ads/bat_ads_service_impl.h"
#include "hns/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#include "hns/components/services/bat_rewards/public/interfaces/rewards_engine_factory.mojom.h"
#include "hns/components/services/bat_rewards/rewards_engine_factory.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "mojo/public/cpp/bindings/service_factory.h"

#if !BUILDFLAG(IS_ANDROID)
#include "hns/utility/importer/hns_profile_import_impl.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/services/ipfs/ipfs_service_impl.h"
#include "hns/components/services/ipfs/public/mojom/ipfs_service.mojom.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/services/tor/public/interfaces/tor.mojom.h"
#include "hns/components/services/tor/tor_launcher_impl.h"
#endif

namespace {

#if !BUILDFLAG(IS_ANDROID)
auto RunHnsProfileImporter(
    mojo::PendingReceiver<hns::mojom::ProfileImport> receiver) {
  return std::make_unique<HnsProfileImportImpl>(std::move(receiver));
}
#endif

#if BUILDFLAG(ENABLE_IPFS)
auto RunIpfsService(mojo::PendingReceiver<ipfs::mojom::IpfsService> receiver) {
  return std::make_unique<ipfs::IpfsServiceImpl>(std::move(receiver));
}
#endif

#if BUILDFLAG(ENABLE_TOR)
auto RunTorLauncher(mojo::PendingReceiver<tor::mojom::TorLauncher> receiver) {
  return std::make_unique<tor::TorLauncherImpl>(std::move(receiver));
}
#endif

auto RunRewardsEngineFactory(
    mojo::PendingReceiver<hns_rewards::mojom::RewardsEngineFactory>
        receiver) {
  return std::make_unique<hns_rewards::internal::RewardsEngineFactory>(
      std::move(receiver));
}

auto RunBatAdsService(
    mojo::PendingReceiver<bat_ads::mojom::BatAdsService> receiver) {
  return std::make_unique<bat_ads::BatAdsServiceImpl>(std::move(receiver));
}

}  // namespace

HnsContentUtilityClient::HnsContentUtilityClient() = default;
HnsContentUtilityClient::~HnsContentUtilityClient() = default;

void HnsContentUtilityClient::RegisterMainThreadServices(
    mojo::ServiceFactory& services) {
#if !BUILDFLAG(IS_ANDROID)
  services.Add(RunHnsProfileImporter);
#endif

#if BUILDFLAG(ENABLE_IPFS)
  services.Add(RunIpfsService);
#endif

#if BUILDFLAG(ENABLE_TOR)
  services.Add(RunTorLauncher);
#endif

  services.Add(RunRewardsEngineFactory);

  services.Add(RunBatAdsService);

  return ChromeContentUtilityClient::RegisterMainThreadServices(services);
}
