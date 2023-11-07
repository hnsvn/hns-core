/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/test/base/testing_hns_browser_process.h"

#include <utility>

#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"

namespace tor {
class HnsTorClientUpdater;
}

namespace ipfs {
class HnsIpfsClientUpdater;
}

// static
TestingHnsBrowserProcess* TestingHnsBrowserProcess::GetGlobal() {
  return static_cast<TestingHnsBrowserProcess*>(g_hns_browser_process);
}

// static
void TestingHnsBrowserProcess::CreateInstance() {
  DCHECK(!g_hns_browser_process);
  TestingHnsBrowserProcess* process = new TestingHnsBrowserProcess;
  g_hns_browser_process = process;
}

// static
void TestingHnsBrowserProcess::DeleteInstance() {
  HnsBrowserProcess* browser_process = g_hns_browser_process;
  g_hns_browser_process = nullptr;
  delete browser_process;
}

// static
void TestingHnsBrowserProcess::StartTearDown() {}

// static
void TestingHnsBrowserProcess::TearDownAndDeleteInstance() {
  TestingHnsBrowserProcess::StartTearDown();
  TestingHnsBrowserProcess::DeleteInstance();
}

TestingHnsBrowserProcess::TestingHnsBrowserProcess() = default;

TestingHnsBrowserProcess::~TestingHnsBrowserProcess() = default;

void TestingHnsBrowserProcess::StartHnsServices() {}

hns_shields::AdBlockService* TestingHnsBrowserProcess::ad_block_service() {
  DCHECK(ad_block_service_);
  return ad_block_service_.get();
}

#if BUILDFLAG(ENABLE_GREASELION)
greaselion::GreaselionDownloadService*
TestingHnsBrowserProcess::greaselion_download_service() {
  NOTREACHED();
  return nullptr;
}
#endif

debounce::DebounceComponentInstaller*
TestingHnsBrowserProcess::debounce_component_installer() {
  NOTREACHED();
  return nullptr;
}

#if BUILDFLAG(ENABLE_REQUEST_OTR)
request_otr::RequestOTRComponentInstallerPolicy*
TestingHnsBrowserProcess::request_otr_component_installer() {
  NOTREACHED();
  return nullptr;
}
#endif

hns::URLSanitizerComponentInstaller*
TestingHnsBrowserProcess::URLSanitizerComponentInstaller() {
  NOTREACHED();
  return nullptr;
}

https_upgrade_exceptions::HttpsUpgradeExceptionsService*
TestingHnsBrowserProcess::https_upgrade_exceptions_service() {
  return nullptr;
}

localhost_permission::LocalhostPermissionComponent*
TestingHnsBrowserProcess::localhost_permission_component() {
  return nullptr;
}

hns_shields::HTTPSEverywhereService*
TestingHnsBrowserProcess::https_everywhere_service() {
  NOTREACHED();
  return nullptr;
}

hns_component_updater::LocalDataFilesService*
TestingHnsBrowserProcess::local_data_files_service() {
  NOTREACHED();
  return nullptr;
}

#if BUILDFLAG(ENABLE_TOR)
tor::HnsTorClientUpdater* TestingHnsBrowserProcess::tor_client_updater() {
  return nullptr;
}

tor::HnsTorPluggableTransportUpdater*
TestingHnsBrowserProcess::tor_pluggable_transport_updater() {
  return nullptr;
}
#endif

#if BUILDFLAG(ENABLE_IPFS)
ipfs::HnsIpfsClientUpdater*
TestingHnsBrowserProcess::ipfs_client_updater() {
  return nullptr;
}
#endif

p3a::P3AService* TestingHnsBrowserProcess::p3a_service() {
  NOTREACHED();
  return nullptr;
}

hns::HnsReferralsService*
TestingHnsBrowserProcess::hns_referrals_service() {
  NOTREACHED();
  return nullptr;
}

hns_stats::HnsStatsUpdater*
TestingHnsBrowserProcess::hns_stats_updater() {
  NOTREACHED();
  return nullptr;
}

hns_ads::HnsStatsHelper*
TestingHnsBrowserProcess::ads_hns_stats_helper() {
  NOTREACHED();
  return nullptr;
}

ntp_background_images::NTPBackgroundImagesService*
TestingHnsBrowserProcess::ntp_background_images_service() {
  NOTREACHED();
  return nullptr;
}

#if BUILDFLAG(ENABLE_SPEEDREADER)
speedreader::SpeedreaderRewriterService*
TestingHnsBrowserProcess::speedreader_rewriter_service() {
  NOTREACHED();
  return nullptr;
}
#endif

hns_ads::ResourceComponent* TestingHnsBrowserProcess::resource_component() {
  NOTREACHED();
  return nullptr;
}

hns::HnsFarblingService*
TestingHnsBrowserProcess::hns_farbling_service() {
  NOTREACHED();
  return nullptr;
}

#if BUILDFLAG(ENABLE_HNS_VPN)
hns_vpn::HnsVPNOSConnectionAPI*
TestingHnsBrowserProcess::hns_vpn_os_connection_api() {
  return nullptr;
}
#endif

misc_metrics::ProcessMiscMetrics*
TestingHnsBrowserProcess::process_misc_metrics() {
  NOTREACHED();
  return nullptr;
}

void TestingHnsBrowserProcess::SetAdBlockService(
    std::unique_ptr<hns_shields::AdBlockService> service) {
  ad_block_service_ = std::move(service);
}

///////////////////////////////////////////////////////////////////////////////

TestingHnsBrowserProcessInitializer::TestingHnsBrowserProcessInitializer() {
  TestingHnsBrowserProcess::CreateInstance();
}

TestingHnsBrowserProcessInitializer::
    ~TestingHnsBrowserProcessInitializer() {
  TestingHnsBrowserProcess::DeleteInstance();
}
