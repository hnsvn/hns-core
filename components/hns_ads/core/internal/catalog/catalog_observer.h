/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_OBSERVER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_OBSERVER_H_

#include "base/observer_list_types.h"

namespace hns_ads {

struct CatalogInfo;

class CatalogObserver : public base::CheckedObserver {
 public:
  // Invoked when the catalog has updated.
  virtual void OnDidUpdateCatalog(const CatalogInfo& catalog) {}

  // Invoked when the catalog failed to update.
  virtual void OnFailedToUpdateCatalog() {}
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_OBSERVER_H_
