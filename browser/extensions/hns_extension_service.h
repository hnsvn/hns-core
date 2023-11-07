/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_SERVICE_H_
#define HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_SERVICE_H_

#include "chrome/browser/extensions/extension_service.h"

namespace extensions {

class HnsExtensionService : public ExtensionService {
 public:
  // Constructor stores pointers to |profile| and |extension_prefs| but
  // ownership remains at caller.
  HnsExtensionService(Profile* profile,
                        const base::CommandLine* command_line,
                        const base::FilePath& install_directory,
                        const base::FilePath& unpacked_install_directory,
                        ExtensionPrefs* extension_prefs,
                        Blocklist* blocklist,
                        bool autoupdate_enabled,
                        bool extensions_enabled,
                        base::OneShotEvent* ready);
  HnsExtensionService(const HnsExtensionService&) = delete;
  HnsExtensionService& operator=(const HnsExtensionService&) = delete;
  ~HnsExtensionService() override;

  void AddComponentExtension(const Extension* extension) override;
};

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_SERVICE_H_
