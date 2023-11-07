/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/renderer/hns_render_thread_observer.h"

#include <utility>

#include "base/logging.h"
#include "base/no_destructor.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"

namespace {

hns::mojom::DynamicParams* GetDynamicConfigParams() {
  static base::NoDestructor<hns::mojom::DynamicParams> dynamic_params;
  return dynamic_params.get();
}

}  // namespace

HnsRenderThreadObserver::HnsRenderThreadObserver() = default;

HnsRenderThreadObserver::~HnsRenderThreadObserver() = default;

// static
const hns::mojom::DynamicParams&
HnsRenderThreadObserver::GetDynamicParams() {
  return *GetDynamicConfigParams();
}

void HnsRenderThreadObserver::RegisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->AddInterface<hns::mojom::HnsRendererConfiguration>(
      base::BindRepeating(
          &HnsRenderThreadObserver::OnRendererConfigurationAssociatedRequest,
          base::Unretained(this)));
}

void HnsRenderThreadObserver::UnregisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->RemoveInterface(
      hns::mojom::HnsRendererConfiguration::Name_);
}

void HnsRenderThreadObserver::OnRendererConfigurationAssociatedRequest(
    mojo::PendingAssociatedReceiver<hns::mojom::HnsRendererConfiguration>
        receiver) {
  renderer_configuration_receivers_.Add(this, std::move(receiver));
}

void HnsRenderThreadObserver::SetInitialConfiguration(bool is_tor_process) {
  is_tor_process_ = is_tor_process;
}

void HnsRenderThreadObserver::SetConfiguration(
    hns::mojom::DynamicParamsPtr params) {
  *GetDynamicConfigParams() = std::move(*params);
}
