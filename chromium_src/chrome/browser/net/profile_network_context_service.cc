/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */
static const char* kHnsCTExcludedHosts[] = {
    // Critical endpoints that shouldn't require SCTs so they always work
    "laptop-updates.hns.com",
    "updates.hnssoftware.com",
    "updates-cdn.hnssoftware.com",
    // Test host for manual testing
    "sct-exempted.hnssoftware.com",
};

#define HNS_PROFILE_NETWORK_CONTEXT_SERVICE_GET_CT_POLICY \
  for (const auto* host : kHnsCTExcludedHosts) {          \
    excluded.push_back(host);                               \
  }

#include "src/chrome/browser/net/profile_network_context_service.cc"
#undef HNS_PROFILE_NETWORK_CONTEXT_SERVICE_GET_CT_POLICY
