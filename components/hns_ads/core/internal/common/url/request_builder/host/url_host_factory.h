/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_URL_REQUEST_BUILDER_HOST_URL_HOST_FACTORY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_URL_REQUEST_BUILDER_HOST_URL_HOST_FACTORY_H_

#include <memory>

#include "hns/components/hns_ads/core/internal/common/url/request_builder/host/hosts/url_host_interface.h"
#include "hns/components/hns_ads/core/internal/common/url/request_builder/host/url_host_types.h"

namespace hns_ads {

class UrlHostFactory final {
 public:
  static std::unique_ptr<UrlHostInterface> Build(UrlHostType type);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_URL_REQUEST_BUILDER_HOST_URL_HOST_FACTORY_H_
