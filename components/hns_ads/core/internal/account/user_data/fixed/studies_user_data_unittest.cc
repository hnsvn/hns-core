/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/studies_user_data.h"

#include "base/metrics/field_trial.h"
#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsStudiesUserDataTest : public UnitTestBase {};

TEST_F(HnsAdsStudiesUserDataTest, BuildStudiesUserDataForRewardsUser) {
  // Arrange
  const scoped_refptr<base::FieldTrial> field_trial_1 =
      base::FieldTrialList::CreateFieldTrial("HnsAds.FooStudy", "GroupA");
  field_trial_1->group_name();

  const scoped_refptr<base::FieldTrial> field_trial_2 =
      base::FieldTrialList::CreateFieldTrial("HnsAds.BarStudy", "GroupB");
  field_trial_2->group_name();

  const scoped_refptr<base::FieldTrial> field_trial_3 =
      base::FieldTrialList::CreateFieldTrial("FooBarStudy", "GroupC");
  field_trial_3->group_name();

  ASSERT_EQ(3U, base::FieldTrialList::GetFieldTrialCount());

  // Act

  // Assert
  EXPECT_EQ(
      base::test::ParseJsonDict(
          R"({"studies":[{"group":"GroupB","name":"HnsAds.BarStudy"},{"group":"GroupA","name":"HnsAds.FooStudy"}]})"),
      BuildStudiesUserData());
}

TEST_F(HnsAdsStudiesUserDataTest, BuildStudiesUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  const scoped_refptr<base::FieldTrial> field_trial_1 =
      base::FieldTrialList::CreateFieldTrial("HnsAds.FooStudy", "GroupA");
  field_trial_1->group_name();

  const scoped_refptr<base::FieldTrial> field_trial_2 =
      base::FieldTrialList::CreateFieldTrial("HnsAds.BarStudy", "GroupB");
  field_trial_2->group_name();

  const scoped_refptr<base::FieldTrial> field_trial_3 =
      base::FieldTrialList::CreateFieldTrial("FooBarStudy", "GroupC");
  field_trial_3->group_name();

  ASSERT_EQ(3U, base::FieldTrialList::GetFieldTrialCount());

  // Act

  // Assert
  EXPECT_TRUE(BuildStudiesUserData().empty());
}

TEST_F(HnsAdsStudiesUserDataTest, BuildStudiesUserDataIfNoFieldTrials) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(R"({"studies":[]})"),
            BuildStudiesUserData());
}

}  // namespace hns_ads
