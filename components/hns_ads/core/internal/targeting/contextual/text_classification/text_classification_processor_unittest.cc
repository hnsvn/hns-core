/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_classification/text_classification_processor.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/common/resources/language_components_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/client_state_manager.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_classification/model/text_classification_alias.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_classification/model/text_classification_model.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_classification/resource/text_classification_resource.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTextClassificationProcessorTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    resource_ = std::make_unique<TextClassificationResource>();
  }

  bool LoadResource() {
    NotifyDidUpdateResourceComponent(kLanguageComponentManifestVersion,
                                     kLanguageComponentId);
    task_environment_.RunUntilIdle();
    return resource_->IsInitialized();
  }

  std::unique_ptr<TextClassificationResource> resource_;
};

TEST_F(HnsAdsTextClassificationProcessorTest,
       DoNotProcessIfResourceIsNotInitialized) {
  // Arrange

  // Act
  TextClassificationProcessor processor(*resource_);
  processor.Process(/*text*/ "The quick brown fox jumps over the lazy dog");

  // Assert
  const TextClassificationProbabilityList& list =
      ClientStateManager::GetInstance()
          .GetTextClassificationProbabilitiesHistory();

  EXPECT_TRUE(list.empty());
}

TEST_F(HnsAdsTextClassificationProcessorTest, DoNotProcessForEmptyText) {
  // Act
  ASSERT_TRUE(LoadResource());

  const std::string text;
  TextClassificationProcessor processor(*resource_);
  processor.Process(text);

  // Assert
  const TextClassificationProbabilityList& list =
      ClientStateManager::GetInstance()
          .GetTextClassificationProbabilitiesHistory();

  EXPECT_TRUE(list.empty());
}

TEST_F(HnsAdsTextClassificationProcessorTest, NeverProcessed) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  const TextClassificationModel model;
  const SegmentList segments = model.GetSegments();

  // Act
  const TextClassificationProbabilityList& list =
      ClientStateManager::GetInstance()
          .GetTextClassificationProbabilitiesHistory();

  // Assert
  EXPECT_TRUE(list.empty());
}

TEST_F(HnsAdsTextClassificationProcessorTest, ProcessText) {
  // Act
  ASSERT_TRUE(LoadResource());

  TextClassificationProcessor processor(*resource_);
  processor.Process(/*text*/ "Some content about technology & computing");

  // Assert
  const TextClassificationProbabilityList& list =
      ClientStateManager::GetInstance()
          .GetTextClassificationProbabilitiesHistory();

  EXPECT_EQ(1U, list.size());
}

TEST_F(HnsAdsTextClassificationProcessorTest, ProcessMultipleText) {
  // Act
  ASSERT_TRUE(LoadResource());

  TextClassificationProcessor processor(*resource_);
  processor.Process(/*text*/ "Some content about cooking food");
  processor.Process(/*text*/ "Some content about finance & banking");
  processor.Process(/*text*/ "Some content about technology & computing");

  // Assert
  const TextClassificationProbabilityList& list =
      ClientStateManager::GetInstance()
          .GetTextClassificationProbabilitiesHistory();

  EXPECT_EQ(3U, list.size());
}

}  // namespace hns_ads
