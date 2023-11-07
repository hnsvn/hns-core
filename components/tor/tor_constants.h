/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_TOR_TOR_CONSTANTS_H_
#define HNS_COMPONENTS_TOR_TOR_CONSTANTS_H_

#include "base/files/file_path.h"

namespace tor {

// Deprecated
extern const base::FilePath::CharType kTorProfileDir[];

constexpr char kTorProfileID[] = "Tor::Profile";

}  // namespace tor

#endif  // HNS_COMPONENTS_TOR_TOR_CONSTANTS_H_
