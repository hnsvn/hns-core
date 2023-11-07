/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_SPONSORED_IMAGES_COMPONENT_DATA_H_
#define HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_SPONSORED_IMAGES_COMPONENT_DATA_H_

#include <string>

#include "base/strings/string_piece.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace ntp_background_images {

struct SponsoredImagesComponentData {
  base::StringPiece region;
  base::StringPiece component_base64_public_key;
  base::StringPiece component_id;
};

absl::optional<SponsoredImagesComponentData> GetSponsoredImagesComponentData(
    const std::string& region);

}  // namespace ntp_background_images

#endif  // HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_SPONSORED_IMAGES_COMPONENT_DATA_H_
