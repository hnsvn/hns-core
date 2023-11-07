/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_RENDERER_HNS_CONTENT_RENDERER_CLIENT_H_
#define HNS_RENDERER_HNS_CONTENT_RENDERER_CLIENT_H_

#include <memory>

#include "hns/components/hns_search/renderer/hns_search_service_worker_holder.h"
#include "chrome/renderer/chrome_content_renderer_client.h"
#include "v8/include/v8.h"

class HnsRenderThreadObserver;
class GURL;

namespace blink {
class WebServiceWorkerContextProxy;
}

class HnsContentRendererClient : public ChromeContentRendererClient {
 public:
  HnsContentRendererClient();
  HnsContentRendererClient(const HnsContentRendererClient&) = delete;
  HnsContentRendererClient& operator=(const HnsContentRendererClient&) =
      delete;
  ~HnsContentRendererClient() override;

  void RenderThreadStarted() override;
  void SetRuntimeFeaturesDefaultsBeforeBlinkInitialization() override;
  void RenderFrameCreated(content::RenderFrame* render_frame) override;
  void RunScriptsAtDocumentStart(content::RenderFrame* render_frame) override;
  void WillEvaluateServiceWorkerOnWorkerThread(
      blink::WebServiceWorkerContextProxy* context_proxy,
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url) override;
  void WillDestroyServiceWorkerContextOnWorkerThread(
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url) override;
  std::unique_ptr<blink::URLLoaderThrottleProvider>
  CreateURLLoaderThrottleProvider(
      blink::URLLoaderThrottleProviderType provider_type) override;

  bool IsTorProcess() const;

 private:
  std::unique_ptr<HnsRenderThreadObserver> hns_observer_;
  hns_search::HnsSearchServiceWorkerHolder
      hns_search_service_worker_holder_;
};

#endif  // HNS_RENDERER_HNS_CONTENT_RENDERER_CLIENT_H_
