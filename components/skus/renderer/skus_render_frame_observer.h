// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_SKUS_RENDERER_SKUS_RENDER_FRAME_OBSERVER_H_
#define HNS_COMPONENTS_SKUS_RENDERER_SKUS_RENDER_FRAME_OBSERVER_H_

#include <memory>

#include "hns/components/skus/renderer/skus_js_handler.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "v8/include/v8.h"

namespace skus {

// Used to conditionally inject a handler which exposes JavaScript methods
// for the SKU SDK. The conditional logic ensures this is a Hns property
// such as account.hns.com.
//
// Implementation-wise, those methods will only resolve in a regular
// (non-private / non-guest / non-Tor) context.
//
// See `browser/hns_content_browser_client.cc` for more information.
class SkusRenderFrameObserver : public content::RenderFrameObserver {
 public:
  explicit SkusRenderFrameObserver(content::RenderFrame* render_frame);
  SkusRenderFrameObserver(const SkusRenderFrameObserver&) = delete;
  SkusRenderFrameObserver& operator=(const SkusRenderFrameObserver&) = delete;
  ~SkusRenderFrameObserver() override;

  // RenderFrameObserver implementation.
  void DidClearWindowObject() override;

 private:
  // RenderFrameObserver implementation.
  void OnDestruct() override;

  bool IsAllowed();
};

}  // namespace skus

#endif  // HNS_COMPONENTS_SKUS_RENDERER_SKUS_RENDER_FRAME_OBSERVER_H_
