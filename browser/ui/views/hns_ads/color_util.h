/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ADS_COLOR_UTIL_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ADS_COLOR_UTIL_H_

#include "base/strings/string_piece_forward.h"
#include "third_party/skia/include/core/SkColor.h"

namespace hns_ads {

bool RgbStringToSkColor(base::StringPiece rgb, SkColor* color);

}  // namespace hns_ads

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ADS_COLOR_UTIL_H_
