/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_URL_REQUEST_BUILDERS_CREATE_REWARD_CONFIRMATION_URL_REQUEST_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_URL_REQUEST_BUILDERS_CREATE_REWARD_CONFIRMATION_URL_REQUEST_BUILDER_H_

#include <string>

#include "hns/components/hns_ads/core/internal/account/confirmations/confirmation_info.h"
#include "hns/components/hns_ads/core/internal/common/url/request_builder/url_request_builder_interface.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

class GURL;

namespace hns_ads {

class CreateRewardConfirmationUrlRequestBuilder final
    : public UrlRequestBuilderInterface {
 public:
  explicit CreateRewardConfirmationUrlRequestBuilder(
      ConfirmationInfo confirmation);

  mojom::UrlRequestInfoPtr Build() override;

 private:
  GURL BuildUrl() const;

  std::string BuildBody() const;

  ConfirmationInfo confirmation_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_URL_REQUEST_BUILDERS_CREATE_REWARD_CONFIRMATION_URL_REQUEST_BUILDER_H_
