/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_rewards/extension_rewards_service_observer.h"

#include <utility>
#include <string>
#include <vector>

#include "base/base64.h"
#include "hns/common/extensions/api/hns_rewards.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile.h"
#include "extensions/browser/event_router.h"

namespace hns_rewards {

ExtensionRewardsServiceObserver::ExtensionRewardsServiceObserver(
    Profile* profile)
    : profile_(profile) {
}

ExtensionRewardsServiceObserver::~ExtensionRewardsServiceObserver() = default;

void ExtensionRewardsServiceObserver::OnRewardsInitialized(
    RewardsService* rewards_service) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::Initialized::kEventName,
      extensions::api::hns_rewards::Initialized::Create(0)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnRewardsWalletCreated() {
  if (auto* event_router = extensions::EventRouter::Get(profile_)) {
    event_router->BroadcastEvent(std::make_unique<extensions::Event>(
        extensions::events::HNS_START,
        extensions::api::hns_rewards::OnRewardsWalletCreated::kEventName,
        base::Value::List()));
  }
}

void ExtensionRewardsServiceObserver::OnPanelPublisherInfo(
    RewardsService* rewards_service,
    const hns_rewards::mojom::Result result,
    const hns_rewards::mojom::PublisherInfo* info,
    uint64_t windowId) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router || !info) {
    return;
  }

  extensions::api::hns_rewards::OnPublisherData::Publisher publisher;

  publisher.percentage = info->percent;
  publisher.status = static_cast<int>(info->status);
  publisher.excluded =
      info->excluded == hns_rewards::mojom::PublisherExclude::EXCLUDED;
  publisher.name = info->name;
  publisher.url = info->url;
  publisher.provider = info->provider;
  publisher.fav_icon_url = info->favicon_url;
  publisher.publisher_key = info->id;

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_ON_PUBLISHER_DATA,
      extensions::api::hns_rewards::OnPublisherData::kEventName,
      extensions::api::hns_rewards::OnPublisherData::Create(windowId,
                                                              publisher)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnFetchPromotions(
    RewardsService* rewards_service,
    const hns_rewards::mojom::Result result,
    const std::vector<hns_rewards::mojom::PromotionPtr>& list) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::vector<extensions::api::hns_rewards::Promotion> promotions;

  for (const auto& item : list) {
    extensions::api::hns_rewards::Promotion promotion;
    promotion.promotion_id = item->id;
    promotion.type = static_cast<int>(item->type);
    promotion.status = static_cast<int>(item->status);
    promotion.created_at = item->created_at;
    promotion.claimable_until = item->claimable_until;
    promotion.expires_at = item->expires_at;
    promotion.amount = item->approximate_value;
    promotions.emplace_back(std::move(promotion));
  }

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnPromotions::kEventName,
      extensions::api::hns_rewards::OnPromotions::Create(
          static_cast<int>(result), promotions)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnPromotionFinished(
    RewardsService* rewards_service,
    const hns_rewards::mojom::Result result,
    hns_rewards::mojom::PromotionPtr promotion) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router || result != hns_rewards::mojom::Result::OK) {
    return;
  }

  extensions::api::hns_rewards::OnPromotionFinish::
        Promotion promotion_api;

  promotion_api.promotion_id = promotion->id;
  promotion_api.type = static_cast<int>(promotion->type);
  promotion_api.status = static_cast<int>(promotion->status);
  promotion_api.expires_at = promotion->expires_at;
  promotion_api.amount = promotion->approximate_value;

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnPromotionFinish::kEventName,
      extensions::api::hns_rewards::OnPromotionFinish::Create(
          static_cast<int>(result), promotion_api)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnPublisherListNormalized(
    RewardsService* rewards_service,
    std::vector<hns_rewards::mojom::PublisherInfoPtr> list) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::vector<extensions::api::hns_rewards::OnPublisherListNormalized::
        PublishersType> publishers;

  for (const auto& item : list) {
    extensions::api::hns_rewards::OnPublisherListNormalized::PublishersType
        publisher;
    publisher.publisher_key = item->id;
    publisher.percentage = item->percent;
    publisher.status = static_cast<int>(item->status);
    publishers.emplace_back(std::move(publisher));
  }

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnPublisherListNormalized::kEventName,
      extensions::api::hns_rewards::OnPublisherListNormalized::Create(
          publishers)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnExcludedSitesChanged(
    RewardsService* rewards_service,
    std::string publisher_key,
    bool excluded) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  extensions::api::hns_rewards::OnExcludedSitesChanged::Properties result;
  result.publisher_key = publisher_key;
  result.excluded = excluded;

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnExcludedSitesChanged::kEventName,
      extensions::api::hns_rewards::OnExcludedSitesChanged::Create(result)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnRecurringTipSaved(
    RewardsService* rewards_service,
    bool success) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnRecurringTipSaved::kEventName,
      extensions::api::hns_rewards::OnRecurringTipSaved::Create(success)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnRecurringTipRemoved(
    RewardsService* rewards_service,
    bool success) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnRecurringTipRemoved::kEventName,
      extensions::api::hns_rewards::OnRecurringTipRemoved::Create(success)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnReconcileComplete(
    RewardsService* rewards_service,
    const hns_rewards::mojom::Result result,
    const std::string& contribution_id,
    const double amount,
    const hns_rewards::mojom::RewardsType type,
    const hns_rewards::mojom::ContributionProcessor processor) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  extensions::api::hns_rewards::OnReconcileComplete::Properties properties;
  properties.result = static_cast<int>(result);
  properties.type = static_cast<int>(type);

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnReconcileComplete::kEventName,
      extensions::api::hns_rewards::OnReconcileComplete::Create(properties)));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnExternalWalletConnected() {
  if (auto* event_router = extensions::EventRouter::Get(profile_)) {
    event_router->BroadcastEvent(std::make_unique<extensions::Event>(
        extensions::events::HNS_START,
        extensions::api::hns_rewards::OnExternalWalletConnected::kEventName,
        base::Value::List()));
  }
}

