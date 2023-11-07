/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/hns_vpn_service_observer.h"

#include <utility>

#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"

namespace hns_vpn {

HnsVPNServiceObserver::HnsVPNServiceObserver() = default;

HnsVPNServiceObserver::~HnsVPNServiceObserver() = default;

void HnsVPNServiceObserver::Observe(HnsVpnService* service) {
  if (!service)
    return;

  if (service->IsHnsVPNEnabled()) {
    mojo::PendingRemote<mojom::ServiceObserver> listener;
    receiver_.Bind(listener.InitWithNewPipeAndPassReceiver());
    service->AddObserver(std::move(listener));
  }
}

}  // namespace hns_vpn
