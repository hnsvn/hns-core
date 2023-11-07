/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_RESOURCE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_RESOURCE_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/common/resources/resource_parsing_error_or.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_info.h"
#include "hns/components/hns_ads/core/public/client/ads_client_notifier_observer.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

class PurchaseIntentResource final : public AdsClientNotifierObserver {
 public:
  PurchaseIntentResource();

  PurchaseIntentResource(const PurchaseIntentResource&) = delete;
  PurchaseIntentResource& operator=(const PurchaseIntentResource&) = delete;

  PurchaseIntentResource(PurchaseIntentResource&&) noexcept = delete;
  PurchaseIntentResource& operator=(PurchaseIntentResource&&) noexcept = delete;

  ~PurchaseIntentResource() override;

  bool IsInitialized() const { return static_cast<bool>(purchase_intent_); }

  const absl::optional<PurchaseIntentInfo>& get() const {
    return purchase_intent_;
  }

 private:
  void MaybeLoad();
  void MaybeLoadOrReset();

  bool DidLoad() const { return did_load_; }
  void Load();
  void LoadCallback(ResourceParsingErrorOr<PurchaseIntentInfo> result);

  void MaybeReset();
  void Reset();

  // AdsClientNotifierObserver:
  void OnNotifyLocaleDidChange(const std::string& locale) override;
  void OnNotifyPrefDidChange(const std::string& path) override;
  void OnNotifyDidUpdateResourceComponent(const std::string& manifest_version,
                                          const std::string& id) override;

  absl::optional<PurchaseIntentInfo> purchase_intent_;

  bool did_load_ = false;
  absl::optional<std::string> manifest_version_;

  base::WeakPtrFactory<PurchaseIntentResource> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_RESOURCE_H_
