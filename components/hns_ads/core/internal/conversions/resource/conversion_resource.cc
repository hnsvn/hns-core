/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource.h"

#include <utility>

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/common/resources/country_components.h"
#include "hns/components/hns_ads/core/internal/common/resources/resources_util_impl.h"
#include "hns/components/hns_ads/core/internal/conversions/conversions_feature.h"
#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource_constants.h"
#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource_info.h"

namespace hns_ads {

ConversionResource::ConversionResource() {
  AdsClientHelper::AddObserver(this);
}

ConversionResource::~ConversionResource() {
  AdsClientHelper::RemoveObserver(this);
}

///////////////////////////////////////////////////////////////////////////////

void ConversionResource::Load() {
  LoadAndParseResource(kConversionResourceId, kConversionResourceVersion.Get(),
                       base::BindOnce(&ConversionResource::LoadCallback,
                                      weak_factory_.GetWeakPtr()));
}

void ConversionResource::LoadCallback(
    ResourceParsingErrorOr<ConversionResourceInfo> result) {
  if (!result.has_value()) {
    BLOG(0, "Failed to initialize " << kConversionResourceId
                                    << " conversion resource ("
                                    << result.error() << ")");
    is_initialized_ = false;
    return;
  }

  if (result.value().version == 0) {
    BLOG(1, kConversionResourceId << " conversion resource is not available");
    is_initialized_ = false;
    return;
  }

  BLOG(1, "Successfully loaded " << kConversionResourceId
                                 << " conversion resource");

  conversion_resource_ = std::move(result).value();

  is_initialized_ = true;

  BLOG(1, "Successfully initialized " << kConversionResourceId
                                      << " conversion resource version "
                                      << kConversionResourceVersion.Get());
}

void ConversionResource::OnNotifyLocaleDidChange(
    const std::string& /*locale*/) {
  Load();
}

void ConversionResource::OnNotifyDidUpdateResourceComponent(
    const std::string& /*manifest_version*/,
    const std::string& id) {
  if (IsValidCountryComponentId(id)) {
    Load();
  }
}

}  // namespace hns_ads
