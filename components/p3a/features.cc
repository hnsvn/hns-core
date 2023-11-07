/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/p3a/features.h"

namespace p3a {
namespace features {

BASE_FEATURE(kConstellation,
             "HnsP3AConstellation",
             base::FEATURE_DISABLED_BY_DEFAULT);
BASE_FEATURE(kConstellationEnclaveAttestation,
             "HnsP3AConstellationEnclaveAttestation",
             base::FEATURE_DISABLED_BY_DEFAULT);

bool IsConstellationEnabled() {
  return base::FeatureList::IsEnabled(features::kConstellation);
}

bool IsConstellationEnclaveAttestationEnabled() {
  return base::FeatureList::IsEnabled(
      features::kConstellationEnclaveAttestation);
}

}  // namespace features
}  // namespace p3a
