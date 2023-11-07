/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/renderer/hns_wallet/hns_wallet_render_frame_observer_p3a_util.h"

#include "hns/components/hns_wallet/renderer/v8_helper.h"
#include "gin/converter.h"
#include "third_party/blink/public/common/browser_interface_broker_proxy.h"
#include "third_party/blink/public/web/blink.h"
#include "third_party/blink/public/web/web_local_frame.h"
#include "v8/include/v8.h"

namespace hns_wallet {

namespace {

const char kEthereumProviderObjectKey[] = "ethereum";
const char kSolanaProviderObjectKey[] = "solana";
const char kIsHnsWalletPropertyName[] = "isHnsWallet";

}  // namespace

HnsWalletRenderFrameObserverP3AUtil::HnsWalletRenderFrameObserverP3AUtil() {
}
HnsWalletRenderFrameObserverP3AUtil::
    ~HnsWalletRenderFrameObserverP3AUtil() {}

void HnsWalletRenderFrameObserverP3AUtil::ReportJSProviders(
    content::RenderFrame* render_frame,
    const hns::mojom::DynamicParams& dynamic_params) {
  if (!EnsureConnected(render_frame)) {
    return;
  }

  v8::Isolate* isolate = blink::MainThreadIsolate();
  v8::HandleScope handle_scope(isolate);
  auto* web_frame = render_frame->GetWebFrame();
  v8::Local<v8::Context> context = web_frame->MainWorldScriptContext();
  if (context.IsEmpty()) {
    return;
  }
  v8::MicrotasksScope microtasks(isolate, context->GetMicrotaskQueue(),
                                 v8::MicrotasksScope::kDoNotRunMicrotasks);

  ReportJSProvider(isolate, context, mojom::CoinType::ETH,
                   kEthereumProviderObjectKey,
                   dynamic_params.allow_overwrite_window_ethereum_provider);
  ReportJSProvider(isolate, context, mojom::CoinType::SOL,
                   kSolanaProviderObjectKey,
                   dynamic_params.allow_overwrite_window_solana_provider);
}

void HnsWalletRenderFrameObserverP3AUtil::ReportJSProvider(
    v8::Isolate* isolate,
    v8::Local<v8::Context>& context,
    mojom::CoinType coin_type,
    const char* provider_object_key,
    bool allow_provider_overwrite) {
  v8::Local<v8::Value> provider_value;
  v8::Local<v8::Object> provider_obj;
  mojom::JSProviderType provider_type = mojom::JSProviderType::None;
  if (context->Global()
          ->Get(context, gin::StringToV8(isolate, provider_object_key))
          .ToLocal(&provider_value) &&
      provider_value->IsObject() &&
      provider_value->ToObject(context).ToLocal(&provider_obj)) {
    v8::Local<v8::Value> is_hns_wallet;

    if ((GetProperty(context, provider_obj, kIsHnsWalletPropertyName)
             .ToLocal(&is_hns_wallet) &&
         is_hns_wallet->BooleanValue(isolate))) {
      provider_type = mojom::JSProviderType::Native;
    } else {
      provider_type = mojom::JSProviderType::ThirdParty;
    }
  }
  hns_wallet_p3a_->ReportJSProvider(provider_type, coin_type,
                                      allow_provider_overwrite);
}

bool HnsWalletRenderFrameObserverP3AUtil::EnsureConnected(
    content::RenderFrame* render_frame) {
  if (!hns_wallet_p3a_.is_bound()) {
    render_frame->GetBrowserInterfaceBroker()->GetInterface(
        hns_wallet_p3a_.BindNewPipeAndPassReceiver());
  }
  return hns_wallet_p3a_.is_bound();
}

}  // namespace hns_wallet
