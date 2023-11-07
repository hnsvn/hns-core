/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/history/history_item_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/history/ad_content_util.h"
#include "hns/components/hns_ads/core/internal/history/category_content_util.h"
#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"

namespace hns_ads {

HistoryItemInfo BuildHistoryItem(const AdInfo& ad,
                                 const ConfirmationType& confirmation_type,
                                 const std::string& title,
                                 const std::string& description) {
  HistoryItemInfo history_item;

  history_item.created_at = base::Time::Now();
  history_item.ad_content =
      BuildAdContent(ad, confirmation_type, title, description);
  history_item.category_content = BuildCategoryContent(ad.segment);

  return history_item;
}

}  // namespace hns_ads
