/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/hns_extension_provider.h"

#include <string>

#include "extensions/common/constants.h"

namespace extensions {

HnsExtensionProvider::HnsExtensionProvider() = default;

HnsExtensionProvider::~HnsExtensionProvider() = default;

std::string HnsExtensionProvider::GetDebugPolicyProviderName() const {
#if defined(NDEBUG)
  NOTREACHED();
  return std::string();
#else
  return "Hns Extension Provider";
#endif
}

bool HnsExtensionProvider::MustRemainInstalled(const Extension* extension,
                                                 std::u16string* error) const {
  return extension->id() == hns_extension_id;
}

}  // namespace extensions
