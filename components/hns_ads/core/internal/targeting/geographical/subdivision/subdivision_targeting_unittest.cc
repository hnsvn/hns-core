/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_url_request_unittest_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/l10n/common/test/scoped_default_locale.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsSubdivisionTargetingTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    subdivision_targeting_ = std::make_unique<SubdivisionTargeting>();
  }

  void MockHttpOkUrlResponse(const std::string& country_code,
                             const std::string& subdivision_code) {
    const URLResponseMap url_responses = {
        {BuildSubdivisionUrlPath(),
         {{net::HTTP_OK, BuildSubdivisionUrlResponseBodyForTesting(
                             country_code, subdivision_code)}}}};
    MockUrlResponses(ads_client_mock_, url_responses);
  }

  std::unique_ptr<SubdivisionTargeting> subdivision_targeting_;
};

TEST_F(HnsAdsSubdivisionTargetingTest,
       AllowAndFetchWhenOptingInToNotificationAds) {
  // Arrange
  DisableHnsNewsAdsForTesting();
  DisableNotificationAdsForTesting();

  MockHttpOkUrlResponse(/*country_code*/ "US", /*subdivision_code*/ "CA");

  // Act
  ads_client_mock_.SetBooleanPref(prefs::kOptedInToNotificationAds, true);

  // Assert
  EXPECT_TRUE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
  EXPECT_EQ("US-CA", ads_client_mock_.GetStringPref(
                         prefs::kSubdivisionTargetingAutoDetectedSubdivision));
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       AllowAndFetchWhenOptingInToHnsNewsAds) {
  // Arrange
  DisableHnsNewsAdsForTesting();
  DisableNotificationAdsForTesting();

  MockHttpOkUrlResponse(/*country_code*/ "US", /*subdivision_code*/ "CA");

  // Act
  ads_client_mock_.SetBooleanPref(hns_news::prefs::kHnsNewsOptedIn, true);
  ads_client_mock_.SetBooleanPref(hns_news::prefs::kNewTabPageShowToday,
                                  true);

  // Assert
  EXPECT_TRUE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
  EXPECT_EQ("US-CA", ads_client_mock_.GetStringPref(
                         prefs::kSubdivisionTargetingAutoDetectedSubdivision));
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       DoNotFetchWhenOptingOutOfNotificationAds) {
  // Arrange
  DisableHnsNewsAdsForTesting();

  MockHttpOkUrlResponse(/*country_code*/ "US", /*subdivision_code*/ "CA");

  NotifyDidInitializeAds();

  // Assert
  EXPECT_CALL(ads_client_mock_, UrlRequest).Times(0);

  // Act
  ads_client_mock_.SetBooleanPref(prefs::kOptedInToNotificationAds, false);
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       DoNotFetchWhenOptingOutOfHnsNewsAds) {
  // Arrange
  DisableNotificationAdsForTesting();

  MockHttpOkUrlResponse(/*country_code*/ "US", /*subdivision_code*/ "CA");

  NotifyDidInitializeAds();

  // Assert
  EXPECT_CALL(ads_client_mock_, UrlRequest).Times(0);

  // Act
  ads_client_mock_.SetBooleanPref(hns_news::prefs::kHnsNewsOptedIn, false);
  ads_client_mock_.SetBooleanPref(hns_news::prefs::kNewTabPageShowToday,
                                  false);
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       ShouldAllowAndAutoDetectForSupportedCountryAndRegionUrlResponse) {
  // Arrange
  MockHttpOkUrlResponse(/*country_code*/ "US", /*subdivision_code*/ "CA");

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_TRUE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
  EXPECT_EQ("US-CA", ads_client_mock_.GetStringPref(
                         prefs::kSubdivisionTargetingAutoDetectedSubdivision));
}

TEST_F(
    HnsAdsSubdivisionTargetingTest,
    ShouldAllowButDefaultToDisabledForSupportedCountryButNoRegionUrlResponse) {
  // Arrange
  MockHttpOkUrlResponse(/*country_code*/ "US",
                        /*subdivision_code*/ "NO REGION");

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_TRUE(SubdivisionTargeting::ShouldAllow());
  EXPECT_TRUE(subdivision_targeting_->IsDisabled());
  EXPECT_FALSE(subdivision_targeting_->ShouldAutoDetect());
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       ShouldAutoDetectForUnsupportedCountryAndRegionUrlResponse) {
  // Arrange
  MockHttpOkUrlResponse(/*country_code*/ "XX", /*subdivision_code*/ "XX");

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_FALSE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
}

