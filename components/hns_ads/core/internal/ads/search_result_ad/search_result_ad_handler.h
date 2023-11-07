/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_SEARCH_RESULT_AD_SEARCH_RESULT_AD_HANDLER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_SEARCH_RESULT_AD_SEARCH_RESULT_AD_HANDLER_H_

#include <string>

#include "base/containers/circular_deque.h"
#include "base/memory/raw_ref.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/search_result_ads/search_result_ad_event_handler.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/search_result_ads/search_result_ad_event_handler_delegate.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ads_callback.h"

namespace hns_ads {

class Account;
class Transfer;
struct SearchResultAdInfo;

class SearchResultAd final : public SearchResultAdEventHandlerDelegate {
 public:
  SearchResultAd(Account& account, Transfer& transfer);

  SearchResultAd(const SearchResultAd&) = delete;
  SearchResultAd& operator=(const SearchResultAd&) = delete;

  SearchResultAd(SearchResultAd&&) noexcept = delete;
  SearchResultAd& operator=(SearchResultAd&&) noexcept = delete;

  ~SearchResultAd() override;

  void TriggerEvent(mojom::SearchResultAdInfoPtr ad_mojom,
                    mojom::SearchResultAdEventType event_type,
                    TriggerAdEventCallback callback);

  static void DeferTriggeringOfAdViewedEvent();
  static void TriggerDeferredAdViewedEvent();

 private:
  void FireServedEventCallback(mojom::SearchResultAdInfoPtr ad_mojom,
                               TriggerAdEventCallback callback,
                               bool success,
                               const std::string& placement_id,
                               mojom::SearchResultAdEventType event_type);

  void MaybeTriggerAdViewedEventFromQueue(TriggerAdEventCallback callback);
  void FireAdViewedEventCallback(TriggerAdEventCallback callback,
                                 bool success,
                                 const std::string& placement_id,
                                 mojom::SearchResultAdEventType event_type);

  // SearchResultAdEventHandlerDelegate:
  void OnDidFireSearchResultAdServedEvent(
      const SearchResultAdInfo& ad) override;
  void OnDidFireSearchResultAdViewedEvent(
      const SearchResultAdInfo& ad) override;
  void OnDidFireSearchResultAdClickedEvent(
      const SearchResultAdInfo& ad) override;

  const raw_ref<Account> account_;
  const raw_ref<Transfer> transfer_;

  SearchResultAdEventHandler event_handler_;

  base::circular_deque<mojom::SearchResultAdInfoPtr> ad_viewed_event_queue_;

  bool trigger_ad_viewed_event_in_progress_ = false;

  base::WeakPtrFactory<SearchResultAd> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_SEARCH_RESULT_AD_SEARCH_RESULT_AD_HANDLER_H_
