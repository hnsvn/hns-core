/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_PROVIDER_H_
#define HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_PROVIDER_H_

#include <string>

#include "extensions/browser/management_policy.h"

namespace extensions {

class HnsExtensionProvider : public ManagementPolicy::Provider {
 public:
  HnsExtensionProvider();
  HnsExtensionProvider(const HnsExtensionProvider&) = delete;
  HnsExtensionProvider& operator=(const HnsExtensionProvider&) = delete;
  ~HnsExtensionProvider() override;
  std::string GetDebugPolicyProviderName() const override;
  bool MustRemainInstalled(const Extension* extension,
                           std::u16string* error) const override;
};

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_PROVIDER_H_
