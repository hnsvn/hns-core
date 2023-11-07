/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_BROWSER_PROCESS_IMPL_H_
#define HNS_BROWSER_HNS_BROWSER_PROCESS_IMPL_H_

#include <memory>

#include "base/memory/ref_counted.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/components/hns_component_updater/browser/hns_component.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/greaselion/browser/buildflags/buildflags.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/tor/hns_tor_pluggable_transport_updater.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "hns/components/url_sanitizer/browser/url_sanitizer_component_installer.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process_impl.h"
#include "extensions/buildflags/buildflags.h"

namespace hns {
class HnsReferralsService;
class HnsFarblingService;
}  // namespace hns

namespace hns_component_updater {
class LocalDataFilesService;
}  // namespace hns_component_updater

namespace hns_shields {
class AdBlockService;
class HTTPSEverywhereService;
}  // namespace hns_shields

namespace https_upgrade_exceptions {
class HttpsUpgradeExceptionsService;
}  // namespace https_upgrade_exceptions

namespace localhost_permission {
class LocalhostPermissionComponent;
}  // namespace localhost_permission

namespace hns_stats {
class HnsStatsUpdater;
}  // namespace hns_stats

namespace greaselion {
#if BUILDFLAG(ENABLE_GREASELION)
class GreaselionDownloadService;
#endif
}  // namespace greaselion

namespace debounce {
class DebounceComponentInstaller;
}  // namespace debounce

namespace misc_metrics {
class ProcessMiscMetrics;
}  // namespace misc_metrics

namespace request_otr {
#if BUILDFLAG(ENABLE_REQUEST_OTR)
class RequestOTRComponentInstallerPolicy;
#endif
}  // namespace request_otr

namespace ntp_background_images {
class NTPBackgroundImagesService;
}  // namespace ntp_background_images

namespace p3a {
class HistogramsHnsizer;
class P3AService;
}  // namespace p3a

namespace tor {
class HnsTorClientUpdater;
class HnsTorPluggableTransportUpdater;
}  // namespace tor

namespace ipfs {
class HnsIpfsClientUpdater;
}

namespace speedreader {
class SpeedreaderRewriterService;
}

namespace hns_ads {
class HnsStatsHelper;
class ResourceComponent;
}  // namespace hns_ads

class HnsBrowserProcessImpl : public HnsBrowserProcess,
                                public BrowserProcessImpl {
 public:
  explicit HnsBrowserProcessImpl(StartupData* startup_data);
  HnsBrowserProcessImpl(const HnsBrowserProcessImpl&) = delete;
  HnsBrowserProcessImpl& operator=(const HnsBrowserProcessImpl&) = delete;
  ~HnsBrowserProcessImpl() override;

  // BrowserProcess implementation.

  ProfileManager* profile_manager() override;
  NotificationPlatformBridge* notification_platform_bridge() override;

  // HnsBrowserProcess implementation.

  void StartHnsServices() override;
  hns_shields::AdBlockService* ad_block_service() override;
  https_upgrade_exceptions::HttpsUpgradeExceptionsService*
  https_upgrade_exceptions_service() override;
  localhost_permission::LocalhostPermissionComponent*
  localhost_permission_component() override;
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
  hns_ads::ResourceComponent* resource_component() override;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderRewriterService* speedreader_rewriter_service()
      override;
#endif
#if BUILDFLAG(ENABLE_HNS_VPN)
  hns_vpn::HnsVPNOSConnectionAPI* hns_vpn_os_connection_api() override;
#endif
  hns::HnsFarblingService* hns_farbling_service() override;
  misc_metrics::ProcessMiscMetrics* process_misc_metrics() override;

 private:
  // BrowserProcessImpl overrides:
  void Init() override;
#if !BUILDFLAG(IS_ANDROID)
  void StartTearDown() override;
  void PostDestroyThreads() override;
#endif

  void CreateProfileManager();

#if BUILDFLAG(ENABLE_TOR)
  void OnTorEnabledChanged();
#endif

  void UpdateHnsDarkMode();
  void OnHnsDarkModeChanged();

  void InitHnsStatsHelper();

  hns_component_updater::HnsComponent::Delegate*
  hns_component_updater_delegate();

  // local_data_files_service_ should always be first because it needs
  // to be destroyed last
  std::unique_ptr<hns_component_updater::LocalDataFilesService>
      local_data_files_service_;
  std::unique_ptr<hns_component_updater::HnsComponent::Delegate>
      hns_component_updater_delegate_;
  std::unique_ptr<hns_shields::AdBlockService> ad_block_service_;
  std::unique_ptr<https_upgrade_exceptions::HttpsUpgradeExceptionsService>
      https_upgrade_exceptions_service_;
  std::unique_ptr<localhost_permission::LocalhostPermissionComponent>
      localhost_permission_component_;
#if BUILDFLAG(ENABLE_GREASELION)
  std::unique_ptr<greaselion::GreaselionDownloadService>
      greaselion_download_service_;
#endif
  std::unique_ptr<debounce::DebounceComponentInstaller>
      debounce_component_installer_;
#if BUILDFLAG(ENABLE_REQUEST_OTR)
  std::unique_ptr<request_otr::RequestOTRComponentInstallerPolicy>
      request_otr_component_installer_;
#endif
  std::unique_ptr<hns::URLSanitizerComponentInstaller>
      url_sanitizer_component_installer_;
  bool created_https_everywhere_service_ = false;
  std::unique_ptr<hns_shields::HTTPSEverywhereService>
      https_everywhere_service_;
  std::unique_ptr<hns_stats::HnsStatsUpdater> hns_stats_updater_;
  std::unique_ptr<hns::HnsReferralsService> hns_referrals_service_;
#if BUILDFLAG(ENABLE_TOR)
  std::unique_ptr<tor::HnsTorClientUpdater> tor_client_updater_;
  std::unique_ptr<tor::HnsTorPluggableTransportUpdater>
      tor_pluggable_transport_updater_;
#endif
#if BUILDFLAG(ENABLE_IPFS)
  std::unique_ptr<ipfs::HnsIpfsClientUpdater> ipfs_client_updater_;
#endif
  scoped_refptr<p3a::P3AService> p3a_service_;
  scoped_refptr<p3a::HistogramsHnsizer> histogram_hnsizer_;
  std::unique_ptr<ntp_background_images::NTPBackgroundImagesService>
      ntp_background_images_service_;
  std::unique_ptr<hns_ads::ResourceComponent> resource_component_;

#if BUILDFLAG(ENABLE_SPEEDREADER)
  std::unique_ptr<speedreader::SpeedreaderRewriterService>
      speedreader_rewriter_service_;
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
  std::unique_ptr<hns_vpn::HnsVPNOSConnectionAPI>
      hns_vpn_os_connection_api_;
#endif

  std::unique_ptr<hns::HnsFarblingService> hns_farbling_service_;
  std::unique_ptr<misc_metrics::ProcessMiscMetrics> process_misc_metrics_;
  std::unique_ptr<hns_ads::HnsStatsHelper> hns_stats_helper_;

  SEQUENCE_CHECKER(sequence_checker_);
};

#endif  // HNS_BROWSER_HNS_BROWSER_PROCESS_IMPL_H_
