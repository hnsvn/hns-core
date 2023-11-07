/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/crash/core/app/crashpad.h"

#if BUILDFLAG(IS_WIN)
namespace {
// Split into two places to avoid patching:
// components\hns_vpn\browser\connection\win\hns_vpn_helper\hns_vpn_helper_crash_reporter_client.cc
// // NOLINT Need keep it in sync
constexpr char kHnsVPNHelperProcessType[] = "hns-vpn-helper";

// Split into two places to avoid patching:
// components\hns_vpn\browser\connection\wireguard\win\hns_vpn_wireguard_service\hns_wireguard_service_crash_reporter_client.cc
// // NOLINT Need keep it in sync
constexpr char kHnsWireguardProcessType[] = "hns-vpn-wireguard-service";
}  // namespace

#define HNS_INITIALIZE_CRASHPAD_IMPL_PROCESS_TYPE \
  process_type == kHnsVPNHelperProcessType ||     \
      process_type == kHnsWireguardProcessType ||
#endif

#include "src/components/crash/core/app/crashpad.cc"
#if BUILDFLAG(IS_WIN)
#undef HNS_INITIALIZE_CRASHPAD_IMPL_PROCESS_TYPE
#endif
