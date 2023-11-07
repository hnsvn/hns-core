/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_resource.h"

#include <memory>
#include <string>
#include <utility>

#include "base/files/file.h"
#include "hns/components/hns_ads/core/internal/common/resources/country_components_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/resources/resources_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_file_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_resource_constants.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPurchaseIntentResourceTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    resource_ = std::make_unique<PurchaseIntentResource>();
  }

  void LoadResource(const std::string& id) {
    NotifyDidUpdateResourceComponent(kCountryComponentManifestVersion, id);
    task_environment_.RunUntilIdle();
  }

  std::unique_ptr<PurchaseIntentResource> resource_;
};

TEST_F(HnsAdsPurchaseIntentResourceTest, IsNotInitialized) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsPurchaseIntentResourceTest, DoNotLoadInvalidResource) {
  // Arrange
  ASSERT_TRUE(CopyFileFromTestPathToTempPath(kInvalidResourceId,
                                             kPurchaseIntentResourceId));

  // Act
  LoadResource(kCountryComponentId);

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsPurchaseIntentResourceTest, DoNotLoadMissingResource) {
  // Arrange
  EXPECT_CALL(ads_client_mock_, LoadFileResource(kPurchaseIntentResourceId,
                                                 ::testing::_, ::testing::_))
      .WillOnce(
          ::testing::Invoke([](const std::string& /*id*/, const int /*version*/,
                               LoadFileCallback callback) {
            const base::FilePath path =
                GetFileResourcePath().AppendASCII(kMissingResourceId);

            base::File file(path, base::File::Flags::FLAG_OPEN |
                                      base::File::Flags::FLAG_READ);
            std::move(callback).Run(std::move(file));
          }));

  // Act
  LoadResource(kCountryComponentId);

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsPurchaseIntentResourceTest, LoadResourceWhenLocaleDidChange) {
  // Arrange
  LoadResource(kCountryComponentId);

  // Act
  NotifyLocaleDidChange(/*locale*/ "en_GB");
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(
    HnsAdsPurchaseIntentResourceTest,
    DoNotLoadResourceWhenLocaleDidChangeIfNotificationAdsAndHnsNewsAdsAreDisabled) {
  // Arrange
  DisableNotificationAdsForTesting();
  DisableHnsNewsAdsForTesting();

  LoadResource(kCountryComponentId);

  // Act
  NotifyLocaleDidChange(/*locale*/ "en_GB");
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsPurchaseIntentResourceTest,
       DoNotResetResourceWhenLocaleDidChange) {
  // Arrange
  LoadResource(kCountryComponentId);

  // Act
  NotifyLocaleDidChange(/*locale*/ "en_GB");
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(HnsAdsPurchaseIntentResourceTest,
       LoadResourceWhenOptedInToNotificationAdsPrefDidChange) {
  // Arrange
  LoadResource(kCountryComponentId);

  // Act
  NotifyPrefDidChange(prefs::kOptedInToNotificationAds);
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(
    HnsAdsPurchaseIntentResourceTest,
    DoNotLoadResourceWhenOptedInToNotificationAdsPrefDidChangeIfNotificationAdsAndHnsNewsAdsAreDisabled) {
  // Arrange
  DisableNotificationAdsForTesting();
  DisableHnsNewsAdsForTesting();

  LoadResource(kCountryComponentId);

  // Act
  NotifyPrefDidChange(prefs::kOptedInToNotificationAds);
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsPurchaseIntentResourceTest,
       DoNotResetResourceWhenOptedInToNotificationAdsPrefDidChange) {
  // Arrange
  LoadResource(kCountryComponentId);

  // Act
  NotifyPrefDidChange(prefs::kOptedInToNotificationAds);
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(HnsAdsPurchaseIntentResourceTest,
       LoadResourceWhenDidUpdateResourceComponent) {
  // Arrange

  // Act
  LoadResource(kCountryComponentId);

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(
    HnsAdsPurchaseIntentResourceTest,
    DoNotLoadResourceWhenDidUpdateResourceComponentIfInvalidCountryComponentId) {
  // Arrange

  // Act
  LoadResource(kInvalidCountryComponentId);

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(
    HnsAdsPurchaseIntentResourceTest,
    DoNotLoadResourceWhenDidUpdateResourceComponentIfNotificationAdsAndHnsNewsAdsAreDisabled) {
  // Arrange
  DisableNotificationAdsForTesting();
  DisableHnsNewsAdsForTesting();

  // Act
  LoadResource(kCountryComponentId);

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsPurchaseIntentResourceTest,
       DoNotResetResourceWhenDidUpdateResourceComponent) {
  // Arrange
  LoadResource(kCountryComponentId);

  // Act
  LoadResource(kCountryComponentId);

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

}  // namespace hns_ads
