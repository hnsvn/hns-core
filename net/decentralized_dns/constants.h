/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_NET_DECENTRALIZED_DNS_CONSTANTS_H_
#define HNS_NET_DECENTRALIZED_DNS_CONSTANTS_H_

namespace decentralized_dns {

constexpr const char* kUnstoppableDomains[] = {
    ".crypto", ".x",          ".nft",     ".dao",
    ".wallet", ".blockchain", ".bitcoin", ".zil"};

constexpr char kEthDomain[] = ".eth";
constexpr char kDNSForEthDomain[] = ".eth.link";

constexpr char kSolDomain[] = ".sol";

}  // namespace decentralized_dns

#endif  // HNS_NET_DECENTRALIZED_DNS_CONSTANTS_H_
