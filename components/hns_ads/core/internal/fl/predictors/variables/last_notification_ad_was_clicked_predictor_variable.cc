/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/last_notification_ad_was_clicked_predictor_variable.h"

#include "base/strings/string_number_conversions.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/fl/predictors/variables/predictor_variable_constants.h"
#include "hns/components/hns_ads/core/internal/history/history_manager.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"

namespace hns_ads {

namespace {

constexpr int kClickedValue = 1;
constexpr int kNotClickedValue = 0;
constexpr base::TimeDelta kTimeWindow = base::Days(7);

}  // namespace

hns_federated::mojom::DataType
LastNotificationAdWasClickedPredictorVariable::GetDataType() const {
  return hns_federated::mojom::DataType::kBool;
}

hns_federated::mojom::CovariateType
LastNotificationAdWasClickedPredictorVariable::GetType() const {
  return hns_federated::mojom::CovariateType::kLastNotificationAdWasClicked;
}

std::string LastNotificationAdWasClickedPredictorVariable::GetValue() const {
  const base::Time now = base::Time::Now();
  const base::Time from_time = now - kTimeWindow;
  const base::Time to_time = now;

  const HistoryItemList history_items = HistoryManager::Get(
      HistoryFilterType::kNone, HistorySortType::kDescendingOrder, from_time,
      to_time);

  const auto iter = base::ranges::find_if(
      history_items, [](const HistoryItemInfo& history_item) {
        return history_item.ad_content.type == AdType::kNotificationAd;
      });

  if (iter == history_items.cend()) {
    return base::NumberToString(kPredictorVariableMissingValue);
  }

  return base::NumberToString(iter->ad_content.confirmation_type ==
                                      ConfirmationType::kClicked
                                  ? kClickedValue
                                  : kNotClickedValue);
}

}  // namespace hns_ads
