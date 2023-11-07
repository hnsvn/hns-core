/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/model/purchase_intent_funnel_keyword_info.h"

#include <utility>

namespace hns_ads {

PurchaseIntentFunnelKeywordInfo::PurchaseIntentFunnelKeywordInfo() = default;

PurchaseIntentFunnelKeywordInfo::PurchaseIntentFunnelKeywordInfo(
    std::string keywords,
    const uint16_t weight)
    : keywords(std::move(keywords)), weight(weight) {}

}  // namespace hns_ads
