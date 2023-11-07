/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_HNS_THIRD_PARTY_BITCOIN_CORE_SRC_SRC_SERIALIZE_H_
#define HNS_CHROMIUM_SRC_HNS_THIRD_PARTY_BITCOIN_CORE_SRC_SRC_SERIALIZE_H_

#include <ios>
#include <streambuf>
#include <string>
#include <vector>

#include "base/check.h"

namespace std {
namespace hns {
using string = ::std::string;
}
}  // namespace std

#define throw CHECK(false) <<
#define ios_base hns
#define failure string
#include "src/hns/third_party/bitcoin-core/src/src/serialize.h"  // IWYU pragma: export
#undef throw
#undef ios_base
#undef string

#endif  // HNS_CHROMIUM_SRC_HNS_THIRD_PARTY_BITCOIN_CORE_SRC_SRC_SERIALIZE_H_
