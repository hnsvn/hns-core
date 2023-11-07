/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_PROMOTED_CONTENT_AD_PROMOTED_CONTENT_AD_HANDLER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_PROMOTED_CONTENT_AD_PROMOTED_CONTENT_AD_HANDLER_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/promoted_content_ads/promoted_content_ad_event_handler.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/promoted_content_ads/promoted_content_ad_event_handler_delegate.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ads_callback.h"

namespace hns_ads {

class Account;
class Transfer;
struct PromotedContentAdInfo;

class PromotedContentAdHandler final
    : public PromotedContentAdEventHandlerDelegate {
 public:
  PromotedContentAdHandler(Account& account, Transfer& transfer);

  PromotedContentAdHandler(const PromotedContentAdHandler&) = delete;
  PromotedContentAdHandler& operator=(const PromotedContentAdHandler&) = delete;

  PromotedContentAdHandler(PromotedContentAdHandler&&) noexcept = delete;
  PromotedContentAdHandler& operator=(PromotedContentAdHandler&&) noexcept =
      delete;

  ~PromotedContentAdHandler() override;

  void TriggerEvent(const std::string& placement_id,
                    const std::string& creative_instance_id,
                    mojom::PromotedContentAdEventType event_type,
                    TriggerAdEventCallback callback);

 private:
  void TriggerServedEventCallback(const std::string& creative_instance_id,
                                  TriggerAdEventCallback callback,
                                  bool success,
                                  const std::string& placement_id,
                                  mojom::PromotedContentAdEventType event_type);

  // PromotedContentAdEventHandlerDelegate:
  void OnDidFirePromotedContentAdServedEvent(
      const PromotedContentAdInfo& ad) override;
  void OnDidFirePromotedContentAdViewedEvent(
      const PromotedContentAdInfo& ad) override;
  void OnDidFirePromotedContentAdClickedEvent(
      const PromotedContentAdInfo& ad) override;

  const raw_ref<Account> account_;
  const raw_ref<Transfer> transfer_;

  PromotedContentAdEventHandler event_handler_;

  base::WeakPtrFactory<PromotedContentAdHandler> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_PROMOTED_CONTENT_AD_PROMOTED_CONTENT_AD_HANDLER_H_
