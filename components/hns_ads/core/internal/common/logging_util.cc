/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/logging_util.h"

#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"

namespace hns_ads {

void Log(const char* file,
         const int line,
         const int verbose_level,
         const std::string& message) {
  if (AdsClientHelper::HasInstance()) {
    AdsClientHelper::GetInstance()->Log(file, line, verbose_level, message);
  }
}

}  // namespace hns_ads
