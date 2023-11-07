/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/reminder/reminder.h"

#include <memory>
#include <vector>

#include "base/test/scoped_feature_list.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/internal/history/history_manager.h"
#include "hns/components/hns_ads/core/internal/reminder/reminder_feature.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

void AddHistory(const size_t count) {
  const NotificationAdInfo ad = BuildNotificationAd(
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true));

  for (size_t i = 0; i < count; i++) {
    HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);
  }
}

}  // namespace

class HnsAdsReminderTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    reminder_ = std::make_unique<Reminder>();
  }

  std::unique_ptr<Reminder> reminder_;
};

TEST_F(HnsAdsReminderTest, ShowReminderWhenUserClicksTheSameAdMultipleTimes) {
  // Arrange

  // Assert
  EXPECT_CALL(ads_client_mock_,
              ShowReminder(mojom::ReminderType::kClickedSameAdMultipleTimes));

  // Act
  AddHistory(/*count*/ kRemindUserIfClickingTheSameAdAfter.Get());

  FastForwardClockBy(base::Seconds(1));
}

TEST_F(HnsAdsReminderTest,
       DoNotShowReminderIfUserDoesNotClickTheSameAdMultipleTimes) {
  // Arrange

  // Assert
  EXPECT_CALL(ads_client_mock_, ShowReminder).Times(0);

  // Act
  AddHistory(/*count*/ kRemindUserIfClickingTheSameAdAfter.Get() - 1);

  FastForwardClockBy(base::Seconds(1));
}

TEST_F(HnsAdsReminderTest,
       DoNotShowReminderIfUserDoesNotClickTheSameAdMultipleTimesWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kReminderFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Assert
  EXPECT_CALL(ads_client_mock_, ShowReminder).Times(0);

  // Act
  AddHistory(/*count*/ kRemindUserIfClickingTheSameAdAfter.Get());

  FastForwardClockBy(base::Seconds(1));
}

}  // namespace hns_ads
