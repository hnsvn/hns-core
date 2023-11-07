/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/toolbar/hns_vpn_status_label.h"

#include <utility>

#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/browser/ui/color/hns_color_id.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_view.h"

using ConnectionState = hns_vpn::mojom::ConnectionState;
using PurchasedState = hns_vpn::mojom::PurchasedState;

namespace {

int GetStringIdForConnectionState(ConnectionState state) {
  switch (state) {
    case ConnectionState::CONNECTING:
      return IDS_HNS_VPN_STATUS_LABEL_CONNECTING;
    case ConnectionState::CONNECTED:
      return IDS_HNS_VPN_STATUS_LABEL_CONNECTED;
    case ConnectionState::DISCONNECTING:
      return IDS_HNS_VPN_STATUS_LABEL_DISCONNECTING;
    default:
      break;
  }
  return IDS_HNS_VPN_STATUS_LABEL_DISCONNECTED;
}

int GetLongestStringIdForConnectionState() {
  size_t max = 0;
  int longest_string_id =
      GetStringIdForConnectionState(ConnectionState::DISCONNECTED);
  for (auto state :
       {ConnectionState::CONNECTING, ConnectionState::CONNECTED,
        ConnectionState::DISCONNECTING, ConnectionState::DISCONNECTED}) {
    auto id = GetStringIdForConnectionState(state);
    auto text = hns_l10n::GetLocalizedResourceUTF16String(id);
    if (text.length() > max) {
      max = text.length();
      longest_string_id = id;
    }
  }
  return longest_string_id;
}
}  // namespace

HnsVPNStatusLabel::HnsVPNStatusLabel(Browser* browser)
    : browser_(browser),
      service_(hns_vpn::HnsVpnServiceFactory::GetForProfile(
          browser_->profile())) {
  DCHECK(service_);

  Observe(service_);
  SetAutoColorReadabilityEnabled(false);
  UpdateState();

  if (const ui::ColorProvider* provider =
          BrowserView::GetBrowserViewForBrowser(browser_)->GetColorProvider()) {
    SetEnabledColor(provider->GetColor(kColorMenuItemSubText));
  }
  longest_state_string_id_ = GetLongestStringIdForConnectionState();
}

HnsVPNStatusLabel::~HnsVPNStatusLabel() = default;

void HnsVPNStatusLabel::OnConnectionStateChanged(ConnectionState state) {
  UpdateState();
}

gfx::Size HnsVPNStatusLabel::CalculatePreferredSize() const {
  auto size = views::Label::CalculatePreferredSize();
  if (longest_state_string_id_ == -1)
    return size;
  auto text =
      hns_l10n::GetLocalizedResourceUTF16String(longest_state_string_id_);
  if (text == GetText())
    return size;
  size.set_width(font_list().GetExpectedTextWidth(text.length()) +
                 GetInsets().width());
  size.set_height(GetHeightForWidth(size.width()));
  return size;
}

void HnsVPNStatusLabel::UpdateState() {
  const auto state = service_->GetConnectionState();

  SetText(hns_l10n::GetLocalizedResourceUTF16String(
      GetStringIdForConnectionState(state)));
}
