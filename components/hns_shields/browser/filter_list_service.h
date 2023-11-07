/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_FILTER_LIST_SERVICE_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_FILTER_LIST_SERVICE_H_

#include <string>
#include <vector>
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_shields/common/filter_list.mojom-forward.h"
#include "hns/components/hns_shields/common/filter_list.mojom.h"
#include "components/keyed_service/core/keyed_service.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote_set.h"

namespace hns_shields {
class AdBlockService;

// This class is not thread-safe and should have single owner
class FilterListService : public KeyedService,
                          public mojom::FilterListAndroidHandler {
 public:
  explicit FilterListService(AdBlockService* ad_block_service);
  ~FilterListService() override;

  mojo::PendingRemote<mojom::FilterListAndroidHandler> MakeRemote();
  void Bind(mojo::PendingReceiver<mojom::FilterListAndroidHandler> receiver);

  void IsFilterListEnabled(const std::string& filterListUuid,
                           IsFilterListEnabledCallback callback) override;
  void EnableFilter(const std::string& filterListUuid,
                    bool shouldEnableFilter) override;
  void GetSubscriptions(GetSubscriptionsCallback callback) override;
  void CreateSubscription(const GURL& subscription_url) override;
  void EnableSubscription(const GURL& sub_url, bool enabled) override;
  void DeleteSubscription(const GURL& sub_url) override;
  void GetFilterLists(GetFilterListsCallback callback) override;

 private:
  raw_ptr<AdBlockService> ad_block_service_ = nullptr;
  mojo::ReceiverSet<mojom::FilterListAndroidHandler> receivers_;
  base::WeakPtrFactory<FilterListService> discovery_weak_factory_{this};

  FilterListService(const FilterListService&) = delete;
  FilterListService& operator=(const FilterListService&) = delete;
};

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_FILTER_LIST_SERVICE_H_
