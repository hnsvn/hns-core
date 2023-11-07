/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_USER_MODEL_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_USER_MODEL_INFO_H_

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_event_info.h"

namespace hns_ads {

struct UserModelInfo final {
  UserModelInfo();

  UserModelInfo(const UserModelInfo&);
  UserModelInfo& operator=(const UserModelInfo&);

  UserModelInfo(UserModelInfo&&) noexcept;
  UserModelInfo& operator=(UserModelInfo&&) noexcept;

  ~UserModelInfo();

  SegmentList intent_segments;
  SegmentList latent_interest_segments;
  SegmentList interest_segments;
  TextEmbeddingHtmlEventList text_embedding_html_events;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_USER_MODEL_INFO_H_
