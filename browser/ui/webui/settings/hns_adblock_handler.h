/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_ADBLOCK_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_ADBLOCK_HANDLER_H_

#include "base/scoped_observation.h"

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_shields/browser/ad_block_subscription_service_manager.h"
#include "hns/components/hns_shields/browser/ad_block_subscription_service_manager_observer.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"

class Profile;
using hns_shields::AdBlockSubscriptionServiceManager;
using hns_shields::AdBlockSubscriptionServiceManagerObserver;

class HnsAdBlockHandler : public settings::SettingsPageUIHandler,
                            public AdBlockSubscriptionServiceManagerObserver {
 public:
  HnsAdBlockHandler();
  HnsAdBlockHandler(const HnsAdBlockHandler&) = delete;
  HnsAdBlockHandler& operator=(const HnsAdBlockHandler&) = delete;
  ~HnsAdBlockHandler() override;

 private:
  // SettingsPageUIHandler overrides
  void RegisterMessages() override;

  // hns_shields::AdblockSubscriptionServiceManagerObserver overrides:
  void OnServiceUpdateEvent() override;

  void OnJavascriptAllowed() override;
  void OnJavascriptDisallowed() override;

  void GetRegionalLists(const base::Value::List& args);
  void EnableFilterList(const base::Value::List& args);
  void GetListSubscriptions(const base::Value::List& args);
  void GetCustomFilters(const base::Value::List& args);
  void AddSubscription(const base::Value::List& args);
  void SetSubscriptionEnabled(const base::Value::List& args);
  void UpdateSubscription(const base::Value::List& args);
  void DeleteSubscription(const base::Value::List& args);
  void ViewSubscriptionSource(const base::Value::List& args);
  void UpdateCustomFilters(const base::Value::List& args);

  void RefreshSubscriptionsList();

  base::Value::List GetSubscriptions();

  raw_ptr<Profile> profile_ = nullptr;

  base::ScopedObservation<AdBlockSubscriptionServiceManager,
                          AdBlockSubscriptionServiceManagerObserver>
      service_observer_{this};
};

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_ADBLOCK_HANDLER_H_
