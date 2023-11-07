/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_SHIELDS_DATA_CONTROLLER_H_
#define HNS_BROWSER_UI_HNS_SHIELDS_DATA_CONTROLLER_H_

#include <set>
#include <string>
#include <vector>

#include "base/observer_list.h"
#include "base/observer_list_types.h"
#include "base/scoped_observation.h"
#include "hns/components/hns_shields/common/hns_shields_panel.mojom.h"
#include "components/content_settings/core/browser/content_settings_observer.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/favicon/core/favicon_driver_observer.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

using hns_shields::mojom::AdBlockMode;
using hns_shields::mojom::CookieBlockMode;
using hns_shields::mojom::FingerprintMode;
using hns_shields::mojom::HttpsUpgradeMode;
using content::NavigationEntry;

namespace hns_shields {

// Per-tab class to manage Shields panel data
class HnsShieldsDataController
    : public content::WebContentsObserver,
      public content::WebContentsUserData<HnsShieldsDataController>,
      public content_settings::Observer,
      public favicon::FaviconDriverObserver {
 public:
  HnsShieldsDataController(const HnsShieldsDataController&) = delete;
  HnsShieldsDataController& operator=(const HnsShieldsDataController&) =
      delete;
  ~HnsShieldsDataController() override;

  class Observer : public base::CheckedObserver {
   public:
    virtual void OnResourcesChanged() = 0;
    virtual void OnFaviconUpdated() {}
    virtual void OnShieldsEnabledChanged() {}
  };

  void HandleItemBlocked(const std::string& block_type,
                         const std::string& subresource);
  void HandleItemAllowedOnce(const std::string& allowed_once_type,
                             const std::string& subresource);
  void ClearAllResourcesList();
  int GetTotalBlockedCount();
  std::vector<GURL> GetBlockedAdsList();
  std::vector<GURL> GetHttpRedirectsList();
  std::vector<GURL> GetBlockedJsList();
  std::vector<GURL> GetAllowedJsList();
  std::vector<GURL> GetFingerprintsList();
  bool GetHnsShieldsEnabled();
  void SetHnsShieldsEnabled(bool is_enabled);
  GURL GetCurrentSiteURL();
  GURL GetFaviconURL(bool refresh);

  AdBlockMode GetAdBlockMode();
  FingerprintMode GetFingerprintMode();
  CookieBlockMode GetCookieBlockMode();
  bool IsHnsShieldsManaged();
  bool IsForgetFirstPartyStorageFeatureEnabled() const;
  bool GetHTTPSEverywhereEnabled();
  HttpsUpgradeMode GetHttpsUpgradeMode();
  bool GetNoScriptEnabled();
  bool GetForgetFirstPartyStorageEnabled();
  void SetAdBlockMode(AdBlockMode mode);
  void SetFingerprintMode(FingerprintMode mode);
  void SetCookieBlockMode(CookieBlockMode mode);
  void SetHttpsUpgradeMode(HttpsUpgradeMode mode);
  void SetIsNoScriptEnabled(bool is_enabled);
  void SetIsHTTPSEverywhereEnabled(bool is_enabled);
  void SetForgetFirstPartyStorageEnabled(bool is_enabled);
  void AllowScriptsOnce(const std::vector<std::string>& origins);
  void BlockAllowedScripts(const std::vector<std::string>& origins);

  void AddObserver(Observer* obs);
  void RemoveObserver(Observer* obs);
  bool HasObserver(Observer* observer);

 private:
  friend class content::WebContentsUserData<HnsShieldsDataController>;

  explicit HnsShieldsDataController(content::WebContents* web_contents);

  // content::WebContentsObserver
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;
  void WebContentsDestroyed() override;

  // content_settings::Observer
  void OnContentSettingChanged(
      const ContentSettingsPattern& primary_pattern,
      const ContentSettingsPattern& secondary_pattern,
      ContentSettingsTypeSet content_type_set) override;

  // favicon::FaviconDriverObserver
  void OnFaviconUpdated(favicon::FaviconDriver* favicon_driver,
                        NotificationIconType notification_icon_type,
                        const GURL& icon_url,
                        bool icon_url_changed,
                        const gfx::Image& image) override;

  void ReloadWebContents();

  base::ObserverList<Observer> observer_list_;
  std::set<GURL> resource_list_blocked_ads_;
  std::set<GURL> resource_list_http_redirects_;
  std::set<GURL> resource_list_blocked_js_;
  std::set<GURL> resource_list_allowed_once_js_;
  std::set<GURL> resource_list_blocked_fingerprints_;
  base::ScopedObservation<HostContentSettingsMap, content_settings::Observer>
      observation_{this};

  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace hns_shields

#endif  // HNS_BROWSER_UI_HNS_SHIELDS_DATA_CONTROLLER_H_
