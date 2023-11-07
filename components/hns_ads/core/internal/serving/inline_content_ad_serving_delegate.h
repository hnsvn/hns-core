/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_INLINE_CONTENT_AD_SERVING_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_INLINE_CONTENT_AD_SERVING_DELEGATE_H_

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads {

struct InlineContentAdInfo;

class InlineContentAdServingDelegate {
 public:
  // Invoked when an opportunity arises to serve an inline content ad for the
  // |segments|.
  virtual void OnOpportunityAroseToServeInlineContentAd(
      const SegmentList& segments) {}

  // Invoked when an inline content ad is served.
  virtual void OnDidServeInlineContentAd(const InlineContentAdInfo& ad) {}

  // Invoked when an inline content ad fails to serve.
  virtual void OnFailedToServeInlineContentAd() {}

 protected:
  virtual ~InlineContentAdServingDelegate() = default;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_INLINE_CONTENT_AD_SERVING_DELEGATE_H_
