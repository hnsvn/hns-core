/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMMON_EXTENSIONS_HNS_EXTENSIONS_API_PROVIDER_H_
#define HNS_COMMON_EXTENSIONS_HNS_EXTENSIONS_API_PROVIDER_H_

#include <string>

#include "extensions/common/extensions_api_provider.h"

namespace extensions {

class HnsExtensionsAPIProvider : public ExtensionsAPIProvider {
 public:
  HnsExtensionsAPIProvider();
  HnsExtensionsAPIProvider(const HnsExtensionsAPIProvider&) = delete;
  HnsExtensionsAPIProvider& operator=(const HnsExtensionsAPIProvider&) =
      delete;
  ~HnsExtensionsAPIProvider() override;

  // ExtensionsAPIProvider:
  void AddAPIFeatures(FeatureProvider* provider) override;
  void AddManifestFeatures(FeatureProvider* provider) override;
  void AddPermissionFeatures(FeatureProvider* provider) override;
  void AddBehaviorFeatures(FeatureProvider* provider) override;
  void AddAPIJSONSources(JSONFeatureProviderSource* json_source) override;
  bool IsAPISchemaGenerated(const std::string& name) override;
  base::StringPiece GetAPISchema(const std::string& name) override;
  void RegisterPermissions(PermissionsInfo* permissions_info) override;
  void RegisterManifestHandlers() override;

 private:
};

}  // namespace extensions

#endif  // HNS_COMMON_EXTENSIONS_HNS_EXTENSIONS_API_PROVIDER_H_
