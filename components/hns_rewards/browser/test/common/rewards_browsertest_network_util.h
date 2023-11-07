/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_NETWORK_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_NETWORK_UTIL_H_

#include <memory>
#include <string>

#include "hns/components/hns_rewards/core/common/request_util.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"
#include "net/test/embedded_test_server/http_request.h"
#include "net/test/embedded_test_server/http_response.h"

namespace hns_rewards::test_util {

std::unique_ptr<net::test_server::HttpResponse> HandleRequest(
    const net::test_server::HttpRequest& request);

std::string GetUpholdCapabilities();

std::string GetUpholdUser();

std::string GetUpholdCard(
    const std::string& balance,
    const std::string& address);

std::string GetOrderCreateResponse(mojom::SKUOrderPtr sku_order);

}  // namespace hns_rewards::test_util

#endif  // HNS_COMPONENTS_HNS_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_NETWORK_UTIL_H_
