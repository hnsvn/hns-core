/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_service/status_tray/ras/ras_utils.h"

#include "base/strings/utf_string_conversions.h"
#include "hns/components/hns_vpn/browser/connection/ikev2/win/ras_utils.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "chrome/common/channel_info.h"

namespace hns_vpn {
namespace ras {

namespace {
std::wstring GetConnectionEntryName() {
  return base::UTF8ToWide(GetHnsVPNEntryName(chrome::GetChannel()));
}
}  // namespace

bool IsRasConnected() {
  return (ras::CheckConnection(GetConnectionEntryName()) ==
          ras::CheckConnectionResult::CONNECTED);
}

bool ConnectRasEntry() {
  return ConnectEntry(GetConnectionEntryName()).success;
}

bool DisconnectRasEntry() {
  return DisconnectEntry(GetConnectionEntryName()).success;
}

}  // namespace ras
}  // namespace hns_vpn
