/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_RENDERER_HNS_RENDER_THREAD_OBSERVER_H_
#define HNS_RENDERER_HNS_RENDER_THREAD_OBSERVER_H_

#include "hns/common/hns_renderer_configuration.mojom.h"
#include "content/public/renderer/render_thread_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace blink {
class AssociatedInterfaceRegistry;
}

class HnsRenderThreadObserver
    : public content::RenderThreadObserver,
      public hns::mojom::HnsRendererConfiguration {
 public:
  HnsRenderThreadObserver(const HnsRenderThreadObserver&) = delete;
  HnsRenderThreadObserver& operator=(const HnsRenderThreadObserver&) =
      delete;
  HnsRenderThreadObserver();
  ~HnsRenderThreadObserver() override;

  bool is_tor_process() const { return is_tor_process_; }

  // Return the dynamic parameters - those that may change while the
  // render process is running.
  static const hns::mojom::DynamicParams& GetDynamicParams();

 private:
  // content::RenderThreadObserver:
  void RegisterMojoInterfaces(
      blink::AssociatedInterfaceRegistry* associated_interfaces) override;
  void UnregisterMojoInterfaces(
      blink::AssociatedInterfaceRegistry* associated_interfaces) override;

  // hns::mojom::HnsRendererConfiguration:
  void SetInitialConfiguration(bool is_tor_process) override;
  void SetConfiguration(hns::mojom::DynamicParamsPtr params) override;

  void OnRendererConfigurationAssociatedRequest(
      mojo::PendingAssociatedReceiver<hns::mojom::HnsRendererConfiguration>
          receiver);

  bool is_tor_process_ = false;
  mojo::AssociatedReceiverSet<hns::mojom::HnsRendererConfiguration>
      renderer_configuration_receivers_;
};

#endif  // HNS_RENDERER_HNS_RENDER_THREAD_OBSERVER_H_
