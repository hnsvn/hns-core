/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/renderer/hns_url_loader_throttle_provider_impl.h"

#include <utility>

#include "hns/components/tor/buildflags/buildflags.h"
#include "hns/renderer/hns_content_renderer_client.h"

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/tor/renderer/onion_domain_throttle.h"
#endif

HnsURLLoaderThrottleProviderImpl::HnsURLLoaderThrottleProviderImpl(
    blink::ThreadSafeBrowserInterfaceBrokerProxy* broker,
    blink::URLLoaderThrottleProviderType type,
    ChromeContentRendererClient* chrome_content_renderer_client)
    : URLLoaderThrottleProviderImpl(broker,
                                    type,
                                    chrome_content_renderer_client),
      hns_content_renderer_client_(static_cast<HnsContentRendererClient*>(
          chrome_content_renderer_client)) {}

HnsURLLoaderThrottleProviderImpl::~HnsURLLoaderThrottleProviderImpl() =
    default;

blink::WebVector<std::unique_ptr<blink::URLLoaderThrottle>>
HnsURLLoaderThrottleProviderImpl::CreateThrottles(
    int render_frame_id,
    const blink::WebURLRequest& request) {
  auto throttles =
      URLLoaderThrottleProviderImpl::CreateThrottles(render_frame_id, request);
#if BUILDFLAG(ENABLE_TOR)
  if (auto onion_domain_throttle =
          tor::OnionDomainThrottle::MaybeCreateThrottle(
              hns_content_renderer_client_->IsTorProcess())) {
    throttles.emplace_back(std::move(onion_domain_throttle));
  }
#endif
  return throttles;
}
