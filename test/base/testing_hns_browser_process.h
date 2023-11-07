/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

// An implementation of HnsBrowserProcess for unit tests that fails for most
// services. By preventing creation of services, we reduce dependencies and
// keep the profile clean. Clients of this class must handle the NULL return
// value, however.

#ifndef HNS_TEST_BASE_TESTING_HNS_BROWSER_PROCESS_H_
#define HNS_TEST_BASE_TESTING_HNS_BROWSER_PROCESS_H_

#include <stdint.h>

#include <memory>
#include <string>

#include "hns/browser/hns_browser_process.h"

namespace hns_shields {
class AdBlockService;
}

class TestingHnsBrowserProcess : public HnsBrowserProcess {
 public:
  // Initializes |g_hns_browser_process| with a new
  // TestingHnsBrowserProcess.
  static void CreateInstance();

  // Cleanly destroys |g_hns_browser_process|.
  static void DeleteInstance();

  // Convenience method to get g_hns_browser_process as a
  // TestingHnsBrowserProcess*.
  static TestingHnsBrowserProcess* GetGlobal();

  // Convenience method to both teardown and destroy the TestingBrowserProcess
  // instance
  static void TearDownAndDeleteInstance();

  TestingHnsBrowserProcess(const TestingHnsBrowserProcess&) = delete;
  TestingHnsBrowserProcess& operator=(const TestingHnsBrowserProcess&) =
      delete;

  // HnsBrowserProcess overrides:
  void StartHnsServices() override;
  hns_shields::AdBlockService* ad_block_service() override;
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionDownloadService* greaselion_download_service() override;
#endif
  debounce::DebounceComponentInstaller* debounce_component_installer() override;
#if BUILDFLAG(ENABLE_REQUEST_OTR)
  request_otr::RequestOTRComponentInstallerPolicy*
  request_otr_component_installer() override;
#endif
  hns::URLSanitizerComponentInstaller* URLSanitizerComponentInstaller()
      override;
  hns_shields::HTTPSEverywhereService* https_everywhere_service() override;
  https_upgrade_exceptions::HttpsUpgradeExceptionsService*
  https_upgrade_exceptions_service() override;
  localhost_permission::LocalhostPermissionComponent*
  localhost_permission_component() override;
  hns_component_updater::LocalDataFilesService* local_data_files_service()
      override;
#if BUILDFLAG(ENABLE_TOR)
  tor::HnsTorClientUpdater* tor_client_updater() override;
  tor::HnsTorPluggableTransportUpdater* tor_pluggable_transport_updater()
      override;
#endif
#if BUILDFLAG(ENABLE_IPFS)
  ipfs::HnsIpfsClientUpdater* ipfs_client_updater() override;
#endif
  p3a::P3AService* p3a_service() override;
  hns::HnsReferralsService* hns_referrals_service() override;
  hns_stats::HnsStatsUpdater* hns_stats_updater() override;
  hns_ads::HnsStatsHelper* ads_hns_stats_helper() override;
  ntp_background_images::NTPBackgroundImagesService*
  ntp_background_images_service() override;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderRewriterService* speedreader_rewriter_service()
      override;
#endif
  hns_ads::ResourceComponent* resource_component() override;
  hns::HnsFarblingService* hns_farbling_service() override;
#if BUILDFLAG(ENABLE_HNS_VPN)
  hns_vpn::HnsVPNOSConnectionAPI* hns_vpn_os_connection_api() override;
#endif
  misc_metrics::ProcessMiscMetrics* process_misc_metrics() override;

  // Populate the mock process with services. Consumer is responsible for
  // cleaning these up after completion of a test.
  void SetAdBlockService(std::unique_ptr<hns_shields::AdBlockService>);

 private:
  // Perform necessary cleanup prior to destruction of |g_browser_process|
  static void StartTearDown();

  // See CreateInstance() and DestroyInstance() above.
  TestingHnsBrowserProcess();
  ~TestingHnsBrowserProcess() override;

  std::unique_ptr<hns_shields::AdBlockService> ad_block_service_;
};

class TestingHnsBrowserProcessInitializer {
 public:
  TestingHnsBrowserProcessInitializer();
  TestingHnsBrowserProcessInitializer(
      const TestingHnsBrowserProcessInitializer&) = delete;
  TestingHnsBrowserProcessInitializer& operator=(
      const TestingHnsBrowserProcessInitializer&) = delete;
  ~TestingHnsBrowserProcessInitializer();
};

#endif  // HNS_TEST_BASE_TESTING_HNS_BROWSER_PROCESS_H_
