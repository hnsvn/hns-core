/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_STUDIES_STUDIES_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_STUDIES_STUDIES_H_

#include "hns/components/hns_ads/core/public/client/ads_client_notifier_observer.h"

namespace hns_ads {

class Studies final : public AdsClientNotifierObserver {
 public:
  Studies();

  Studies(const Studies&) = delete;
  Studies& operator=(const Studies&) = delete;

  Studies(Studies&&) noexcept = delete;
  Studies& operator=(Studies&&) noexcept = delete;

  ~Studies() override;

 private:
  // AdsClientNotifierObserver:
  void OnNotifyDidInitializeAds() override;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_STUDIES_STUDIES_H_
