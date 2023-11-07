/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/deposits/cash_deposit.h"

#include "base/test/mock_callback.h"
#include "hns/components/hns_ads/core/internal/account/deposits/deposit_interface.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCashDepositIntegrationTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUpForTesting(/*is_integration_test*/ true);
  }

  void SetUpMocks() override {
    const URLResponseMap url_responses = {
        {BuildCatalogUrlPath(),
         {{net::HTTP_OK, /*response_body*/ "/catalog.json"}}}};
    MockUrlResponses(ads_client_mock_, url_responses);
  }
};

TEST_F(HnsAdsCashDepositIntegrationTest, GetValue) {
  // Arrange

  // Assert
  base::MockCallback<GetDepositCallback> callback;
  EXPECT_CALL(callback, Run(/*success*/ true, /*value*/ 1.0));

  // Act
  CashDeposit deposit;
  deposit.GetValue(kCreativeInstanceId, callback.Get());
}

TEST_F(HnsAdsCashDepositIntegrationTest,
       DoNotGetValueForMissingCreativeInstanceId) {
  // Arrange

  // Assert
  base::MockCallback<GetDepositCallback> callback;
  EXPECT_CALL(callback, Run(/*success*/ false, /*value*/ 0.0));

  // Act
  CashDeposit deposit;
  deposit.GetValue(kMissingCreativeInstanceId, callback.Get());
}

}  // namespace hns_ads
