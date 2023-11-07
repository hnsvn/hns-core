/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NEW_TAB_PAGE_AD_NEW_TAB_PAGE_AD_HANDLER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NEW_TAB_PAGE_AD_NEW_TAB_PAGE_AD_HANDLER_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/serving/new_tab_page_ad_serving.h"
#include "hns/components/hns_ads/core/internal/serving/new_tab_page_ad_serving_delegate.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/new_tab_page_ads/new_tab_page_ad_event_handler.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/new_tab_page_ads/new_tab_page_ad_event_handler_delegate.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ads_callback.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

class Account;
class AntiTargetingResource;
class SubdivisionTargeting;
class Transfer;
struct NewTabPageAdInfo;

class NewTabPageAdHandler final : public NewTabPageAdEventHandlerDelegate,
                                  public NewTabPageAdServingDelegate {
 public:
  NewTabPageAdHandler(Account& account,
                      Transfer& transfer,
                      const SubdivisionTargeting& subdivision_targeting,
                      const AntiTargetingResource& anti_targeting_resource);

  NewTabPageAdHandler(const NewTabPageAdHandler&) = delete;
  NewTabPageAdHandler& operator=(const NewTabPageAdHandler&) = delete;

  NewTabPageAdHandler(NewTabPageAdHandler&&) noexcept = delete;
  NewTabPageAdHandler& operator=(NewTabPageAdHandler&&) noexcept = delete;

  ~NewTabPageAdHandler() override;

  void MaybeServe(MaybeServeNewTabPageAdCallback callback);

  void TriggerEvent(const std::string& placement_id,
                    const std::string& creative_instance_id,
                    mojom::NewTabPageAdEventType event_type,
                    TriggerAdEventCallback callback);

 private:
  void MaybeServeCallback(MaybeServeNewTabPageAdCallback callback,
                          const absl::optional<NewTabPageAdInfo>& ad);

  void TriggerServedEventCallback(const std::string& creative_instance_id,
                                  TriggerAdEventCallback callback,
                                  bool success,
                                  const std::string& placement_id,
                                  mojom::NewTabPageAdEventType event_type);

  // NewTabPageAdServingDelegate:
  void OnOpportunityAroseToServeNewTabPageAd(
      const SegmentList& segments) override;
  void OnDidServeNewTabPageAd(const NewTabPageAdInfo& ad) override;

  // NewTabPageAdEventHandlerDelegate:
  void OnDidFireNewTabPageAdServedEvent(const NewTabPageAdInfo& ad) override;
  void OnDidFireNewTabPageAdViewedEvent(const NewTabPageAdInfo& ad) override;
  void OnDidFireNewTabPageAdClickedEvent(const NewTabPageAdInfo& ad) override;

  NewTabPageAdEventHandler event_handler_;

  const raw_ref<Account> account_;
  const raw_ref<Transfer> transfer_;

  NewTabPageAdServing serving_;

  base::WeakPtrFactory<NewTabPageAdHandler> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NEW_TAB_PAGE_AD_NEW_TAB_PAGE_AD_HANDLER_H_
