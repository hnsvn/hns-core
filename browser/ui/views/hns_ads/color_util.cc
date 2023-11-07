/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/hns_ads/color_util.h"

#include "base/strings/string_number_conversions.h"
#include "base/strings/string_piece.h"

namespace hns_ads {

bool RgbStringToSkColor(base::StringPiece rgb, SkColor* color) {
  CHECK(color);

  // Expect three RGB color components with length == 2, e.g. 42fe4c.
  constexpr size_t kColorComponentsCount = 3;
  constexpr size_t kColorComponentLen = 2;

  if (rgb.size() != kColorComponentsCount * kColorComponentLen) {
    return false;
  }

  uint32_t components[kColorComponentsCount];
  for (size_t i = 0; i < kColorComponentsCount; ++i) {
    const size_t beg = kColorComponentLen * i;
    uint32_t component = 0;
    if (!base::HexStringToUInt(rgb.substr(beg, kColorComponentLen),
                               &component)) {
      return false;
    }
    components[i] = component;
  }

  *color = SkColorSetRGB(components[0], components[1], components[2]);
  return true;
}

}  // namespace hns_ads
