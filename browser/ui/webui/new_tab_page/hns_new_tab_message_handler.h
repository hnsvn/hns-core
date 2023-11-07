// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_MESSAGE_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_MESSAGE_HANDLER_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "hns/components/hns_ads/browser/ads_service.h"
#include "components/prefs/pref_change_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"

class Profile;

namespace base {
class Time;
class Value;
}  //  namespace base

namespace content {
class WebUIDataSource;
}

class PrefRegistrySimple;
class PrefService;

// TODO(simonhong): Migrate to hns_new_tab_page.mojom.
// Handles messages to and from the New Tab Page javascript
class HnsNewTabMessageHandler : public content::WebUIMessageHandler,
                                  public hns_ads::AdsServiceObserver {
 public:
  HnsNewTabMessageHandler(Profile* profile, bool was_invisible_and_restored);
  HnsNewTabMessageHandler(const HnsNewTabMessageHandler&) = delete;
  HnsNewTabMessageHandler& operator=(const HnsNewTabMessageHandler&) =
      delete;
  ~HnsNewTabMessageHandler() override;

  static void RegisterLocalStatePrefs(PrefRegistrySimple* local_state);
  static void RecordInitialP3AValues(PrefService* local_state);
  static bool CanPromptHnsTalk();
  static bool CanPromptHnsTalk(base::Time now);
  static HnsNewTabMessageHandler* Create(
      content::WebUIDataSource* html_source,
      Profile* profile,
      bool was_invisible_and_restored);

 private:
  // WebUIMessageHandler implementation.
  void RegisterMessages() override;
  void OnJavascriptAllowed() override;
  void OnJavascriptDisallowed() override;

  void HandleGetPreferences(const base::Value::List& args);
  void HandleGetStats(const base::Value::List& args);
  void HandleGetPrivateProperties(const base::Value::List& args);
  void HandleGetNewTabAdsData(const base::Value::List& args);
  void HandleSaveNewTabPagePref(const base::Value::List& args);
  void HandleToggleAlternativeSearchEngineProvider(
      const base::Value::List& args);
  void HandleRegisterNewTabPageView(const base::Value::List& args);
  void HandleBrandedWallpaperLogoClicked(const base::Value::List& args);
  void HandleGetWallpaperData(const base::Value::List& args);
  void HandleCustomizeClicked(const base::Value::List& args);

  void OnStatsChanged();
  void OnPreferencesChanged();
  void OnPrivatePropertiesChanged();

  base::Value::Dict GetAdsDataDictionary() const;

  // hns_ads::AdsServiceObserver:
  void OnNeedsBrowserUpgradeToServeAds() override;

  PrefChangeRegistrar pref_change_registrar_;
  // Weak pointer.
  raw_ptr<Profile> profile_ = nullptr;
  raw_ptr<hns_ads::AdsService> ads_service_ = nullptr;

  bool was_invisible_and_restored_ = false;

  base::ScopedObservation<hns_ads::AdsService, hns_ads::AdsServiceObserver>
      ads_service_observation_{this};

  base::WeakPtrFactory<HnsNewTabMessageHandler> weak_ptr_factory_;
};

#endif  // HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_MESSAGE_HANDLER_H_
