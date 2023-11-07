// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_TOR_SNOWFLAKE_EXTENSION_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_TOR_SNOWFLAKE_EXTENSION_HANDLER_H_

#include <string>

#include "base/memory/scoped_refptr.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "chrome/common/extensions/webstore_install_result.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_registry_observer.h"

class HnsTorSnowflakeExtensionHandler
    : public settings::SettingsPageUIHandler,
      public extensions::ExtensionRegistryObserver {
 public:
  HnsTorSnowflakeExtensionHandler();
  HnsTorSnowflakeExtensionHandler(const HnsTorSnowflakeExtensionHandler&) =
      delete;
  HnsTorSnowflakeExtensionHandler& operator=(
      const HnsTorSnowflakeExtensionHandler&) = delete;
  ~HnsTorSnowflakeExtensionHandler() override;

 private:
  // SettingsPageUIHandler:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  // extensions::ExtensionRegistryObserver:
  void OnExtensionLoaded(content::BrowserContext* browser_context,
                         const extensions::Extension* extension) override;
  void OnExtensionUnloaded(content::BrowserContext* browser_context,
                           const extensions::Extension* extension,
                           extensions::UnloadedExtensionReason reason) override;
  void OnExtensionInstalled(content::BrowserContext* browser_context,
                            const extensions::Extension* extension,
                            bool is_update) override;
  void OnExtensionUninstalled(content::BrowserContext* browser_context,
                              const extensions::Extension* extension,
                              extensions::UninstallReason reason) override;

  extensions::ExtensionRegistry* GetExtensionRegistry();

  bool IsTorAllowedByPolicy();
  void IsSnowflakeExtensionAllowed(const base::Value::List& args);
  void IsSnowflakeExtensionEnabled(const base::Value::List& args);
  void EnableSnowflakeExtension(const base::Value::List& args);
  void OnSnowflakeExtensionInstalled(
      base::Value js_callback,
      bool success,
      const std::string& error,
      extensions::webstore_install::Result result);

  base::ScopedObservation<extensions::ExtensionRegistry,
                          extensions::ExtensionRegistryObserver>
      observation_{this};
  scoped_refptr<class SnowflakeWebstoreInstaller> installer_;

  base::WeakPtrFactory<HnsTorSnowflakeExtensionHandler> weak_factory_{this};
};

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_TOR_SNOWFLAKE_EXTENSION_HANDLER_H_
