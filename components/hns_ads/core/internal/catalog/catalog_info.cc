/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/catalog/catalog_info.h"

#include <tuple>

namespace hns_ads {

CatalogInfo::CatalogInfo() = default;

CatalogInfo::CatalogInfo(const CatalogInfo& other) = default;

CatalogInfo& CatalogInfo::operator=(const CatalogInfo& other) = default;

CatalogInfo::CatalogInfo(CatalogInfo&& other) noexcept = default;

CatalogInfo& CatalogInfo::operator=(CatalogInfo&& other) noexcept = default;

CatalogInfo::~CatalogInfo() = default;

bool CatalogInfo::operator==(const CatalogInfo& other) const {
  const auto tie = [](const CatalogInfo& catalog) {
    return std::tie(catalog.id, catalog.version, catalog.ping,
                    catalog.campaigns);
  };

  return tie(*this) == tie(other);
}

bool CatalogInfo::operator!=(const CatalogInfo& other) const {
  return !(*this == other);
}

}  // namespace hns_ads
