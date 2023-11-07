/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/extensions/component_extensions_allowlist/allowlist.h"

#define IsComponentExtensionAllowlisted IsComponentExtensionAllowlisted_ChromiumImpl  // NOLINT
#include "src/chrome/browser/extensions/component_extensions_allowlist/allowlist.cc"
#undef IsComponentExtensionAllowlisted

#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/components/hns_extension/grit/hns_extension.h"
#include "hns/components/hns_webtorrent/grit/hns_webtorrent_resources.h"
#include "components/grit/hns_components_resources.h"
#include "extensions/common/constants.h"

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#endif

namespace extensions {

  bool IsComponentExtensionAllowlisted(const std::string& extension_id) {
    const char* const kAllowed[] = {
      hns_extension_id,
#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
      ethereum_remote_client_extension_id,
#endif
      hns_webtorrent_extension_id
    };

    for (size_t i = 0; i < std::size(kAllowed); ++i) {
      if (extension_id == kAllowed[i])
        return true;
    }

    return IsComponentExtensionAllowlisted_ChromiumImpl(extension_id);
  }

  bool IsComponentExtensionAllowlisted(int manifest_resource_id) {
    switch (manifest_resource_id) {
      // Please keep the list in alphabetical order.
      case IDR_HNS_EXTENSION:
      case IDR_HNS_WEBTORRENT:
        return true;
    }

    return IsComponentExtensionAllowlisted_ChromiumImpl(manifest_resource_id);
  }

}  // namespace extensions
