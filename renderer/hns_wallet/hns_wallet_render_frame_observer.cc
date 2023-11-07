/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/renderer/hns_wallet/hns_wallet_render_frame_observer.h"

#include <memory>
#include <utility>

#include "base/feature_list.h"
#include "hns/components/hns_wallet/common/features.h"
#include "hns/components/hns_wallet/renderer/v8_helper.h"
#include "build/buildflag.h"
#include "content/public/common/isolated_world_ids.h"
#include "content/public/renderer/render_frame.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "third_party/blink/public/web/blink.h"
#include "third_party/blink/public/web/web_local_frame.h"

namespace hns_wallet {

HnsWalletRenderFrameObserver::HnsWalletRenderFrameObserver(
    content::RenderFrame* render_frame,
    GetDynamicParamsCallback get_dynamic_params_callback)
    : RenderFrameObserver(render_frame),
      get_dynamic_params_callback_(std::move(get_dynamic_params_callback)) {}

HnsWalletRenderFrameObserver::~HnsWalletRenderFrameObserver() = default;

void HnsWalletRenderFrameObserver::DidStartNavigation(
    const GURL& url,
    absl::optional<blink::WebNavigationType> navigation_type) {
  url_ = url;
}

bool HnsWalletRenderFrameObserver::IsPageValid() {
  // There could be empty, invalid and "about:blank" URLs,
  // they should fallback to the main frame rules
  if (url_.is_empty() || !url_.is_valid() || url_.spec() == "about:blank") {
    url_ = url::Origin(render_frame()->GetWebFrame()->GetSecurityOrigin())
               .GetURL();
  }
  return url_.SchemeIsHTTPOrHTTPS();
}

bool HnsWalletRenderFrameObserver::CanCreateProvider() {
  if (!IsPageValid()) {
    return false;
  }

  // Wallet provider objects should only be created in secure contexts
  if (!render_frame()->GetWebFrame()->GetDocument().IsSecureContext()) {
    return false;
  }

  return true;
}

void HnsWalletRenderFrameObserver::DidFinishLoad() {
#if !BUILDFLAG(IS_ANDROID)
  // Only record P3A for desktop and valid HTTP/HTTPS pages
  if (!IsPageValid()) {
    return;
  }

  auto dynamic_params = get_dynamic_params_callback_.Run();

  p3a_util_.ReportJSProviders(render_frame(), dynamic_params);
#endif
}

void HnsWalletRenderFrameObserver::DidClearWindowObject() {
  if (!CanCreateProvider()) {
    return;
  }

  v8::Isolate* isolate = blink::MainThreadIsolate();
  v8::HandleScope handle_scope(isolate);
  auto* web_frame = render_frame()->GetWebFrame();
  v8::Local<v8::Context> context = web_frame->MainWorldScriptContext();
  if (context.IsEmpty()) {
    return;
  }
  v8::MicrotasksScope microtasks(isolate, context->GetMicrotaskQueue(),
                                 v8::MicrotasksScope::kDoNotRunMicrotasks);

  auto dynamic_params = get_dynamic_params_callback_.Run();
  if (!dynamic_params.install_window_hns_ethereum_provider &&
      !dynamic_params.install_window_ethereum_provider &&
      !dynamic_params.hns_use_native_solana_wallet) {
    return;
  }

  if (!dynamic_params.install_window_hns_ethereum_provider &&
      dynamic_params.install_window_ethereum_provider) {
    NOTREACHED();
    return;
  }

  if (dynamic_params.install_window_hns_ethereum_provider &&
      web_frame->GetDocument().IsDOMFeaturePolicyEnabled(context, "ethereum")) {
    JSEthereumProvider::Install(
        dynamic_params.install_window_ethereum_provider,
        dynamic_params.allow_overwrite_window_ethereum_provider,
        render_frame());
  }

  if (base::FeatureList::IsEnabled(
          hns_wallet::features::kHnsWalletSolanaFeature) &&
      base::FeatureList::IsEnabled(
          hns_wallet::features::kHnsWalletSolanaProviderFeature) &&
      web_frame->GetDocument().IsDOMFeaturePolicyEnabled(context, "solana") &&
      dynamic_params.hns_use_native_solana_wallet) {
    JSSolanaProvider::Install(
        dynamic_params.allow_overwrite_window_solana_provider, render_frame());
  }
}

void HnsWalletRenderFrameObserver::OnDestruct() {
  delete this;
}

}  // namespace hns_wallet
