/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/renderer/worker_content_settings_client.h"

#include "hns/components/hns_shields/common/hns_shield_utils.h"
#include "components/content_settings/renderer/content_settings_agent_impl.h"
#include "net/base/features.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"

HnsFarblingLevel WorkerContentSettingsClient::GetHnsFarblingLevel() {
  ContentSetting setting = CONTENT_SETTING_DEFAULT;
  if (content_setting_rules_) {
    const GURL& primary_url = top_frame_origin_.GetURL();
    for (const auto& rule : content_setting_rules_->hns_shields_rules) {
      if (rule.primary_pattern.Matches(primary_url)) {
        setting = rule.GetContentSetting();
        break;
      }
    }
    if (setting == CONTENT_SETTING_BLOCK) {
      // Hns Shields is down
      setting = CONTENT_SETTING_ALLOW;
    } else {
      // Hns Shields is up, so check fingerprinting rules
      setting = hns_shields::GetHnsFPContentSettingFromRules(
          content_setting_rules_->fingerprinting_rules, primary_url);
    }
  }
  if (setting == CONTENT_SETTING_BLOCK) {
    return HnsFarblingLevel::MAXIMUM;
  } else if (setting == CONTENT_SETTING_ALLOW) {
    return HnsFarblingLevel::OFF;
  } else {
    return HnsFarblingLevel::BALANCED;
  }
}

blink::WebSecurityOrigin
WorkerContentSettingsClient::GetEphemeralStorageOriginSync() {
  if (!base::FeatureList::IsEnabled(net::features::kHnsEphemeralStorage))
    return {};

  if (is_unique_origin_)
    return {};

  // If first party ephemeral storage is enabled, we should always ask the
  // browser if a worker should use ephemeral storage or not.
  if (!base::FeatureList::IsEnabled(
          net::features::kHnsFirstPartyEphemeralStorage) &&
      net::registry_controlled_domains::SameDomainOrHost(
          top_frame_origin_, document_origin_,
          net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES)) {
    return {};
  }

  EnsureContentSettingsManager();

  absl::optional<url::Origin> optional_ephemeral_storage_origin;
  content_settings_manager_->AllowEphemeralStorageAccess(
      render_frame_id_, document_origin_, site_for_cookies_, top_frame_origin_,
      &optional_ephemeral_storage_origin);
  // Don't cache the value intentionally as other WorkerContentSettingsClient
  // methods do.
  return blink::WebSecurityOrigin(
      optional_ephemeral_storage_origin
          ? blink::WebSecurityOrigin(*optional_ephemeral_storage_origin)
          : blink::WebSecurityOrigin());
}

bool WorkerContentSettingsClient::HasContentSettingsRules() const {
  return content_setting_rules_.get();
}

#include "src/chrome/renderer/worker_content_settings_client.cc"
