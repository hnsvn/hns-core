/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_PREF_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_PREF_INFO_H_

#include <string>

#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct PrefInfo final {
  PrefInfo();

  PrefInfo(const PrefInfo&);
  PrefInfo& operator=(const PrefInfo&);

  PrefInfo(PrefInfo&&) noexcept;
  PrefInfo& operator=(PrefInfo&&) noexcept;

  ~PrefInfo();

  absl::optional<std::string> value;
  std::string default_value;
};

bool operator==(const PrefInfo&, const PrefInfo&);
bool operator!=(const PrefInfo&, const PrefInfo&);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_PREF_INFO_H_
