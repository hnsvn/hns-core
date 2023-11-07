/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_CONVERSIONS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_CONVERSIONS_H_

#include <cstdint>
#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/conversion_queue.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/conversion_queue_delegate.h"
#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource.h"
#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_info.h"
#include "hns/components/hns_ads/core/internal/tabs/tab_manager_observer.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class GURL;

namespace hns_ads {

class ConversionsObserver;
struct ConversionInfo;
struct VerifiableConversionInfo;

class Conversions final : public ConversionQueueDelegate,
                          public TabManagerObserver {
 public:
  Conversions();

  Conversions(const Conversions&) = delete;
  Conversions& operator=(const Conversions&) = delete;

  Conversions(Conversions&&) noexcept = delete;
  Conversions& operator=(Conversions&&) noexcept = delete;

  ~Conversions() override;

  void AddObserver(ConversionsObserver* observer);
  void RemoveObserver(ConversionsObserver* observer);

  // Examine potential view-through or click-through conversions through various
  // channels, such as URL redirects or HTML pages.
  void MaybeConvert(const std::vector<GURL>& redirect_chain,
                    const std::string& html);

 private:
  void GetCreativeSetConversions(const std::vector<GURL>& redirect_chain,
                                 const std::string& html);
  void GetCreativeSetConversionsCallback(
      const std::vector<GURL>& redirect_chain,
      const std::string& html,
      bool success,
      const CreativeSetConversionList& creative_set_conversions);

  void GetAdEvents(const std::vector<GURL>& redirect_chain,
                   const std::string& html,
                   const CreativeSetConversionList& creative_set_conversions);
  void GetAdEventsCallback(
      const std::vector<GURL>& redirect_chain,
      const std::string& html,
      const CreativeSetConversionList& creative_set_conversions,
      bool success,
      const AdEventList& ad_events);

  void CheckForConversions(
      const std::vector<GURL>& redirect_chain,
      const std::string& html,
      const CreativeSetConversionList& creative_set_conversions,
      const AdEventList& ad_events);
  void Convert(
      const AdEventInfo& ad_event,
      const absl::optional<VerifiableConversionInfo>& verifiable_conversion);
  void ConvertCallback(
      const AdEventInfo& ad_event,
      const absl::optional<VerifiableConversionInfo>& verifiable_conversion,
      bool success);

  void NotifyDidConvertAd(const ConversionInfo& conversion) const;
  void NotifyFailedToConvertAd(const std::string& creative_instance_id) const;

  // ConversionQueueDelegate:
  void OnDidAddConversionToQueue(const ConversionInfo& conversion) override;
  void OnFailedToAddConversionToQueue(
      const ConversionInfo& conversion) override;
  void OnWillProcessConversionQueue(const ConversionInfo& conversion,
                                    base::Time process_at) override;
  void OnDidProcessConversionQueue(const ConversionInfo& conversion) override;
  void OnFailedToProcessConversionQueue(
      const ConversionInfo& conversion) override;
  void OnDidExhaustConversionQueue() override;

  // TabManagerObserver:
  void OnHtmlContentDidChange(int32_t tab_id,
                              const std::vector<GURL>& redirect_chain,
                              const std::string& html) override;

  base::ObserverList<ConversionsObserver> observers_;

  ConversionResource resource_;

  ConversionQueue queue_;

  base::WeakPtrFactory<Conversions> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_CONVERSIONS_H_
