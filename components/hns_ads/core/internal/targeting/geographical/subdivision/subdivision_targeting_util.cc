/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting_util.h"

#include "base/containers/contains.h"
#include "base/strings/string_piece.h"
#include "hns/components/hns_ads/core/public/targeting/geographical/subdivision/supported_subdivisions.h"

namespace hns_ads {

bool ShouldTargetSubdivisionCountryCode(const std::string& country_code) {
  return base::Contains(GetSupportedSubdivisions(), country_code);
}

bool ShouldTargetSubdivision(const std::string& country_code,
                             const std::string& subdivision) {
  const auto iter = GetSupportedSubdivisions().find(country_code);
  if (iter == GetSupportedSubdivisions().cend()) {
    return false;
  }

  const auto& [_, subdivisions] = *iter;

  return base::Contains(subdivisions, subdivision);
}

}  // namespace hns_ads
