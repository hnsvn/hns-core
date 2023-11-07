/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/permissions/permission_manager_factory.h"

#include "hns/browser/geolocation/hns_geolocation_permission_context_delegate.h"
#include "hns/browser/permissions/permission_lifetime_manager_factory.h"
#include "hns/components/permissions/hns_permission_manager.h"
#include "hns/components/permissions/contexts/hns_google_sign_in_permission_context.h"
#include "hns/components/permissions/contexts/hns_localhost_permission_context.h"
#include "hns/components/permissions/contexts/hns_wallet_permission_context.h"
#include "hns/components/permissions/permission_lifetime_manager.h"
#include "components/permissions/features.h"

#define GeolocationPermissionContextDelegate \
  HnsGeolocationPermissionContextDelegate

#define BuildServiceInstanceForBrowserContext \
  BuildServiceInstanceForBrowserContext_ChromiumImpl

#include "src/chrome/browser/permissions/permission_manager_factory.cc"

#undef GeolocationPermissionContextDelegate
#undef BuildServiceInstanceForBrowserContext

std::unique_ptr<KeyedService>
PermissionManagerFactory::BuildServiceInstanceForBrowserContext(
    content::BrowserContext* context) const {
  Profile* profile = Profile::FromBrowserContext(context);
  auto permission_contexts = CreatePermissionContexts(profile);

  permission_contexts[ContentSettingsType::HNS_ETHEREUM] =
      std::make_unique<permissions::HnsWalletPermissionContext>(
          profile, ContentSettingsType::HNS_ETHEREUM);
  permission_contexts[ContentSettingsType::HNS_SOLANA] =
      std::make_unique<permissions::HnsWalletPermissionContext>(
          profile, ContentSettingsType::HNS_SOLANA);
  permission_contexts[ContentSettingsType::HNS_GOOGLE_SIGN_IN] =
      std::make_unique<permissions::HnsGoogleSignInPermissionContext>(
          profile);
  permission_contexts[ContentSettingsType::HNS_LOCALHOST_ACCESS] =
      std::make_unique<permissions::HnsLocalhostPermissionContext>(profile);

  if (base::FeatureList::IsEnabled(
          permissions::features::kPermissionLifetime)) {
    auto factory =
        base::BindRepeating(&PermissionLifetimeManagerFactory::GetForProfile);
    for (auto& permission_context : permission_contexts) {
      permission_context.second->SetPermissionLifetimeManagerFactory(factory);
    }
  }

  return std::make_unique<permissions::HnsPermissionManager>(
      profile, std::move(permission_contexts));
}