TEST_F(HnsAdsSubdivisionTargetingTest, ShouldAutoDetectForUnsupportedLocale) {
  // Arrange
  const hns_l10n::test::ScopedDefaultLocale scoped_default_locale{"xx_XX"};

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_FALSE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       ShouldAllowIfDisabledAndCountryIsSupported) {
  // Arrange
  SetStringPref(prefs::kSubdivisionTargetingSubdivision, "DISABLED");

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_TRUE(SubdivisionTargeting::ShouldAllow());
  EXPECT_TRUE(subdivision_targeting_->IsDisabled());
  EXPECT_FALSE(subdivision_targeting_->ShouldAutoDetect());
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       ShouldAllowAndAutoDetectIfCountryIsSupported) {
  // Arrange
  SetStringPref(prefs::kSubdivisionTargetingAutoDetectedSubdivision, "US-CA");

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_TRUE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
  EXPECT_EQ("US-CA", ads_client_mock_.GetStringPref(
                         prefs::kSubdivisionTargetingAutoDetectedSubdivision));
}

TEST_F(HnsAdsSubdivisionTargetingTest, ShouldNotAllowIfCountryIsUnsupported) {
  // Arrange
  const hns_l10n::test::ScopedDefaultLocale scoped_default_locale{"xx_XX"};

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_FALSE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       ShouldNotAllowIfLocaleAndSubdivisionCountriesMismatch) {
  // Arrange
  SetStringPref(prefs::kSubdivisionTargetingSubdivision, "CA-QC");

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_FALSE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       ShouldAutoDetectAndNotAllowIfSubdivisionCodeIsEmpty) {
  // Arrange

  // Act
  NotifyDidInitializeAds();

  // Assert
  EXPECT_FALSE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_TRUE(subdivision_targeting_->ShouldAutoDetect());
}

TEST_F(HnsAdsSubdivisionTargetingTest,
       RetryAfterInvalidUrlResponseStatusCode) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildSubdivisionUrlPath(),
       {{net::HTTP_INTERNAL_SERVER_ERROR,
         /*response_body*/ net::GetHttpReasonPhrase(
             net::HTTP_INTERNAL_SERVER_ERROR)},
        {net::HTTP_OK,
         BuildSubdivisionUrlResponseBodyForTesting(
             /*country_code*/ "US", /*subdivision_code*/ "CA")}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  NotifyDidInitializeAds();

  // Act
  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_EQ("US-CA", ads_client_mock_.GetStringPref(
                         prefs::kSubdivisionTargetingAutoDetectedSubdivision));
}

class HnsAdsSubdivisionTargetingRetryOnInvalidUrlResponseBodyTest
    : public HnsAdsSubdivisionTargetingTest,
      public ::testing::WithParamInterface<const char*> {};

TEST_P(HnsAdsSubdivisionTargetingRetryOnInvalidUrlResponseBodyTest,
       RetryAfterInvalidUrlResponseBody) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildSubdivisionUrlPath(),
       {{net::HTTP_OK, /*response_body*/ GetParam()},
        {net::HTTP_OK,
         BuildSubdivisionUrlResponseBodyForTesting(
             /*country_code*/ "US", /*subdivision_code*/ "CA")}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  NotifyDidInitializeAds();

  // Act
  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(SubdivisionTargeting::ShouldAllow());
  EXPECT_FALSE(subdivision_targeting_->IsDisabled());
  EXPECT_EQ("US-CA", ads_client_mock_.GetStringPref(
                         prefs::kSubdivisionTargetingAutoDetectedSubdivision));
}

INSTANTIATE_TEST_SUITE_P(
    ,
    HnsAdsSubdivisionTargetingRetryOnInvalidUrlResponseBodyTest,
    ::testing::Values("",
                      "INVALID",
                      "{}",
                      "{INVALID}",
                      R"({"country":"US","region":""})",
                      R"({"country":"","region":"CA"})",
                      R"({"country":"","region":""})",
                      R"({"country":"US"})",
                      R"({"region":"CA"})"));

}  // namespace hns_ads
