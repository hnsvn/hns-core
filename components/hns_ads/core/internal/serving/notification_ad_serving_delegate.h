/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_NOTIFICATION_AD_SERVING_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_NOTIFICATION_AD_SERVING_DELEGATE_H_

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads {

struct NotificationAdInfo;

class NotificationAdServingDelegate {
 public:
  // Invoked when an opportunity arises to serve a notification ad for the
  // |segments|.
  virtual void OnOpportunityAroseToServeNotificationAd(
      const SegmentList& segments) {}

  // Invoked when a notification ad is served.
  virtual void OnDidServeNotificationAd(const NotificationAdInfo& ad) {}

  // Invoked when a notification ad fails to serve.
  virtual void OnFailedToServeNotificationAd() {}

 protected:
  virtual ~NotificationAdServingDelegate() = default;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_NOTIFICATION_AD_SERVING_DELEGATE_H_
