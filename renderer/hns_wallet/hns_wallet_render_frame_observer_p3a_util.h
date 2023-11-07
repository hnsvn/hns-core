/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_RENDERER_HNS_WALLET_HNS_WALLET_RENDER_FRAME_OBSERVER_P3A_UTIL_H_
#define HNS_RENDERER_HNS_WALLET_HNS_WALLET_RENDER_FRAME_OBSERVER_P3A_UTIL_H_

#include "hns/common/hns_renderer_configuration.mojom.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "content/public/renderer/render_frame.h"
#include "mojo/public/cpp/bindings/remote.h"

namespace v8 {
class Context;
template <class T>
class Local;
class Isolate;
}  // namespace v8

namespace hns_wallet {

class HnsWalletRenderFrameObserverP3AUtil {
 public:
  HnsWalletRenderFrameObserverP3AUtil();
  ~HnsWalletRenderFrameObserverP3AUtil();

  void ReportJSProviders(content::RenderFrame* render_frame,
                         const hns::mojom::DynamicParams& dynamic_params);

 private:
  bool EnsureConnected(content::RenderFrame* render_frame);

  void ReportJSProvider(v8::Isolate* isolate,
                        v8::Local<v8::Context>& context,
                        mojom::CoinType coin_type,
                        const char* provider_object_key,
                        bool allow_provider_overwrite);

  mojo::Remote<hns_wallet::mojom::HnsWalletP3A> hns_wallet_p3a_;
};

}  // namespace hns_wallet

#endif  // HNS_RENDERER_HNS_WALLET_HNS_WALLET_RENDER_FRAME_OBSERVER_P3A_UTIL_H_
