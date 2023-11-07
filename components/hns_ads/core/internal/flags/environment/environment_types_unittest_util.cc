/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/flags/environment/environment_types_unittest_util.h"

#include "base/check.h"

namespace hns_ads {

namespace {

constexpr char kProductionEnvironment[] = "Production";
constexpr char kStagingEnvironment[] = "Staging";

}  // namespace

std::string EnvironmentTypeEnumToStringForTesting(
    const mojom::EnvironmentType environment_type) {
  CHECK(mojom::IsKnownEnumValue(environment_type));

  switch (environment_type) {
    case mojom::EnvironmentType::kProduction: {
      return kProductionEnvironment;
    }

    case mojom::EnvironmentType::kStaging: {
      return kStagingEnvironment;
    }
  }
}

}  // namespace hns_ads
