/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/epsilon_greedy_bandit_processor.h"

#include <string>
#include <utility>

#include "base/check_op.h"
#include "base/containers/contains.h"
#include "base/notreached.h"
#include "base/strings/string_piece.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/segments/segment_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/epsilon_greedy_bandit_arm_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/epsilon_greedy_bandit_arm_util.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/epsilon_greedy_bandit_arms_alias.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/epsilon_greedy_bandit_feedback_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/epsilon_greedy_bandit_segments.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"

namespace hns_ads {

namespace {

constexpr double kDefaultArmValue = 1.0;
constexpr int kDefaultArmPulls = 0;

bool DoesRequireResource() {
  return UserHasOptedInToHnsNewsAds() || UserHasOptedInToNotificationAds();
}

void MaybeAddOrResetArms(EpsilonGreedyBanditArmMap& arms) {
  for (const base::StringPiece value : GetSegments()) {
    std::string segment = static_cast<std::string>(value);
    if (base::Contains(arms, segment)) {
      BLOG(3, "Epsilon greedy bandit arm already exists for " << segment
                                                              << " segment");

      continue;
    }

    EpsilonGreedyBanditArmInfo arm;
    arm.segment = segment;
    arm.value = kDefaultArmValue;
    arm.pulls = kDefaultArmPulls;

    BLOG(2,
         "Epsilon greedy bandit arm was added for " << segment << " segment");

    arms.insert_or_assign(std::move(segment), arm);
  }
}

void MaybeDeleteArms(EpsilonGreedyBanditArmMap& arms) {
  for (auto iter = arms.cbegin(); iter != arms.cend();) {
    if (base::Contains(GetSegments(), iter->first)) {
      ++iter;
      continue;
    }

    BLOG(2, "Epsilon greedy bandit arm was deleted for " << iter->first
                                                         << " segment ");

    iter = arms.erase(iter);
  }
}

void UpdateArm(const double reward, const std::string& segment) {
  EpsilonGreedyBanditArmMap arms = GetEpsilonGreedyBanditArms();
  if (arms.empty()) {
    return BLOG(1, "No epsilon greedy bandit arms");
  }

  const auto iter = arms.find(segment);
  if (iter == arms.cend()) {
    return BLOG(1, "Epsilon greedy bandit arm was not found for "
                       << segment << " segment");
  }

  EpsilonGreedyBanditArmInfo arm = iter->second;
  arm.pulls++;
  CHECK_NE(0, arm.pulls);
  arm.value = arm.value + (1.0 / arm.pulls * (reward - arm.value));
  iter->second = arm;

  SetEpsilonGreedyBanditArms(arms);

  BLOG(1,
       "Epsilon greedy bandit arm was updated for " << segment << " segment");
}

}  // namespace

EpsilonGreedyBanditProcessor::EpsilonGreedyBanditProcessor() {
  AdsClientHelper::AddObserver(this);
}

EpsilonGreedyBanditProcessor::~EpsilonGreedyBanditProcessor() {
  AdsClientHelper::RemoveObserver(this);
}

void EpsilonGreedyBanditProcessor::Process(
    const EpsilonGreedyBanditFeedbackInfo& feedback) const {
  CHECK(mojom::IsKnownEnumValue(feedback.ad_event_type));

  if (!is_initialized_) {
    return;
  }

  CHECK(!feedback.segment.empty());

  const std::string segment = GetParentSegment(feedback.segment);
  CHECK(!segment.empty());

  switch (feedback.ad_event_type) {
    case mojom::NotificationAdEventType::kTimedOut:
    case mojom::NotificationAdEventType::kDismissed: {
      UpdateArm(/*reward*/ 0.0, segment);
      break;
    }

    case mojom::NotificationAdEventType::kClicked: {
      UpdateArm(/*reward*/ 1.0, segment);
      break;
    }

    case mojom::NotificationAdEventType::kServed:
    case mojom::NotificationAdEventType::kViewed: {
      NOTREACHED_NORETURN();
    }
  }

  BLOG(1, "Epsilon greedy bandit processed " << feedback.ad_event_type);
}

///////////////////////////////////////////////////////////////////////////////

void EpsilonGreedyBanditProcessor::MaybeInitializeArms() {
  if (DoesRequireResource()) {
    InitializeArms();
  }
}

void EpsilonGreedyBanditProcessor::MaybeInitializeOrResetArms() {
  if (!IsInitialized() && DoesRequireResource()) {
    InitializeArms();
  } else if (IsInitialized() && !DoesRequireResource()) {
    ResetArms();
  }
}

void EpsilonGreedyBanditProcessor::InitializeArms() {
  if (is_initialized_) {
    return;
  }

  EpsilonGreedyBanditArmMap arms = GetEpsilonGreedyBanditArms();

  MaybeAddOrResetArms(arms);

  MaybeDeleteArms(arms);

  SetEpsilonGreedyBanditArms(arms);

  is_initialized_ = true;

  BLOG(1, "Successfully initialized epsilon greedy bandit arms");
}

void EpsilonGreedyBanditProcessor::ResetArms() {
  BLOG(1, "Reset epsilon greedy bandit arms");

  is_initialized_ = false;

  ResetEpsilonGreedyBanditArms();
}

void EpsilonGreedyBanditProcessor::OnNotifyDidInitializeAds() {
  MaybeInitializeArms();
}

void EpsilonGreedyBanditProcessor::OnNotifyPrefDidChange(
    const std::string& path) {
  if (path == hns_rewards::prefs::kEnabled ||
      path == prefs::kOptedInToNotificationAds ||
      path == hns_news::prefs::kHnsNewsOptedIn ||
      path == hns_news::prefs::kNewTabPageShowToday) {
    MaybeInitializeOrResetArms();
  }
}

}  // namespace hns_ads
