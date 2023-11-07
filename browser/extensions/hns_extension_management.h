/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_MANAGEMENT_H_
#define HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_MANAGEMENT_H_

#include "base/scoped_observation.h"
#include "chrome/browser/extensions/extension_management.h"
#include "components/prefs/pref_change_registrar.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_registry_observer.h"

namespace extensions {

class HnsExtensionManagement : public ExtensionManagement,
                                 public ExtensionRegistryObserver {
 public:
  explicit HnsExtensionManagement(Profile* profile);
  HnsExtensionManagement(const HnsExtensionManagement&) = delete;
  HnsExtensionManagement& operator=(const HnsExtensionManagement&) = delete;
  ~HnsExtensionManagement() override;

 private:
  // ExtensionRegistryObserver implementation.
  void OnExtensionLoaded(
      content::BrowserContext* browser_context,
      const Extension* extension) override;
  void OnExtensionUnloaded(
      content::BrowserContext* browser_context,
      const Extension* extension,
      UnloadedExtensionReason reason) override;

  void OnTorDisabledChanged();
  void OnTorPluggableTransportChanged();
  void Cleanup(content::BrowserContext* browser_context);

  PrefChangeRegistrar local_state_pref_change_registrar_;

  base::ScopedObservation<ExtensionRegistry, ExtensionRegistryObserver>
      extension_registry_observer_{this};
};

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_HNS_EXTENSION_MANAGEMENT_H_
