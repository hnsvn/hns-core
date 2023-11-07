/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_H_

#include <memory>
#include <string>

#include "base/observer_list.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_observer.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_delegate.h"
#include "hns/components/hns_ads/core/internal/database/database_manager_observer.h"
#include "hns/components/hns_ads/core/public/client/ads_client_notifier_observer.h"

namespace hns_ads {

class CatalogUrlRequest;
struct CatalogInfo;

class Catalog final : public AdsClientNotifierObserver,
                      public CatalogUrlRequestDelegate,
                      public DatabaseManagerObserver {
 public:
  Catalog();

  Catalog(const Catalog&) = delete;
  Catalog& operator=(const Catalog&) = delete;

  Catalog(Catalog&&) noexcept = delete;
  Catalog& operator=(Catalog&&) noexcept = delete;

  ~Catalog() override;

  void AddObserver(CatalogObserver* observer);
  void RemoveObserver(CatalogObserver* observer);

 private:
  void Initialize();

  void MaybeRequireCatalog();
  void InitializeCatalogUrlRequest();
  void ShutdownCatalogUrlRequest();

  void MaybeFetchCatalog() const;

  void NotifyDidUpdateCatalog(const CatalogInfo& catalog) const;
  void NotifyFailedToUpdateCatalog() const;

  // AdsClientNotifierObserver:
  void OnNotifyDidInitializeAds() override;
  void OnNotifyPrefDidChange(const std::string& path) override;

  // CatalogUrlRequestDelegate:
  void OnDidFetchCatalog(const CatalogInfo& catalog) override;
  void OnFailedToFetchCatalog() override;

  // DatabaseManagerObserver:
  void OnDidMigrateDatabase(int from_version, int to_version) override;

  base::ObserverList<CatalogObserver> observers_;

  std::unique_ptr<CatalogUrlRequest> catalog_url_request_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_H_
