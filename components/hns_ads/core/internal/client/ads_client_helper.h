/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CLIENT_ADS_CLIENT_HELPER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CLIENT_ADS_CLIENT_HELPER_H_

#include "hns/components/hns_ads/core/public/client/ads_client.h"

namespace hns_ads {

class AdsClientNotifierObserver;

class AdsClientHelper final {
 public:
  AdsClientHelper() = default;

  AdsClientHelper(const AdsClientHelper&) = delete;
  AdsClientHelper& operator=(const AdsClientHelper&) = delete;

  AdsClientHelper(AdsClientHelper&&) noexcept = delete;
  AdsClientHelper& operator=(AdsClientHelper&&) noexcept = delete;

  ~AdsClientHelper() = default;

  static AdsClient* GetInstance();

  static bool HasInstance();

  static void AddObserver(AdsClientNotifierObserver* observer);
  static void RemoveObserver(AdsClientNotifierObserver* observer);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CLIENT_ADS_CLIENT_HELPER_H_
