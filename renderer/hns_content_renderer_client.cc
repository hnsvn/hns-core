/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/renderer/hns_content_renderer_client.h"

#include "base/feature_list.h"
#include "hns/components/hns_search/common/hns_search_utils.h"
#include "hns/components/hns_search/renderer/hns_search_render_frame_observer.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/common/features.h"
#include "hns/components/cosmetic_filters/renderer/cosmetic_filters_js_render_frame_observer.h"
#include "hns/components/playlist/common/buildflags/buildflags.h"
#include "hns/components/safe_builtins/renderer/safe_builtins.h"
#include "hns/components/skus/common/features.h"
#include "hns/components/skus/renderer/skus_render_frame_observer.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/renderer/hns_render_thread_observer.h"
#include "hns/renderer/hns_url_loader_throttle_provider_impl.h"
#include "hns/renderer/hns_wallet/hns_wallet_render_frame_observer.h"
#include "chrome/common/chrome_isolated_world_ids.h"
#include "chrome/renderer/chrome_render_thread_observer.h"
#include "content/public/renderer/render_thread.h"
#include "third_party/blink/public/common/features.h"
#include "third_party/blink/public/platform/web_runtime_features.h"
#include "third_party/blink/public/web/modules/service_worker/web_service_worker_context_proxy.h"
#include "third_party/blink/public/web/web_script_controller.h"
#include "url/gurl.h"

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/components/speedreader/common/features.h"
#include "hns/components/speedreader/renderer/speedreader_render_frame_observer.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#if BUILDFLAG(IS_ANDROID)
#include "hns/components/hns_vpn/renderer/android/vpn_render_frame_observer.h"
#endif  // BUILDFLAG(IS_ANDROID)
#endif  // BUILDFLAG(ENABLE_HNS_VPN)

#if BUILDFLAG(ENABLE_PLAYLIST)
#include "hns/components/playlist/common/features.h"
#include "hns/components/playlist/renderer/playlist_render_frame_observer.h"
#endif

HnsContentRendererClient::HnsContentRendererClient() = default;

void HnsContentRendererClient::
    SetRuntimeFeaturesDefaultsBeforeBlinkInitialization() {
  ChromeContentRendererClient::
      SetRuntimeFeaturesDefaultsBeforeBlinkInitialization();

  blink::WebRuntimeFeatures::EnableWebNFC(false);
  blink::WebRuntimeFeatures::EnableAnonymousIframe(false);

  // These features don't have dedicated WebRuntimeFeatures wrappers.
  blink::WebRuntimeFeatures::EnableFeatureFromString("DigitalGoods", false);
  if (!base::FeatureList::IsEnabled(blink::features::kFileSystemAccessAPI)) {
    blink::WebRuntimeFeatures::EnableFeatureFromString("FileSystemAccessLocal",
                                                       false);
    blink::WebRuntimeFeatures::EnableFeatureFromString(
        "FileSystemAccessAPIExperimental", false);
  }
  if (!base::FeatureList::IsEnabled(blink::features::kHnsWebSerialAPI)) {
    blink::WebRuntimeFeatures::EnableFeatureFromString("Serial", false);
  }
  blink::WebRuntimeFeatures::EnableFeatureFromString(
      "SpeculationRulesPrefetchProxy", false);
  blink::WebRuntimeFeatures::EnableFeatureFromString("AdTagging", false);
  blink::WebRuntimeFeatures::EnableFeatureFromString("WebEnvironmentIntegrity",
                                                     false);
}

HnsContentRendererClient::~HnsContentRendererClient() = default;

void HnsContentRendererClient::RenderThreadStarted() {
  ChromeContentRendererClient::RenderThreadStarted();

  hns_observer_ = std::make_unique<HnsRenderThreadObserver>();
  content::RenderThread::Get()->AddObserver(hns_observer_.get());
  hns_search_service_worker_holder_.SetBrowserInterfaceBrokerProxy(
      browser_interface_broker_.get());

  blink::WebScriptController::RegisterExtension(
      hns::SafeBuiltins::CreateV8Extension());
}

