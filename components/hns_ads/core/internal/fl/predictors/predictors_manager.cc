/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/predictors_manager.h"

#include <utility>

#include "base/check.h"
#include "base/containers/fixed_flat_map.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/fl/predictors/variables/average_clickthrough_rate_predictor_variable.h"
#include "hns/components/hns_ads/core/internal/fl/predictors/variables/last_notification_ad_was_clicked_predictor_variable.h"
#include "hns/components/hns_ads/core/internal/fl/predictors/variables/number_of_user_activity_events_predictor_variable.h"
#include "hns/components/hns_ads/core/internal/fl/predictors/variables/time_since_last_user_activity_event_predictor_variable.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"
#include "hns/components/hns_federated/public/interfaces/hns_federated.mojom.h"

namespace hns_ads {

namespace {

constexpr auto kUserActivityEventToPredictorVariableTypeMapping =
    base::MakeFixedFlatMap<UserActivityEventType,
                           std::pair<hns_federated::mojom::CovariateType,
                                     hns_federated::mojom::CovariateType>>(
        {{UserActivityEventType::kBrowserDidBecomeActive,
          {hns_federated::mojom::CovariateType::
               kNumberOfBrowserDidBecomeActiveEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastBrowserDidBecomeActiveEvent}},
         {UserActivityEventType::kBrowserDidEnterForeground,
          {hns_federated::mojom::CovariateType::
               kNumberOfBrowserDidEnterForegroundEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastBrowserDidEnterForegroundEvent}},
         {UserActivityEventType::kBrowserDidResignActive,
          {hns_federated::mojom::CovariateType::
               kNumberOfBrowserWindowIsInactiveEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastBrowserWindowIsInactiveEvent}},
         {UserActivityEventType::kClickedBackOrForwardNavigationButtons,
          {hns_federated::mojom::CovariateType::
               kNumberOfClickedBackOrForwardNavigationButtonsEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastClickedBackOrForwardNavigationButtonsEvent}},
         {UserActivityEventType::kClickedLink,
          {hns_federated::mojom::CovariateType::kNumberOfClickedLinkEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastClickedLinkEvent}},
         {UserActivityEventType::kClickedReloadButton,
          {hns_federated::mojom::CovariateType::
               kNumberOfClickedReloadButtonEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastClickedReloadButtonEvent}},
         {UserActivityEventType::kClosedTab,
          {hns_federated::mojom::CovariateType::kNumberOfClosedTabEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastClosedTabEvent}},
         {UserActivityEventType::kTabChangedFocus,
          {hns_federated::mojom::CovariateType::
               kNumberOfFocusedOnExistingTabEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastFocusedOnExistingTabEvent}},
         {UserActivityEventType::kNewNavigation,
          {hns_federated::mojom::CovariateType::kNumberOfNewNavigationEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastNewNavigationEvent}},
         {UserActivityEventType::kOpenedNewTab,
          {hns_federated::mojom::CovariateType::kNumberOfOpenedNewTabEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastOpenedNewTabEvent}},
         {UserActivityEventType::kTabStartedPlayingMedia,
          {hns_federated::mojom::CovariateType::kNumberOfPlayedMediaEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastPlayedMediaEvent}},
         {UserActivityEventType::kSubmittedForm,
          {hns_federated::mojom::CovariateType::kNumberOfSubmittedFormEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastSubmittedFormEvent}},
         {UserActivityEventType::kTypedAndSelectedNonUrl,
          {hns_federated::mojom::CovariateType::
               kNumberOfTypedAndSelectedNonUrlEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastTypedAndSelectedNonUrlEvent}},
         {UserActivityEventType::kTypedKeywordOtherThanDefaultSearchProvider,
          {hns_federated::mojom::CovariateType::
               kNumberOfTypedKeywordOtherThanDefaultSearchProviderEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastTypedKeywordOtherThanDefaultSearchProviderEvent}},
         {UserActivityEventType::kTypedUrl,
          {hns_federated::mojom::CovariateType::kNumberOfTypedUrlEvents,
           hns_federated::mojom::CovariateType::
               kTimeSinceLastTypedUrlEvent}}});

constexpr base::TimeDelta kAverageClickthroughRateTimeWindows[] = {
    base::Days(1), base::Days(7), base::Days(28)};

}  // namespace

PredictorsManager::PredictorsManager() {
  SetPredictorVariable(
      std::make_unique<LastNotificationAdWasClickedPredictorVariable>());

  for (const auto& [user_activity_event_type, predictor_variable_type] :
       kUserActivityEventToPredictorVariableTypeMapping) {
    const auto& [number_of_user_activity_events,
                 time_since_last_user_activity_event] = predictor_variable_type;

    SetPredictorVariable(
        std::make_unique<NumberOfUserActivityEventsPredictorVariable>(
            user_activity_event_type, number_of_user_activity_events));

    SetPredictorVariable(
        std::make_unique<TimeSinceLastUserActivityEventPredictorVariable>(
            user_activity_event_type, time_since_last_user_activity_event));
  }

  for (const auto& time_window : kAverageClickthroughRateTimeWindows) {
    SetPredictorVariable(
        std::make_unique<AverageClickthroughRatePredictorVariable>(
            time_window));
  }
}

PredictorsManager::~PredictorsManager() = default;

// static
PredictorsManager& PredictorsManager::GetInstance() {
  return GlobalState::GetInstance()->GetPredictorsManager();
}

void PredictorsManager::SetPredictorVariable(
    std::unique_ptr<PredictorVariableInterface> predictor_variable) {
  CHECK(predictor_variable);

  const hns_federated::mojom::CovariateType type =
      predictor_variable->GetType();
  predictor_variables_[type] = std::move(predictor_variable);
}

std::vector<hns_federated::mojom::CovariateInfoPtr>
PredictorsManager::GetTrainingSample() const {
  std::vector<hns_federated::mojom::CovariateInfoPtr> training_sample;

  for (const auto& [_, predictor_variable] : predictor_variables_) {
    CHECK(predictor_variable);

    hns_federated::mojom::CovariateInfoPtr predictor =
        hns_federated::mojom::CovariateInfo::New();
    predictor->data_type = predictor_variable->GetDataType();
    predictor->type = predictor_variable->GetType();
    predictor->value = predictor_variable->GetValue();

    training_sample.push_back(std::move(predictor));
  }

  return training_sample;
}

void PredictorsManager::AddTrainingSample() const {
  std::vector<hns_federated::mojom::CovariateInfoPtr> training_sample =
      GetTrainingSample();
  AdsClientHelper::GetInstance()->AddTrainingSample(std::move(training_sample));
}

}  // namespace hns_ads