void ExtensionRewardsServiceObserver::OnExternalWalletLoggedOut() {
  if (auto* event_router = extensions::EventRouter::Get(profile_)) {
    event_router->BroadcastEvent(std::make_unique<extensions::Event>(
        extensions::events::HNS_START,
        extensions::api::hns_rewards::OnExternalWalletLoggedOut::kEventName,
        base::Value::List()));
  }
}

void ExtensionRewardsServiceObserver::OnExternalWalletDisconnected() {
  if (auto* event_router = extensions::EventRouter::Get(profile_)) {
    event_router->BroadcastEvent(std::make_unique<extensions::Event>(
        extensions::events::HNS_START,
        extensions::api::hns_rewards::OnExternalWalletDisconnected::
            kEventName,
        base::Value::List()));
  }
}

void ExtensionRewardsServiceObserver::OnUnblindedTokensReady(
      hns_rewards::RewardsService* rewards_service) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnUnblindedTokensReady::kEventName,
      base::Value::List()));
  event_router->BroadcastEvent(std::move(event));
}

void ExtensionRewardsServiceObserver::OnCompleteReset(const bool success) {
  auto* event_router = extensions::EventRouter::Get(profile_);
  if (!event_router) {
    return;
  }

  extensions::api::hns_rewards::OnCompleteReset::Properties properties;
  properties.success = success;

  std::unique_ptr<extensions::Event> event(new extensions::Event(
      extensions::events::HNS_START,
      extensions::api::hns_rewards::OnCompleteReset::kEventName,
      extensions::api::hns_rewards::OnCompleteReset::Create(properties)));
  event_router->BroadcastEvent(std::move(event));
}

}  // namespace hns_rewards
