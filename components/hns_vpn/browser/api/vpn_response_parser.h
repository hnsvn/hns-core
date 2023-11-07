/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_API_VPN_RESPONSE_PARSER_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_API_VPN_RESPONSE_PARSER_H_

#include <string>

namespace hns_vpn {

std::string ParseSubscriberCredentialFromJson(const std::string& json,
                                              std::string* error);

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_API_VPN_RESPONSE_PARSER_H_
