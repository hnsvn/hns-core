/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_tab_helpers.h"

#include "base/command_line.h"
#include "base/feature_list.h"
#include "hns/browser/hns_ads/ads_tab_helper.h"
#include "hns/browser/hns_ads/search_result_ad/search_result_ad_tab_helper.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/hns_news/hns_news_tab_helper.h"
#include "hns/browser/hns_rewards/rewards_tab_helper.h"
#include "hns/browser/hns_shields/hns_shields_web_contents_observer.h"
#include "hns/browser/hns_stats/hns_stats_tab_helper.h"
#include "hns/browser/hns_wallet/hns_wallet_tab_helper.h"
#include "hns/browser/ephemeral_storage/ephemeral_storage_tab_helper.h"
#include "hns/browser/misc_metrics/page_metrics_tab_helper.h"
#include "hns/browser/misc_metrics/process_misc_metrics.h"
#include "hns/browser/ntp_background/ntp_tab_helper.h"
#include "hns/browser/ui/bookmark/hns_bookmark_tab_helper.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/hns_perf_predictor/browser/perf_predictor_tab_helper.h"
#include "hns/components/hns_wayback_machine/buildflags/buildflags.h"
#include "hns/components/greaselion/browser/buildflags/buildflags.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/playlist/common/buildflags/buildflags.h"
#include "hns/components/request_otr/common/buildflags/buildflags.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "extensions/buildflags/buildflags.h"
#include "net/base/features.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(ENABLE_GREASELION)
#include "hns/browser/greaselion/greaselion_tab_helper.h"
#endif

#if BUILDFLAG(IS_ANDROID)
#include "hns/browser/android/preferences/background_video_playback_tab_helper.h"
#endif

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/ui/hns_shields_data_controller.h"
#include "chrome/browser/ui/thumbnails/thumbnail_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/components/ai_chat/browser/ai_chat_tab_helper.h"
#include "hns/components/ai_chat/common/features.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "hns/browser/hns_drm_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_HNS_WAYBACK_MACHINE)
#include "hns/browser/infobars/hns_wayback_machine_delegate_impl.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/browser/speedreader/speedreader_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/tor/onion_location_tab_helper.h"
#include "hns/components/tor/tor_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/browser/ipfs/ipfs_service_factory.h"
#include "hns/browser/ipfs/ipfs_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "hns/browser/web_discovery/web_discovery_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_REQUEST_OTR)
#include "hns/browser/request_otr/request_otr_tab_helper.h"
#include "hns/components/request_otr/common/features.h"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
#include "hns/browser/playlist/playlist_tab_helper.h"
#endif

#if defined(TOOLKIT_VIEWS)
#include "hns/browser/ui/sidebar/sidebar_tab_helper.h"
#endif

namespace hns {

void AttachTabHelpers(content::WebContents* web_contents) {
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionTabHelper::CreateForWebContents(web_contents);
#endif
  hns_shields::HnsShieldsWebContentsObserver::CreateForWebContents(
      web_contents);
#if BUILDFLAG(IS_ANDROID)
  BackgroundVideoPlaybackTabHelper::CreateForWebContents(web_contents);
#else
  // Add tab helpers here unless they are intended for android too
  HnsBookmarkTabHelper::CreateForWebContents(web_contents);
  hns_shields::HnsShieldsDataController::CreateForWebContents(web_contents);
  ThumbnailTabHelper::CreateForWebContents(web_contents);
#endif

  hns_rewards::RewardsTabHelper::CreateForWebContents(web_contents);

#if BUILDFLAG(ENABLE_AI_CHAT)
  if (ai_chat::features::IsAIChatEnabled()) {
    ai_chat::AIChatTabHelper::CreateForWebContents(
        web_contents,
        g_hns_browser_process->process_misc_metrics()->ai_chat_metrics());
  }
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
  HnsDrmTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_HNS_WAYBACK_MACHINE)
  HnsWaybackMachineDelegateImpl::AttachTabHelperIfNeeded(web_contents);
#endif

  hns_perf_predictor::PerfPredictorTabHelper::CreateForWebContents(
      web_contents);

  hns_ads::AdsTabHelper::CreateForWebContents(web_contents);

  hns_ads::SearchResultAdTabHelper::MaybeCreateForWebContents(web_contents);

#if BUILDFLAG(ENABLE_EXTENSIONS)
  WebDiscoveryTabHelper::MaybeCreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderTabHelper::MaybeCreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_TOR)
  tor::TorTabHelper::MaybeCreateForWebContents(
      web_contents, web_contents->GetBrowserContext()->IsTor());
  tor::OnionLocationTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_IPFS)
  ipfs::IPFSTabHelper::MaybeCreateForWebContents(web_contents);
#endif

#if defined(TOOLKIT_VIEWS)
  SidebarTabHelper::CreateForWebContents(web_contents);
#endif

  if (!web_contents->GetBrowserContext()->IsOffTheRecord()) {
    HnsNewsTabHelper::CreateForWebContents(web_contents);
  }

  hns_stats::HnsStatsTabHelper::CreateForWebContents(web_contents);

  if (base::FeatureList::IsEnabled(net::features::kHnsEphemeralStorage)) {
    ephemeral_storage::EphemeralStorageTabHelper::CreateForWebContents(
        web_contents);
  }

  hns_wallet::HnsWalletTabHelper::CreateForWebContents(web_contents);

  if (!web_contents->GetBrowserContext()->IsOffTheRecord()) {
    ntp_background_images::NTPTabHelper::CreateForWebContents(web_contents);
    misc_metrics::PageMetricsTabHelper::CreateForWebContents(web_contents);
#if BUILDFLAG(ENABLE_REQUEST_OTR)
    if (base::FeatureList::IsEnabled(
            request_otr::features::kHnsRequestOTRTab)) {
      RequestOTRTabHelper::CreateForWebContents(web_contents);
    }
#endif
  }

#if BUILDFLAG(ENABLE_PLAYLIST)
  playlist::PlaylistTabHelper::MaybeCreateForWebContents(web_contents);
#endif
}

}  // namespace hns
