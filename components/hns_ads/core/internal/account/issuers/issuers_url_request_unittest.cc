/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/issuers/issuers_url_request.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/account/issuers/issuers_info.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_url_request_delegate_mock.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "net/http/http_status_code.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsIssuersUrlRequestTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    issuers_url_request_ = std::make_unique<IssuersUrlRequest>();
    issuers_url_request_->SetDelegate(&issuers_url_request_delegate_mock_);
  }

  std::unique_ptr<IssuersUrlRequest> issuers_url_request_;
  ::testing::NiceMock<IssuersUrlRequestDelegateMock>
      issuers_url_request_delegate_mock_;
};

TEST_F(HnsAdsIssuersUrlRequestTest, FetchIssuers) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildIssuersUrlPath(),
       {{net::HTTP_OK, BuildIssuersUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  // Assert
  EXPECT_CALL(issuers_url_request_delegate_mock_,
              OnDidFetchIssuers(BuildIssuersForTesting()));
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnFailedToFetchIssuers)
      .Times(0);
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnWillRetryFetchingIssuers)
      .Times(0);
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnDidRetryFetchingIssuers)
      .Times(0);

  // Act
  issuers_url_request_->PeriodicallyFetch();
}

TEST_F(HnsAdsIssuersUrlRequestTest,
       DoNotFetchIssuersIfInvalidJsonResponseBody) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildIssuersUrlPath(), {{net::HTTP_OK, /*response_body*/ "{INVALID}"}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  EXPECT_CALL(issuers_url_request_delegate_mock_, OnDidFetchIssuers).Times(0);
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnFailedToFetchIssuers);
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnWillRetryFetchingIssuers);
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnDidRetryFetchingIssuers)
      .Times(0);

  // Act
  issuers_url_request_->PeriodicallyFetch();

  // Assert
  EXPECT_FALSE(GetIssuers());
}

TEST_F(HnsAdsIssuersUrlRequestTest, RetryFetchingIssuersIfNonHttpOkResponse) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildIssuersUrlPath(),
       {{net::HTTP_INTERNAL_SERVER_ERROR,
         /*response_body*/ net::GetHttpReasonPhrase(
             net::HTTP_INTERNAL_SERVER_ERROR)},
        {net::HTTP_OK, BuildIssuersUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  EXPECT_CALL(issuers_url_request_delegate_mock_, OnDidFetchIssuers);
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnFailedToFetchIssuers);
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnWillRetryFetchingIssuers);
  EXPECT_CALL(issuers_url_request_delegate_mock_, OnDidRetryFetchingIssuers);

  ON_CALL(issuers_url_request_delegate_mock_, OnDidFetchIssuers)
      .WillByDefault(::testing::Invoke(
          [](const IssuersInfo& issuers) { SetIssuers(issuers); }));

  // Act
  issuers_url_request_->PeriodicallyFetch();

  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(GetIssuers());
}

}  // namespace hns_ads
