/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/common/extensions/hns_extensions_api_provider.h"

#include "hns/common/extensions/api/generated_includes.h"
#include "extensions/common/features/json_feature_provider_source.h"
#include "extensions/common/permissions/permissions_info.h"

namespace extensions {

HnsExtensionsAPIProvider::HnsExtensionsAPIProvider() = default;
HnsExtensionsAPIProvider::~HnsExtensionsAPIProvider() = default;

void HnsExtensionsAPIProvider::AddAPIFeatures(FeatureProvider* provider) {
  AddHnsAPIFeatures(provider);
}

void HnsExtensionsAPIProvider::AddManifestFeatures(
    FeatureProvider* provider) {
  AddHnsManifestFeatures(provider);
}

void HnsExtensionsAPIProvider::AddPermissionFeatures(
    FeatureProvider* provider) {
  AddHnsPermissionFeatures(provider);
}

void HnsExtensionsAPIProvider::AddBehaviorFeatures(
    FeatureProvider* provider) {
  // No hns-specific behavior features.
}

void HnsExtensionsAPIProvider::AddAPIJSONSources(
    JSONFeatureProviderSource* json_source) {
  json_source->LoadJSON(IDR_HNS_EXTENSION_API_FEATURES);
}

bool HnsExtensionsAPIProvider::IsAPISchemaGenerated(
    const std::string& name) {
  return api::HnsGeneratedSchemas::IsGenerated(name);
}

base::StringPiece HnsExtensionsAPIProvider::GetAPISchema(
    const std::string& name) {
  return api::HnsGeneratedSchemas::Get(name);
}

void HnsExtensionsAPIProvider::RegisterPermissions(
    PermissionsInfo* permissions_info) {
  // No hns-specific permissions.
}

void HnsExtensionsAPIProvider::RegisterManifestHandlers() {
  // No hns-specific manifest handlers.
}

}  // namespace extensions
