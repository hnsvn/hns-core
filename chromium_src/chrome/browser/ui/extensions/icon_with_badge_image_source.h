/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns {
class HnsIconWithBadgeImageSource;
}

#define HNS_ICON_WITH_BADGE_IMAGE_SOURCE_H_            \
 private:                                                \
  friend class hns::HnsIconWithBadgeImageSource;     \
  virtual absl::optional<int> GetCustomGraphicSize();    \
  virtual absl::optional<int> GetCustomGraphicXOffset(); \
  virtual absl::optional<int> GetCustomGraphicYOffset(); \
                                                         \
 public:                                                 \
  // #define HNS_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#define PaintBadge virtual PaintBadge
#define GetIconAreaRect virtual GetIconAreaRect
#include "src/chrome/browser/ui/extensions/icon_with_badge_image_source.h"  // IWYU pragma: export
#undef GetIconAreaRect
#undef PaintBadge
#undef HNS_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_