void HnsContentRendererClient::RenderFrameCreated(
    content::RenderFrame* render_frame) {
  ChromeContentRendererClient::RenderFrameCreated(render_frame);

  if (base::FeatureList::IsEnabled(
          hns_shields::features::kHnsAdblockCosmeticFiltering)) {
    auto dynamic_params_closure = base::BindRepeating([]() {
      auto dynamic_params = HnsRenderThreadObserver::GetDynamicParams();
      return dynamic_params.de_amp_enabled;
    });

    new cosmetic_filters::CosmeticFiltersJsRenderFrameObserver(
        render_frame, ISOLATED_WORLD_ID_HNS_INTERNAL, dynamic_params_closure);
  }

  if (base::FeatureList::IsEnabled(
          hns_wallet::features::kNativeHnsWalletFeature)) {
    new hns_wallet::HnsWalletRenderFrameObserver(
        render_frame,
        base::BindRepeating(&HnsRenderThreadObserver::GetDynamicParams));
  }

  if (hns_search::IsDefaultAPIEnabled()) {
    new hns_search::HnsSearchRenderFrameObserver(
        render_frame, content::ISOLATED_WORLD_ID_GLOBAL);
  }

  if (base::FeatureList::IsEnabled(skus::features::kSkusFeature) &&
      !ChromeRenderThreadObserver::is_incognito_process()) {
    new skus::SkusRenderFrameObserver(render_frame);
  }

#if BUILDFLAG(IS_ANDROID)
  if (hns_vpn::IsHnsVPNFeatureEnabled()) {
    new hns_vpn::VpnRenderFrameObserver(render_frame,
                                          content::ISOLATED_WORLD_ID_GLOBAL);
  }
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
  if (base::FeatureList::IsEnabled(speedreader::kSpeedreaderFeature)) {
    new speedreader::SpeedreaderRenderFrameObserver(
        render_frame, ISOLATED_WORLD_ID_HNS_INTERNAL);
  }
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
  if (base::FeatureList::IsEnabled(playlist::features::kPlaylist) &&
      !ChromeRenderThreadObserver::is_incognito_process()) {
    new playlist::PlaylistRenderFrameObserver(render_frame,
                                              ISOLATED_WORLD_ID_HNS_INTERNAL);
  }
#endif
}

void HnsContentRendererClient::RunScriptsAtDocumentStart(
    content::RenderFrame* render_frame) {
  auto* observer =
      cosmetic_filters::CosmeticFiltersJsRenderFrameObserver::Get(render_frame);
  // Run this before any extensions
  if (observer) {
    observer->RunScriptsAtDocumentStart();
  }

#if BUILDFLAG(ENABLE_PLAYLIST)
  if (base::FeatureList::IsEnabled(playlist::features::kPlaylist)) {
    if (auto* playlist_observer =
            playlist::PlaylistRenderFrameObserver::Get(render_frame)) {
      playlist_observer->RunScriptsAtDocumentStart();
    }
  }
#endif

  ChromeContentRendererClient::RunScriptsAtDocumentStart(render_frame);
}

void HnsContentRendererClient::WillEvaluateServiceWorkerOnWorkerThread(
    blink::WebServiceWorkerContextProxy* context_proxy,
    v8::Local<v8::Context> v8_context,
    int64_t service_worker_version_id,
    const GURL& service_worker_scope,
    const GURL& script_url) {
  hns_search_service_worker_holder_.WillEvaluateServiceWorkerOnWorkerThread(
      context_proxy, v8_context, service_worker_version_id,
      service_worker_scope, script_url);
  ChromeContentRendererClient::WillEvaluateServiceWorkerOnWorkerThread(
      context_proxy, v8_context, service_worker_version_id,
      service_worker_scope, script_url);
}

void HnsContentRendererClient::WillDestroyServiceWorkerContextOnWorkerThread(
    v8::Local<v8::Context> v8_context,
    int64_t service_worker_version_id,
    const GURL& service_worker_scope,
    const GURL& script_url) {
  hns_search_service_worker_holder_
      .WillDestroyServiceWorkerContextOnWorkerThread(
          v8_context, service_worker_version_id, service_worker_scope,
          script_url);
  ChromeContentRendererClient::WillDestroyServiceWorkerContextOnWorkerThread(
      v8_context, service_worker_version_id, service_worker_scope, script_url);
}

std::unique_ptr<blink::URLLoaderThrottleProvider>
HnsContentRendererClient::CreateURLLoaderThrottleProvider(
    blink::URLLoaderThrottleProviderType provider_type) {
  return std::make_unique<HnsURLLoaderThrottleProviderImpl>(
      browser_interface_broker_.get(), provider_type, this);
}

bool HnsContentRendererClient::IsTorProcess() const {
  return hns_observer_->is_tor_process();
}
