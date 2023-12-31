/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_resource.h"

#include <utility>

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/common/resources/country_components.h"
#include "hns/components/hns_ads/core/internal/common/resources/resources_util_impl.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/purchase_intent_feature.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_resource_constants.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"

namespace hns_ads {

namespace {

bool DoesRequireResource() {
  return UserHasOptedInToHnsNewsAds() || UserHasOptedInToNotificationAds();
}

}  // namespace

PurchaseIntentResource::PurchaseIntentResource() {
  AdsClientHelper::AddObserver(this);
}

PurchaseIntentResource::~PurchaseIntentResource() {
  AdsClientHelper::RemoveObserver(this);
}

///////////////////////////////////////////////////////////////////////////////

void PurchaseIntentResource::MaybeLoad() {
  if (manifest_version_ && DoesRequireResource()) {
    Load();
  }
}

void PurchaseIntentResource::MaybeLoadOrReset() {
  DidLoad() ? MaybeReset() : MaybeLoad();
}

void PurchaseIntentResource::Load() {
  did_load_ = true;

  LoadAndParseResource(kPurchaseIntentResourceId,
                       kPurchaseIntentResourceVersion.Get(),
                       base::BindOnce(&PurchaseIntentResource::LoadCallback,
                                      weak_factory_.GetWeakPtr()));
}

void PurchaseIntentResource::LoadCallback(
    ResourceParsingErrorOr<PurchaseIntentInfo> result) {
  if (!result.has_value()) {
    return BLOG(0, "Failed to initialize " << kPurchaseIntentResourceId
                                           << " purchase intent resource ("
                                           << result.error() << ")");
  }

  if (result.value().version == 0) {
    return BLOG(1, kPurchaseIntentResourceId
                       << " purchase intent resource is not available");
  }

  BLOG(1, "Successfully loaded " << kPurchaseIntentResourceId
                                 << " purchase intent resource");

  purchase_intent_ = std::move(result).value();

  BLOG(1, "Successfully initialized " << kPurchaseIntentResourceId
                                      << " purchase intent resource version "
                                      << kPurchaseIntentResourceVersion.Get());
}

void PurchaseIntentResource::MaybeReset() {
  if (DidLoad() && !DoesRequireResource()) {
    Reset();
  }
}

void PurchaseIntentResource::Reset() {
  BLOG(1, "Reset " << kPurchaseIntentResourceId << " purchase intent resource");
  purchase_intent_.reset();
  did_load_ = false;
}

void PurchaseIntentResource::OnNotifyLocaleDidChange(
    const std::string& /*locale*/) {
  MaybeLoad();
}

void PurchaseIntentResource::OnNotifyPrefDidChange(const std::string& path) {
  if (path == hns_rewards::prefs::kEnabled ||
      path == prefs::kOptedInToNotificationAds ||
      path == hns_news::prefs::kHnsNewsOptedIn ||
      path == hns_news::prefs::kNewTabPageShowToday) {
    MaybeLoadOrReset();
  }
}

void PurchaseIntentResource::OnNotifyDidUpdateResourceComponent(
    const std::string& manifest_version,
    const std::string& id) {
  if (!IsValidCountryComponentId(id)) {
    return;
  }

  if (manifest_version == manifest_version_) {
    return;
  }

  manifest_version_ = manifest_version;

  MaybeLoad();
}

}  // namespace hns_ads
