/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_PLATFORM_PLATFORM_HELPER_MOCK_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_PLATFORM_PLATFORM_HELPER_MOCK_H_

#include "hns/components/hns_ads/core/internal/common/platform/platform_helper.h"

#include <string>

#include "testing/gmock/include/gmock/gmock.h"

namespace hns_ads {

class PlatformHelperMock : public PlatformHelper {
 public:
  PlatformHelperMock();

  PlatformHelperMock(const PlatformHelperMock&) = delete;
  PlatformHelperMock& operator=(const PlatformHelperMock&) = delete;

  PlatformHelperMock(PlatformHelperMock&&) noexcept = delete;
  PlatformHelperMock& operator=(PlatformHelperMock&&) noexcept = delete;

  ~PlatformHelperMock() override;

  MOCK_CONST_METHOD0(IsMobile, bool());
  MOCK_CONST_METHOD0(GetName, std::string());
  MOCK_CONST_METHOD0(GetType, PlatformType());
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_PLATFORM_PLATFORM_HELPER_MOCK_H_
