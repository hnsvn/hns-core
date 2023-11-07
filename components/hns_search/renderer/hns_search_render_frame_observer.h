// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SEARCH_RENDERER_HNS_SEARCH_RENDER_FRAME_OBSERVER_H_
#define HNS_COMPONENTS_HNS_SEARCH_RENDERER_HNS_SEARCH_RENDER_FRAME_OBSERVER_H_

#include <memory>

#include "hns/components/hns_search/renderer/hns_search_default_js_handler.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "third_party/blink/public/web/web_navigation_type.h"
#include "v8/include/v8.h"

namespace hns_search {

class HnsSearchRenderFrameObserver : public content::RenderFrameObserver {
 public:
  explicit HnsSearchRenderFrameObserver(content::RenderFrame* render_frame,
                                          int32_t world_id);
  HnsSearchRenderFrameObserver(const HnsSearchRenderFrameObserver&) =
      delete;
  HnsSearchRenderFrameObserver& operator=(
      const HnsSearchRenderFrameObserver&) = delete;
  ~HnsSearchRenderFrameObserver() override;

  // RenderFrameObserver implementation.
  void DidCreateScriptContext(v8::Local<v8::Context> context,
                              int32_t world_id) override;
  void DidStartNavigation(
      const GURL& url,
      absl::optional<blink::WebNavigationType> navigation_type) override;

 private:
  // RenderFrameObserver implementation.
  void OnDestruct() override;

  // Handle to "handler" JavaScript object functionality.
  std::unique_ptr<HnsSearchDefaultJSHandler> native_javascript_handle_;
  int32_t world_id_;
  GURL url_;
};

}  // namespace hns_search

#endif  // HNS_COMPONENTS_HNS_SEARCH_RENDERER_HNS_SEARCH_RENDER_FRAME_OBSERVER_H_
