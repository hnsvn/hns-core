/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/transfer/transfer.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTransferTest : public TransferObserver, public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    transfer_ = std::make_unique<Transfer>();
    transfer_->AddObserver(this);
  }

  void TearDown() override {
    transfer_->RemoveObserver(this);

    UnitTestBase::TearDown();
  }

  void OnDidTransferAd(const AdInfo& /*ad*/) override { transfer_count_++; }

  void OnFailedToTransferAd(const AdInfo& /*ad*/) override {
    transfer_count_--;
  }

  int GetTransferCount() const { return transfer_count_; }

  std::unique_ptr<Transfer> transfer_;

 private:
  int transfer_count_ = 0;
};

TEST_F(HnsAdsTransferTest, DoNotTransferAdIfUrlIsMissingHTTPOrHTTPSScheme) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  transfer_->SetLastClickedAd(ad);

  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ true);

  // Act
  transfer_->MaybeTransferAd(/*tab_id*/ 1, {GURL("hns.com")});
  AdvanceClockBy(base::Seconds(10));

  // Assert
  EXPECT_EQ(0, GetTransferCount());
}

TEST_F(HnsAdsTransferTest,
       DoNotTransferAdIfTheUrlDoesNotMatchTheLastClickedAd) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  transfer_->SetLastClickedAd(ad);

  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://foobar.com")},
      /*is_visible*/ true);

  // Act
  transfer_->MaybeTransferAd(/*tab_id*/ 1, {GURL("hns.com")});
  AdvanceClockBy(base::Seconds(10));

  // Assert
  EXPECT_EQ(0, GetTransferCount());
}

TEST_F(HnsAdsTransferTest, DoNotTransferAdIfTheSameAdIsAlreadyTransferring) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  transfer_->SetLastClickedAd(ad);

  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ true);

  transfer_->MaybeTransferAd(/*tab_id*/ 1, {GURL("https://hns.com")});

  // Act
  transfer_->MaybeTransferAd(/*tab_id*/ 1, {GURL("https://hns.com")});

  FastForwardClockBy(base::Seconds(10));

  // Assert
  EXPECT_EQ(1, GetTransferCount());
}

TEST_F(HnsAdsTransferTest, TransferAdIfAnotherAdIsAlreadyTransferring) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  transfer_->SetLastClickedAd(ad);

  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://foobar.com")},
      /*is_visible*/ true);

  transfer_->MaybeTransferAd(/*tab_id*/ 1, {GURL("https://foobar.com")});

  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://foobar.com")},
      /*is_visible*/ false);

  NotifyTabDidChange(
      /*id*/ 2, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ true);

  // Act
  transfer_->MaybeTransferAd(/*tab_id*/ 2, {GURL("https://hns.com")});

  FastForwardClockBy(base::Seconds(10));

  // Assert
  EXPECT_EQ(1, GetTransferCount());
}

TEST_F(HnsAdsTransferTest,
       TransferAdIfTheTabIsVisibleAndTheUrlIsTheSameAsTheDomainOrHost) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  transfer_->SetLastClickedAd(ad);

  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ true);

  // Act
  transfer_->MaybeTransferAd(/*tab_id*/ 1, {GURL("https://hns.com")});

  FastForwardClockBy(base::Seconds(10));

  // Assert
  EXPECT_EQ(1, GetTransferCount());
}

TEST_F(HnsAdsTransferTest, FailToTransferAdIfNotVisible) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  transfer_->SetLastClickedAd(ad);

  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ false);

  // Act
  transfer_->MaybeTransferAd(/*tab_id*/ 1, {GURL("https://hns.com")});

  FastForwardClockBy(base::Seconds(10));

  // Assert
  EXPECT_EQ(-1, GetTransferCount());
}

TEST_F(HnsAdsTransferTest,
       FailToTransferAdIfTheTabUrlIsNotTheSameAsTheDomainOrHost) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  transfer_->SetLastClickedAd(ad);

  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ true);

  transfer_->MaybeTransferAd(/*tab_id*/ 1, {GURL("https://hns.com")});

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://foobar.com")},
      /*is_visible*/ true);

  FastForwardClockBy(base::Seconds(10));

  // Assert
  EXPECT_EQ(-1, GetTransferCount());
}

}  // namespace hns_ads
