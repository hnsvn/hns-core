/* Copyright 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_URL_ORIGIN_H_
#define HNS_CHROMIUM_SRC_URL_ORIGIN_H_

namespace net {
class EphemeralStorageOriginUtils;
}  // namespace net

#define GetTupleOrPrecursorTupleIfOpaque   \
  NotUsed() const;                         \
  friend net::EphemeralStorageOriginUtils; \
  const SchemeHostPort& GetTupleOrPrecursorTupleIfOpaque

#include "src/url/origin.h"  // IWYU pragma: export

#undef GetTupleOrPrecursorTupleIfOpaque

#endif  // HNS_CHROMIUM_SRC_URL_ORIGIN_H_
