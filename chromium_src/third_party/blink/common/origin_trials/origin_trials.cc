/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/public/common/origin_trials/origin_trials.h"

#include "base/containers/contains.h"

namespace blink {
namespace origin_trials {
bool IsTrialValid_ChromiumImpl(base::StringPiece trial_name);
}  // namespace origin_trials
}  // namespace blink

#define IsTrialValid IsTrialValid_ChromiumImpl
#include "../gen/third_party/blink/common/origin_trials/origin_trials.cc"
#undef IsTrialValid

namespace blink {
namespace origin_trials {

bool IsTrialDisabledInHns(base::StringPiece trial_name) {
  // When updating also update the array in the overload below.
  // clang-format off
  static const char* const kHnsDisabledTrialNames[] = {
      "AdInterestGroupAPI",
      "DeviceAttributes",
      "DigitalGoodsV2",
      "InterestCohortAPI",
      "FencedFrames",
      "Fledge",
      "Parakeet",
      "PrivacySandboxAdsAPIs",
      "SignedExchangeSubresourcePrefetch",
      "SubresourceWebBundles",
      "TrustTokens",
      "WebEnvironmentIntegrity",
  };
  // clang-format on

  if (base::Contains(kHnsDisabledTrialNames, trial_name)) {
    // Check if this is still a valid trial name in Chromium. If not, it needs
    // to be changed as in Chromium or removed.
    DCHECK(IsTrialValid_ChromiumImpl(trial_name));
    return true;
  }

  return false;
}

bool IsTrialDisabledInHns(OriginTrialFeature feature) {
  // When updating also update the array in the overload above.
  // clang-format off
  static const OriginTrialFeature kHnsDisabledTrialFeatures[] =
      {   // NOLINT
          OriginTrialFeature::kAdInterestGroupAPI,
          OriginTrialFeature::kDeviceAttributes,
          OriginTrialFeature::kDigitalGoods,
          OriginTrialFeature::kFencedFrames,
          OriginTrialFeature::kFledge,
          OriginTrialFeature::kParakeet,
          OriginTrialFeature::kPrivacySandboxAdsAPIs,
          OriginTrialFeature::kPrivateStateTokens,
          OriginTrialFeature::kWebEnvironmentIntegrity,
      };
  // clang-format on

  return base::Contains(kHnsDisabledTrialFeatures, feature);
}

bool IsTrialValid(base::StringPiece trial_name) {
  if (IsTrialDisabledInHns(trial_name))
    return false;

  return IsTrialValid_ChromiumImpl(trial_name);
}

}  // namespace origin_trials
}  // namespace blink