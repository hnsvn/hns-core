/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_RENDERER_HNS_WALLET_HNS_WALLET_RENDER_FRAME_OBSERVER_H_
#define HNS_RENDERER_HNS_WALLET_HNS_WALLET_RENDER_FRAME_OBSERVER_H_

#include "hns/common/hns_renderer_configuration.mojom.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/renderer/js_ethereum_provider.h"
#include "hns/components/hns_wallet/renderer/js_solana_provider.h"
#include "hns/renderer/hns_wallet/hns_wallet_render_frame_observer_p3a_util.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "third_party/blink/public/web/web_navigation_type.h"
#include "url/gurl.h"
#include "v8/include/v8.h"

namespace hns_wallet {

class HnsWalletRenderFrameObserver : public content::RenderFrameObserver {
 public:
  using GetDynamicParamsCallback =
      base::RepeatingCallback<const hns::mojom::DynamicParams&()>;

  explicit HnsWalletRenderFrameObserver(
      content::RenderFrame* render_frame,
      GetDynamicParamsCallback get_dynamic_params_callback);
  ~HnsWalletRenderFrameObserver() override;

  // RenderFrameObserver implementation.
  void DidStartNavigation(
      const GURL& url,
      absl::optional<blink::WebNavigationType> navigation_type) override;
  void DidClearWindowObject() override;

  void DidFinishLoad() override;

 private:
  // RenderFrameObserver implementation.
  void OnDestruct() override;

  bool IsPageValid();
  bool CanCreateProvider();

  GURL url_;
  GetDynamicParamsCallback get_dynamic_params_callback_;

  HnsWalletRenderFrameObserverP3AUtil p3a_util_;
};

}  // namespace hns_wallet

#endif  // HNS_RENDERER_HNS_WALLET_HNS_WALLET_RENDER_FRAME_OBSERVER_H_
