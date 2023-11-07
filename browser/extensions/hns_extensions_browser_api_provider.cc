/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/hns_extensions_browser_api_provider.h"

#include "hns/common/extensions/api/generated_api_registration.h"
#include "extensions/browser/extension_function_registry.h"

namespace extensions {

HnsExtensionsBrowserAPIProvider::HnsExtensionsBrowserAPIProvider() =
    default;
HnsExtensionsBrowserAPIProvider::~HnsExtensionsBrowserAPIProvider() =
    default;

void HnsExtensionsBrowserAPIProvider::RegisterExtensionFunctions(
    ExtensionFunctionRegistry* registry) {
  // Generated APIs from Hns.
  api::HnsGeneratedFunctionRegistry::RegisterAll(registry);
}

}  // namespace extensions
