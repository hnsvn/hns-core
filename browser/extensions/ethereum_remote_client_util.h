/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_ETHEREUM_REMOTE_CLIENT_UTIL_H_
#define HNS_BROWSER_EXTENSIONS_ETHEREUM_REMOTE_CLIENT_UTIL_H_

#include <string>

namespace extensions {

std::string GetInfuraProjectID();

std::string GetHnsKey();

bool HasInfuraProjectID();

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_ETHEREUM_REMOTE_CLIENT_UTIL_H_
