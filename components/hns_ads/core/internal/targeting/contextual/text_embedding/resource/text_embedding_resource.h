/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_CONTEXTUAL_TEXT_EMBEDDING_RESOURCE_TEXT_EMBEDDING_RESOURCE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_CONTEXTUAL_TEXT_EMBEDDING_RESOURCE_TEXT_EMBEDDING_RESOURCE_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/common/resources/resource_parsing_error_or.h"
#include "hns/components/hns_ads/core/internal/ml/pipeline/text_processing/embedding_processing.h"
#include "hns/components/hns_ads/core/public/client/ads_client_notifier_observer.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

class TextEmbeddingResource final : public AdsClientNotifierObserver {
 public:
  TextEmbeddingResource();

  TextEmbeddingResource(const TextEmbeddingResource&) = delete;
  TextEmbeddingResource& operator=(const TextEmbeddingResource&) = delete;

  TextEmbeddingResource(TextEmbeddingResource&&) noexcept = delete;
  TextEmbeddingResource& operator=(TextEmbeddingResource&&) noexcept = delete;

  ~TextEmbeddingResource() override;

  bool IsInitialized() const {
    return static_cast<bool>(embedding_processing_);
  }

  const absl::optional<ml::pipeline::EmbeddingProcessing>& get() const {
    return embedding_processing_;
  }

 private:
  void MaybeLoad();
  void MaybeLoadOrReset();

  bool DidLoad() const { return did_load_; }
  void Load();
  void LoadCallback(
      ResourceParsingErrorOr<ml::pipeline::EmbeddingProcessing> result);

  void MaybeReset();
  void Reset();

  // AdsClientNotifierObserver:
  void OnNotifyLocaleDidChange(const std::string& locale) override;
  void OnNotifyPrefDidChange(const std::string& path) override;
  void OnNotifyDidUpdateResourceComponent(const std::string& manifest_version,
                                          const std::string& id) override;

  absl::optional<ml::pipeline::EmbeddingProcessing> embedding_processing_;

  bool did_load_ = false;
  absl::optional<std::string> manifest_version_;

  base::WeakPtrFactory<TextEmbeddingResource> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_CONTEXTUAL_TEXT_EMBEDDING_RESOURCE_TEXT_EMBEDDING_RESOURCE_H_
