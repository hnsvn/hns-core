/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_CLIENT_STATE_MANAGER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_CLIENT_STATE_MANAGER_H_

#include <map>
#include <string>

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/client_info.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/filtered_advertiser_info.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/filtered_category_info.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/flagged_ad_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_signal_history_info.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_classification/model/text_classification_alias.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ads_callback.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

class AdType;
struct AdContentInfo;
struct AdInfo;

class ClientStateManager final {
 public:
  ClientStateManager();

  ClientStateManager(const ClientStateManager&) = delete;
  ClientStateManager& operator=(const ClientStateManager&) = delete;

  ClientStateManager(ClientStateManager&&) noexcept = delete;
  ClientStateManager& operator=(ClientStateManager&&) noexcept = delete;

  ~ClientStateManager();

  static ClientStateManager& GetInstance();

  void Load(InitializeCallback callback);

  const FilteredAdvertiserList& GetFilteredAdvertisers() const;
  const FilteredCategoryList& GetFilteredCategories() const;
  const FlaggedAdList& GetFlaggedAds() const;

  void AppendHistory(const HistoryItemInfo& history_item);
  const HistoryItemList& GetHistory() const;

  void AppendToPurchaseIntentSignalHistoryForSegment(
      const std::string& segment,
      const PurchaseIntentSignalHistoryInfo& history);
  const PurchaseIntentSignalHistoryMap& GetPurchaseIntentSignalHistory() const;

  mojom::UserReactionType ToggleLikeAd(const AdContentInfo& ad_content);
  mojom::UserReactionType ToggleDislikeAd(const AdContentInfo& ad_content);
  mojom::UserReactionType GetUserReactionTypeForAdvertiser(
      const std::string& advertiser_id);

  mojom::UserReactionType ToggleLikeCategory(
      const CategoryContentInfo& category_content);
  mojom::UserReactionType ToggleDislikeCategory(
      const CategoryContentInfo& category_content);
  mojom::UserReactionType GetUserReactionTypeForSegment(
      const std::string& segment);

  bool ToggleSaveAd(const AdContentInfo& ad_content);

  bool ToggleMarkAdAsInappropriate(const AdContentInfo& ad_content);

  void UpdateSeenAd(const AdInfo& ad);
  const std::map<std::string, bool>& GetSeenAdsForType(const AdType& type);
  void ResetSeenAdsForType(const CreativeAdList& creative_ads,
                           const AdType& type);
  void ResetAllSeenAdsForType(const AdType& type);
  const std::map<std::string, bool>& GetSeenAdvertisersForType(
      const AdType& type);
  void ResetSeenAdvertisersForType(const CreativeAdList& creative_ads,
                                   const AdType& type);
  void ResetAllSeenAdvertisersForType(const AdType& type);

  void AppendTextClassificationProbabilitiesToHistory(
      const TextClassificationProbabilityMap& probabilities);
  const TextClassificationProbabilityList&
  GetTextClassificationProbabilitiesHistory() const;

 private:
  void Save();

  void LoadCallback(InitializeCallback callback,
                    const absl::optional<std::string>& json);

  [[nodiscard]] bool FromJson(const std::string& json);

  ClientInfo client_;

  bool is_initialized_ = false;

  base::WeakPtrFactory<ClientStateManager> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_CLIENT_STATE_MANAGER_H_
