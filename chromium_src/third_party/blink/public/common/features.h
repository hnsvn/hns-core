/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_FEATURES_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_FEATURES_H_

#include "src/third_party/blink/public/common/features.h"  // IWYU pragma: export

#include <string>

namespace blink {
namespace features {

BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kAllowCertainClientHints);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kClampPlatformVersionClientHint);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kFileSystemAccessAPI);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kHnsWebBluetoothAPI);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kHnsWebSerialAPI);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kNavigatorConnectionAttribute);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kPartitionBlinkMemoryCache);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kRestrictWebSocketsPool);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kHnsBlockScreenFingerprinting);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kHnsGlobalPrivacyControl);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kHnsRoundTimeStamps);
BLINK_COMMON_EXPORT BASE_DECLARE_FEATURE(kRestrictEventSourcePool);

// Specifies the value of the third component of the dotted version of the
// platform version client hint.
BLINK_COMMON_EXPORT extern const base::FeatureParam<std::string>
    kClampPlatformVersionClientHintPatchValue;

// Chromium used this function to control Prerender2 feature, but then the
// feature was permanently enabled and the function was removed. We still want
// to keep the Prerender2 functionality disabled, so putting back the function
// to use in various places where Prerender2 needs to be turned off.
BLINK_COMMON_EXPORT bool IsPrerender2Enabled();

}  // namespace features
}  // namespace blink

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_COMMON_FEATURES_H_
