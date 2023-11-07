/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// This interface is for managing the global services of the application. Each
// service is lazily created when requested the first time. The service getters
// will return NULL if the service is not available, so callers must check for
// this condition.

#ifndef HNS_BROWSER_HNS_BROWSER_PROCESS_H_
#define HNS_BROWSER_HNS_BROWSER_PROCESS_H_

#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/greaselion/browser/buildflags/buildflags.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/request_otr/common/buildflags/buildflags.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "extensions/buildflags/buildflags.h"

namespace hns {
class HnsReferralsService;
class HnsFarblingService;
class URLSanitizerComponentInstaller;
}  // namespace hns

#if BUILDFLAG(ENABLE_HNS_VPN)
namespace hns_vpn {
class HnsVPNOSConnectionAPI;
}  // namespace hns_vpn
#endif

namespace hns_component_updater {
class LocalDataFilesService;
}  // namespace hns_component_updater

namespace hns_shields {
class AdBlockService;
class HTTPSEverywhereService;
}  // namespace hns_shields

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

namespace https_upgrade_exceptions {
class HttpsUpgradeExceptionsService;
}  // namespace https_upgrade_exceptions

namespace localhost_permission {
class LocalhostPermissionComponent;
}  // namespace localhost_permission

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

class HnsBrowserProcess {
 public:
  HnsBrowserProcess();
  virtual ~HnsBrowserProcess();
  virtual void StartHnsServices() = 0;
  virtual hns_shields::AdBlockService* ad_block_service() = 0;
  virtual https_upgrade_exceptions::HttpsUpgradeExceptionsService*
  https_upgrade_exceptions_service() = 0;
  virtual localhost_permission::LocalhostPermissionComponent*
  localhost_permission_component() = 0;
#if BUILDFLAG(ENABLE_GREASELION)
  virtual greaselion::GreaselionDownloadService*
  greaselion_download_service() = 0;
#endif
  virtual debounce::DebounceComponentInstaller*
  debounce_component_installer() = 0;
#if BUILDFLAG(ENABLE_REQUEST_OTR)
  virtual request_otr::RequestOTRComponentInstallerPolicy*
  request_otr_component_installer() = 0;
#endif
  virtual hns::URLSanitizerComponentInstaller*
  URLSanitizerComponentInstaller() = 0;
  virtual hns_shields::HTTPSEverywhereService* https_everywhere_service() = 0;
  virtual hns_component_updater::LocalDataFilesService*
  local_data_files_service() = 0;
#if BUILDFLAG(ENABLE_TOR)
  virtual tor::HnsTorClientUpdater* tor_client_updater() = 0;
  virtual tor::HnsTorPluggableTransportUpdater*
  tor_pluggable_transport_updater() = 0;
#endif
#if BUILDFLAG(ENABLE_IPFS)
  virtual ipfs::HnsIpfsClientUpdater* ipfs_client_updater() = 0;
#endif
  virtual p3a::P3AService* p3a_service() = 0;
  virtual hns::HnsReferralsService* hns_referrals_service() = 0;
  virtual hns_stats::HnsStatsUpdater* hns_stats_updater() = 0;
  virtual hns_ads::HnsStatsHelper* ads_hns_stats_helper() = 0;
  virtual ntp_background_images::NTPBackgroundImagesService*
  ntp_background_images_service() = 0;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  virtual speedreader::SpeedreaderRewriterService*
  speedreader_rewriter_service() = 0;
#endif
#if BUILDFLAG(ENABLE_HNS_VPN)
  virtual hns_vpn::HnsVPNOSConnectionAPI* hns_vpn_os_connection_api() = 0;
#endif
  virtual hns_ads::ResourceComponent* resource_component() = 0;
  virtual hns::HnsFarblingService* hns_farbling_service() = 0;
  virtual misc_metrics::ProcessMiscMetrics* process_misc_metrics() = 0;
};

extern HnsBrowserProcess* g_hns_browser_process;

#endif  // HNS_BROWSER_HNS_BROWSER_PROCESS_H_
