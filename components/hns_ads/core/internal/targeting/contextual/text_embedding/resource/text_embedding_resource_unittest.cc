/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/resource/text_embedding_resource.h"

#include <memory>
#include <string>
#include <utility>

#include "base/files/file.h"
#include "hns/components/hns_ads/core/internal/common/resources/language_components_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/resources/resources_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_file_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/resource/text_embedding_resource_constants.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTextEmbeddingResourceTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    resource_ = std::make_unique<TextEmbeddingResource>();
  }

  void LoadResource(const std::string& id) {
    NotifyDidUpdateResourceComponent(kLanguageComponentManifestVersion, id);
    task_environment_.RunUntilIdle();
  }

  std::unique_ptr<TextEmbeddingResource> resource_;
};

TEST_F(HnsAdsTextEmbeddingResourceTest, IsNotInitialized) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest, DoNotLoadInvalidResource) {
  // Arrange
  ASSERT_TRUE(CopyFileFromTestPathToTempPath(kInvalidResourceId,
                                             kTextEmbeddingResourceId));

  // Act
  LoadResource(kLanguageComponentId);

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest, DoNotLoadMissingResource) {
  // Arrange
  EXPECT_CALL(ads_client_mock_, LoadFileResource(kTextEmbeddingResourceId,
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
  LoadResource(kLanguageComponentId);

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest, LoadResourceWhenLocaleDidChange) {
  // Arrange
  LoadResource(kLanguageComponentId);

  // Act
  NotifyLocaleDidChange(/*locale*/ "en_GB");
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest,
       DoNotLoadResourceWhenLocaleDidChangeIfOptedOutOfNotificationAds) {
  // Arrange
  DisableNotificationAdsForTesting();

  LoadResource(kLanguageComponentId);

  // Act
  NotifyLocaleDidChange(/*locale*/ "en_GB");
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest,
       DoNotResetResourceWhenLocaleDidChange) {
  // Arrange
  LoadResource(kLanguageComponentId);

  // Act
  NotifyLocaleDidChange(/*locale*/ "en_GB");
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest,
       LoadResourceWhenOptedInToNotificationAdsPrefDidChange) {
  // Arrange
  LoadResource(kLanguageComponentId);

  // Act
  NotifyPrefDidChange(prefs::kOptedInToNotificationAds);
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(
    HnsAdsTextEmbeddingResourceTest,
    DoNotLoadResourceWhenOptedInToNotificationAdsPrefDidChangeIfOptedOutOfNotificationAds) {
  // Arrange
  DisableNotificationAdsForTesting();

  LoadResource(kLanguageComponentId);

  // Act
  NotifyPrefDidChange(prefs::kOptedInToNotificationAds);
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest,
       ResetResourceWhenOptedInToNotificationAdsPrefDidChange) {
  // Arrange
  LoadResource(kLanguageComponentId);

  // Act
  NotifyPrefDidChange(prefs::kOptedInToNotificationAds);
  task_environment_.RunUntilIdle();

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest,
       LoadResourceWhenDidUpdateResourceComponent) {
  // Arrange

  // Act
  LoadResource(kLanguageComponentId);

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

TEST_F(
    HnsAdsTextEmbeddingResourceTest,
    DoNotLoadResourceWhenDidUpdateResourceComponentIfInvalidLanguageComponentId) {
  // Arrange

  // Act
  LoadResource(kInvalidLanguageComponentId);

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(
    HnsAdsTextEmbeddingResourceTest,
    DoNotLoadResourceWhenDidUpdateResourceComponentIfOptedOutOfNotificationAds) {
  // Arrange
  DisableNotificationAdsForTesting();

  // Act
  LoadResource(kLanguageComponentId);

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

TEST_F(HnsAdsTextEmbeddingResourceTest,
       DoNotResetResourceWhenDidUpdateResourceComponent) {
  // Arrange
  LoadResource(kLanguageComponentId);

  // Act
  LoadResource(kLanguageComponentId);

  // Assert
  EXPECT_TRUE(resource_->IsInitialized());
}

}  // namespace hns_ads
