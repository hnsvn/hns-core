/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/hns_extensions_browser_client_impl.h"

#include <memory>
#include <tuple>

#include "hns/browser/extensions/hns_extensions_browser_api_provider.h"
#include "chrome/browser/extensions/chrome_component_extension_resource_manager.h"

namespace extensions {

HnsExtensionsBrowserClientImpl::HnsExtensionsBrowserClientImpl() {
  AddAPIProvider(std::make_unique<HnsExtensionsBrowserAPIProvider>());
  // ChromeComponentExtensionResourceManager's Data needs to be LazyInit'ed on
  // the UI thread (due to pdf_extension_util::AddStrings calling
  // g_browser_process->GetApplicationLocale() that has a DCHECK to that
  // regard).
  std::ignore = GetComponentExtensionResourceManager()
                    ->GetTemplateReplacementsForExtension("");
}

}  // namespace extensions
