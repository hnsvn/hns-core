/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_WIN_UTILS_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_WIN_UTILS_H_

#include <wrl/client.h>
#include <string>

#include "base/win/windows_types.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_vpn {
HRESULT HRESULTFromLastError();
absl::optional<DWORD> GetWindowsServiceStatus(const std::wstring& service_name);
bool IsWindowsServiceRunning(const std::wstring& service_name);
bool SetServiceFailureActions(SC_HANDLE service);
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_WIN_UTILS_H_
