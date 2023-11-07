/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_builder_unittest_util.h"

#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_info.h"

namespace hns_ads {

UserModelInfo BuildUserModelForTesting(
    const SegmentList& intent_segments,
    const SegmentList& latent_interest_segments,
    const SegmentList& interest_segments,
    const TextEmbeddingHtmlEventList& text_embedding_html_events) {
  UserModelInfo user_model;

  user_model.intent_segments = intent_segments;
  user_model.latent_interest_segments = latent_interest_segments;
  user_model.interest_segments = interest_segments;
  user_model.text_embedding_html_events = text_embedding_html_events;

  return user_model;
}

}  // namespace hns_ads
