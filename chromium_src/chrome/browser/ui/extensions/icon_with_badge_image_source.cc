/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define HNS_ICON_WITH_BADGE_IMAGE_SOURCE_DRAW_1 \
  true ? ScaleImageSkiaRep(rep,                             \
                           GetCustomGraphicSize().value_or( \
                               extensions::ExtensionAction::ActionIconSize()), \
                           canvas->image_scale()) :

#define HNS_ICON_WITH_BADGE_IMAGE_SOURCE_DRAW_2 \
  if (GetCustomGraphicXOffset().has_value())      \
    x_offset = GetCustomGraphicXOffset().value(); \
  if (GetCustomGraphicYOffset().has_value())      \
    y_offset = GetCustomGraphicYOffset().value();

#include "src/chrome/browser/ui/extensions/icon_with_badge_image_source.cc"
#undef HNS_ICON_WITH_BADGE_IMAGE_SOURCE_DRAW_2
#undef HNS_ICON_WITH_BADGE_IMAGE_SOURCE_DRAW_1

#include "third_party/abseil-cpp/absl/types/optional.h"

// Implement default virtual methods
absl::optional<int> IconWithBadgeImageSource::GetCustomGraphicSize() {
  return absl::nullopt;
}

absl::optional<int> IconWithBadgeImageSource::GetCustomGraphicXOffset() {
  return absl::nullopt;
}

absl::optional<int> IconWithBadgeImageSource::GetCustomGraphicYOffset() {
  return absl::nullopt;
}
