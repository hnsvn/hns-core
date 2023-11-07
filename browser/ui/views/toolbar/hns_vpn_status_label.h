/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_STATUS_LABEL_H_
#define HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_STATUS_LABEL_H_

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service_observer.h"
#include "ui/views/controls/label.h"

namespace hns_vpn {
class HnsVpnService;
}  // namespace hns_vpn

class Browser;

class HnsVPNStatusLabel : public views::Label,
                            public hns_vpn::HnsVPNServiceObserver {
 public:
  explicit HnsVPNStatusLabel(Browser* browser);
  ~HnsVPNStatusLabel() override;

  HnsVPNStatusLabel(const HnsVPNStatusLabel&) = delete;
  HnsVPNStatusLabel& operator=(const HnsVPNStatusLabel&) = delete;

  gfx::Size CalculatePreferredSize() const override;

 private:
  // hns_vpn::HnsVPNServiceObserver overrides:
  void OnConnectionStateChanged(
      hns_vpn::mojom::ConnectionState state) override;

  void UpdateState();

  int longest_state_string_id_ = -1;
  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<hns_vpn::HnsVpnService> service_ = nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_VPN_STATUS_LABEL_H_
