/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_RENDERER_HNS_URL_LOADER_THROTTLE_PROVIDER_IMPL_H_
#define HNS_RENDERER_HNS_URL_LOADER_THROTTLE_PROVIDER_IMPL_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "chrome/renderer/url_loader_throttle_provider_impl.h"

class HnsContentRendererClient;

class HnsURLLoaderThrottleProviderImpl
    : public URLLoaderThrottleProviderImpl {
 public:
  HnsURLLoaderThrottleProviderImpl(
      blink::ThreadSafeBrowserInterfaceBrokerProxy* broker,
      blink::URLLoaderThrottleProviderType type,
      ChromeContentRendererClient* chrome_content_renderer_client);
  ~HnsURLLoaderThrottleProviderImpl() override;

  HnsURLLoaderThrottleProviderImpl(
      const HnsURLLoaderThrottleProviderImpl&) = delete;
  HnsURLLoaderThrottleProviderImpl& operator=(
      const HnsURLLoaderThrottleProviderImpl&) = delete;

  // blink::URLLoaderThrottleProvider implementation.
  blink::WebVector<std::unique_ptr<blink::URLLoaderThrottle>> CreateThrottles(
      int render_frame_id,
      const blink::WebURLRequest& request) override;

 private:
  raw_ptr<HnsContentRendererClient> hns_content_renderer_client_ = nullptr;
};

#endif  // HNS_RENDERER_HNS_URL_LOADER_THROTTLE_PROVIDER_IMPL_H_
