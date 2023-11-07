/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_PLATFORM_PLATFORM_HELPER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_PLATFORM_PLATFORM_HELPER_H_

#include <string>

#include "base/no_destructor.h"
#include "hns/components/hns_ads/core/internal/common/platform/platform_helper_types.h"

namespace hns_ads {

class PlatformHelper {
 public:
  virtual ~PlatformHelper();

  static const PlatformHelper& GetInstance();

  static void SetForTesting(const PlatformHelper* platform_helper);

  virtual bool IsMobile() const;
  virtual std::string GetName() const;
  virtual PlatformType GetType() const;

 protected:
  friend class base::NoDestructor<PlatformHelper>;

  PlatformHelper();

 private:
  static const PlatformHelper& GetInstanceImpl();
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_PLATFORM_PLATFORM_HELPER_H_
