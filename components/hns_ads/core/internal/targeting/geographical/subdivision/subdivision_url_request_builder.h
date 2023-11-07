/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_GEOGRAPHICAL_SUBDIVISION_SUBDIVISION_URL_REQUEST_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_GEOGRAPHICAL_SUBDIVISION_SUBDIVISION_URL_REQUEST_BUILDER_H_

#include "hns/components/hns_ads/core/internal/common/url/request_builder/url_request_builder_interface.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

class GetSubdivisionUrlRequestBuilder final
    : public UrlRequestBuilderInterface {
 public:
  mojom::UrlRequestInfoPtr Build() override;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_GEOGRAPHICAL_SUBDIVISION_SUBDIVISION_URL_REQUEST_BUILDER_H_
