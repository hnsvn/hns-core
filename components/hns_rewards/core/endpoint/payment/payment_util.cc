/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoint/payment/payment_util.h"

#include "hns/components/hns_rewards/core/rewards_engine_impl.h"

namespace hns_rewards::internal {
namespace endpoint {
namespace payment {

const char kDevelopment[] = "https://payment.rewards.hns.software";
const char kStaging[] = "https://payment.rewards.hnssoftware.com";
const char kProduction[] = "https://payment.rewards.hns.com";

std::string GetServerUrl(const std::string& path) {
  DCHECK(!path.empty());

  std::string url;
  switch (_environment) {
    case mojom::Environment::DEVELOPMENT:
      url = kDevelopment;
      break;
    case mojom::Environment::STAGING:
      url = kStaging;
      break;
    case mojom::Environment::PRODUCTION:
      url = kProduction;
      break;
  }

  return url + path;
}

}  // namespace payment
}  // namespace endpoint
}  // namespace hns_rewards::internal
