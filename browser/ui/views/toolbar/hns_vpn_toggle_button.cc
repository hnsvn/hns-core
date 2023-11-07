/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/toolbar/hns_vpn_toggle_button.h"

#include <utility>

#include "base/functional/bind.h"
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/color/color_id.h"

using ConnectionState = hns_vpn::mojom::ConnectionState;
using PurchasedState = hns_vpn::mojom::PurchasedState;

HnsVPNToggleButton::HnsVPNToggleButton(Browser* browser)
    : browser_(browser),
      service_(hns_vpn::HnsVpnServiceFactory::GetForProfile(
          browser_->profile())) {
  DCHECK(service_);

  Observe(service_);

  SetCallback(base::BindRepeating(&HnsVPNToggleButton::OnButtonPressed,
                                  base::Unretained(this)));
  UpdateState();

  if (const ui::ColorProvider* provider =
          BrowserView::GetBrowserViewForBrowser(browser_)->GetColorProvider()) {
    SetThumbOnColor(provider->GetColor(ui::kColorToggleButtonThumbOn));
    SetThumbOffColor(provider->GetColor(ui::kColorToggleButtonThumbOff));
    SetTrackOnColor(provider->GetColor(ui::kColorToggleButtonTrackOn));
    SetTrackOffColor(provider->GetColor(ui::kColorToggleButtonTrackOff));
  }

  // TODO(simonhong): Re-visit this name.
  SetAccessibleName(hns_l10n::GetLocalizedResourceUTF16String(
      IDS_HNS_VPN_TOGGLE_MENU_ITEM_TEXT));
}

HnsVPNToggleButton::~HnsVPNToggleButton() = default;

void HnsVPNToggleButton::OnConnectionStateChanged(ConnectionState state) {
  UpdateState();
}

void HnsVPNToggleButton::OnButtonPressed(const ui::Event& event) {
  service_->ToggleConnection();
}

void HnsVPNToggleButton::UpdateState() {
  const auto state = service_->GetConnectionState();
  bool is_on = (state == ConnectionState::CONNECTING ||
                state == ConnectionState::CONNECTED);
  SetIsOn(is_on);
}
