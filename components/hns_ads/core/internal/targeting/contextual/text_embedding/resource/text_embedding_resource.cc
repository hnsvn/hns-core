/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/resource/text_embedding_resource.h"

#include <utility>

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/common/resources/language_components.h"
#include "hns/components/hns_ads/core/internal/common/resources/resources_util_impl.h"
#include "hns/components/hns_ads/core/internal/ml/pipeline/text_processing/embedding_processing.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/resource/text_embedding_resource_constants.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_feature.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"

namespace hns_ads {

namespace {

bool DoesRequireResource() {
  return UserHasOptedInToNotificationAds();
}

}  // namespace

TextEmbeddingResource::TextEmbeddingResource() {
  AdsClientHelper::AddObserver(this);
}

TextEmbeddingResource::~TextEmbeddingResource() {
  AdsClientHelper::RemoveObserver(this);
}

///////////////////////////////////////////////////////////////////////////////

void TextEmbeddingResource::MaybeLoad() {
  if (manifest_version_ && DoesRequireResource()) {
    Load();
  }
}

void TextEmbeddingResource::MaybeLoadOrReset() {
  DidLoad() ? MaybeReset() : MaybeLoad();
}

void TextEmbeddingResource::Load() {
  did_load_ = true;

  LoadAndParseResource(kTextEmbeddingResourceId,
                       kTextEmbeddingResourceVersion.Get(),
                       base::BindOnce(&TextEmbeddingResource::LoadCallback,
                                      weak_factory_.GetWeakPtr()));
}

void TextEmbeddingResource::LoadCallback(
    ResourceParsingErrorOr<ml::pipeline::EmbeddingProcessing> result) {
  if (!result.has_value()) {
    return BLOG(0, "Failed to initialize " << kTextEmbeddingResourceId
                                           << " text embedding resource ("
                                           << result.error() << ")");
  }

  if (!result.value().IsInitialized()) {
    return BLOG(1, kTextEmbeddingResourceId
                       << " text embedding resource is not available");
  }

  BLOG(1, "Successfully loaded " << kTextEmbeddingResourceId
                                 << " text embedding resource");

  embedding_processing_ = std::move(result).value();

  BLOG(1, "Successfully initialized " << kTextEmbeddingResourceId
                                      << " text embedding resource version "
                                      << kTextEmbeddingResourceVersion.Get());
}

void TextEmbeddingResource::MaybeReset() {
  if (DidLoad() && !DoesRequireResource()) {
    Reset();
  }
}

void TextEmbeddingResource::Reset() {
  BLOG(1, "Reset " << kTextEmbeddingResourceId << " text embedding resource");
  embedding_processing_.reset();
  did_load_ = false;
}

void TextEmbeddingResource::OnNotifyLocaleDidChange(
    const std::string& /*locale*/) {
  MaybeLoad();
}

void TextEmbeddingResource::OnNotifyPrefDidChange(const std::string& path) {
  if (path == hns_rewards::prefs::kEnabled ||
      path == prefs::kOptedInToNotificationAds) {
    MaybeLoadOrReset();
  }
}

void TextEmbeddingResource::OnNotifyDidUpdateResourceComponent(
    const std::string& manifest_version,
    const std::string& id) {
  if (!IsValidLanguageComponentId(id)) {
    return;
  }

  if (manifest_version == manifest_version_) {
    return;
  }

  manifest_version_ = manifest_version;

  MaybeLoad();
}

}  // namespace hns_ads
