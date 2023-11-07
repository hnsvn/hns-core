/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_adblock_ui.h"

#include <memory>
#include <utility>

#include "base/scoped_observation.h"
#include "base/values.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/ui/webui/hns_webui_source.h"
#include "hns/components/hns_adblock/resources/grit/hns_adblock_generated_map.h"
#include "hns/components/hns_shields/browser/ad_block_custom_filters_provider.h"
#include "hns/components/hns_shields/browser/ad_block_regional_service_manager.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/hns_shields/browser/ad_block_service_helper.h"
#include "hns/components/hns_shields/browser/ad_block_subscription_service_manager.h"
#include "hns/components/hns_shields/browser/ad_block_subscription_service_manager_observer.h"
#include "hns/components/constants/webui_url_constants.h"
#include "build/build_config.h"
#include "components/grit/hns_components_resources.h"
#include "content/public/browser/web_ui_message_handler.h"

#if BUILDFLAG(IS_ANDROID)
#include "chrome/browser/android/tab_web_contents_delegate_android.h"
#include "content/public/browser/web_contents.h"
#else
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_navigator_params.h"
#include "chrome/browser/ui/singleton_tabs.h"
#endif

namespace {

class AdblockDOMHandler
    : public content::WebUIMessageHandler,
      public hns_shields::AdBlockSubscriptionServiceManagerObserver {
 public:
  AdblockDOMHandler();
  AdblockDOMHandler(const AdblockDOMHandler&) = delete;
  AdblockDOMHandler& operator=(const AdblockDOMHandler&) = delete;
  ~AdblockDOMHandler() override;

  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

  // hns_shields::AdblockSubscriptionServiceManagerObserver overrides:
  void OnServiceUpdateEvent() override;

  void OnJavascriptAllowed() override;
  void OnJavascriptDisallowed() override;

 private:
  void HandleEnableFilterList(const base::Value::List& args);
  void HandleGetCustomFilters(const base::Value::List& args);
  void HandleGetRegionalLists(const base::Value::List& args);
  void HandleGetListSubscriptions(const base::Value::List& args);
  void HandleUpdateCustomFilters(const base::Value::List& args);
  void HandleSubmitNewSubscription(const base::Value::List& args);
  void HandleSetSubscriptionEnabled(const base::Value::List& args);
  void HandleDeleteSubscription(const base::Value::List& args);
  void HandleRefreshSubscription(const base::Value::List& args);
  void HandleViewSubscriptionSource(const base::Value::List& args);

  void RefreshSubscriptionsList();

  base::ScopedObservation<
      hns_shields::AdBlockSubscriptionServiceManager,
      hns_shields::AdBlockSubscriptionServiceManagerObserver>
      service_observer_{this};
};

AdblockDOMHandler::AdblockDOMHandler() = default;

AdblockDOMHandler::~AdblockDOMHandler() = default;

void AdblockDOMHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "hns_adblock.enableFilterList",
      base::BindRepeating(&AdblockDOMHandler::HandleEnableFilterList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.getCustomFilters",
      base::BindRepeating(&AdblockDOMHandler::HandleGetCustomFilters,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.getRegionalLists",
      base::BindRepeating(&AdblockDOMHandler::HandleGetRegionalLists,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.getListSubscriptions",
      base::BindRepeating(&AdblockDOMHandler::HandleGetListSubscriptions,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.updateCustomFilters",
      base::BindRepeating(&AdblockDOMHandler::HandleUpdateCustomFilters,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.submitNewSubscription",
      base::BindRepeating(&AdblockDOMHandler::HandleSubmitNewSubscription,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.setSubscriptionEnabled",
      base::BindRepeating(&AdblockDOMHandler::HandleSetSubscriptionEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.deleteSubscription",
      base::BindRepeating(&AdblockDOMHandler::HandleDeleteSubscription,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.refreshSubscription",
      base::BindRepeating(&AdblockDOMHandler::HandleRefreshSubscription,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "hns_adblock.viewSubscriptionSource",
      base::BindRepeating(&AdblockDOMHandler::HandleViewSubscriptionSource,
                          base::Unretained(this)));
}

void AdblockDOMHandler::OnJavascriptAllowed() {
  service_observer_.Observe(g_hns_browser_process->ad_block_service()
                                ->subscription_service_manager());
}

void AdblockDOMHandler::OnJavascriptDisallowed() {
  service_observer_.Reset();
}

void AdblockDOMHandler::OnServiceUpdateEvent() {
  if (!IsJavascriptAllowed()) {
    return;
  }
  RefreshSubscriptionsList();
}

void AdblockDOMHandler::HandleEnableFilterList(const base::Value::List& args) {
  DCHECK_EQ(args.size(), 2U);
  if (!args[0].is_string() || !args[1].is_bool())
    return;

  std::string uuid = args[0].GetString();
  bool enabled = args[1].GetBool();
  g_hns_browser_process->ad_block_service()
      ->regional_service_manager()
      ->EnableFilterList(uuid, enabled);
}

void AdblockDOMHandler::HandleGetCustomFilters(const base::Value::List& args) {
  DCHECK_EQ(args.size(), 0U);
  AllowJavascript();
  const std::string custom_filters = g_hns_browser_process->ad_block_service()
                                         ->custom_filters_provider()
                                         ->GetCustomFilters();
  CallJavascriptFunction("hns_adblock.onGetCustomFilters",
                         base::Value(custom_filters));
}

void AdblockDOMHandler::HandleGetRegionalLists(const base::Value::List& args) {
  DCHECK_EQ(args.size(), 0U);
  AllowJavascript();
  auto regional_lists = g_hns_browser_process->ad_block_service()
                            ->regional_service_manager()
                            ->GetRegionalLists();
  CallJavascriptFunction("hns_adblock.onGetRegionalLists", regional_lists);
}

void AdblockDOMHandler::HandleGetListSubscriptions(
    const base::Value::List& args) {
  DCHECK_EQ(args.size(), 0U);
  AllowJavascript();
  RefreshSubscriptionsList();
}

void AdblockDOMHandler::HandleUpdateCustomFilters(
    const base::Value::List& args) {
  DCHECK_EQ(args.size(), 1U);
  if (!args[0].is_string())
    return;

  std::string custom_filters = args[0].GetString();
  g_hns_browser_process->ad_block_service()
      ->custom_filters_provider()
      ->UpdateCustomFilters(custom_filters);
}

void AdblockDOMHandler::HandleSubmitNewSubscription(
    const base::Value::List& args) {
  DCHECK_EQ(args.size(), 1U);
  AllowJavascript();
  if (!args[0].is_string())
    return;

  std::string subscription_url_string = args[0].GetString();
  const GURL subscription_url = GURL(subscription_url_string);
  if (!subscription_url.is_valid()) {
    return;
  }

  g_hns_browser_process->ad_block_service()
      ->subscription_service_manager()
      ->CreateSubscription(subscription_url);
  RefreshSubscriptionsList();
}

void AdblockDOMHandler::HandleSetSubscriptionEnabled(
    const base::Value::List& args) {
  DCHECK_EQ(args.size(), 2U);
  AllowJavascript();
  if (!args[0].is_string() || !args[1].is_bool())
    return;

  std::string subscription_url_string = args[0].GetString();
  bool enabled = args[1].GetBool();
  const GURL subscription_url = GURL(subscription_url_string);
  if (!subscription_url.is_valid()) {
    return;
  }
  g_hns_browser_process->ad_block_service()
      ->subscription_service_manager()
      ->EnableSubscription(subscription_url, enabled);
  RefreshSubscriptionsList();
}

void AdblockDOMHandler::HandleDeleteSubscription(
    const base::Value::List& args) {
  DCHECK_EQ(args.size(), 1U);
  AllowJavascript();
  if (!args[0].is_string())
    return;

  std::string subscription_url_string = args[0].GetString();
  const GURL subscription_url = GURL(subscription_url_string);
  if (!subscription_url.is_valid()) {
    return;
  }
  g_hns_browser_process->ad_block_service()
      ->subscription_service_manager()
      ->DeleteSubscription(subscription_url);
  RefreshSubscriptionsList();
}

void AdblockDOMHandler::HandleRefreshSubscription(
    const base::Value::List& args) {
  DCHECK_EQ(args.size(), 1U);
  // This handler does not call Javascript directly, but refreshing the
  // subscription will trigger the observer later, which will require it.
  AllowJavascript();
  if (!args[0].is_string())
    return;

  std::string subscription_url_string = args[0].GetString();
  const GURL subscription_url = GURL(subscription_url_string);
  if (!subscription_url.is_valid()) {
    return;
  }
  g_hns_browser_process->ad_block_service()
      ->subscription_service_manager()
      ->RefreshSubscription(subscription_url, true);
}

void AdblockDOMHandler::HandleViewSubscriptionSource(
    const base::Value::List& args) {
  DCHECK_EQ(args.size(), 1U);
  if (!args[0].is_string())
    return;

  std::string subscription_url_string = args[0].GetString();
  const GURL subscription_url = GURL(subscription_url_string);
  if (!subscription_url.is_valid()) {
    return;
  }

  const GURL file_url = g_hns_browser_process->ad_block_service()
                            ->subscription_service_manager()
                            ->GetListTextFileUrl(subscription_url);

#if BUILDFLAG(IS_ANDROID)
  web_ui()->GetWebContents()->GetDelegate()->OpenURLFromTab(
      web_ui()->GetWebContents(),
      content::OpenURLParams(file_url, content::Referrer(),
                             WindowOpenDisposition::NEW_FOREGROUND_TAB,
                             ui::PAGE_TRANSITION_AUTO_TOPLEVEL, false));
#else
  auto* browser =
      chrome::FindBrowserWithWebContents(web_ui()->GetWebContents());
  ShowSingletonTabOverwritingNTP(browser, file_url);
#endif
}

// Convenience method to push updated subscription information to the UI.
void AdblockDOMHandler::RefreshSubscriptionsList() {
  DCHECK(IsJavascriptAllowed());
  auto list_subscriptions = g_hns_browser_process->ad_block_service()
                                ->subscription_service_manager()
                                ->GetSubscriptions();
  base::Value::List list_value;
  for (const auto& subscription : list_subscriptions) {
    base::Value::Dict dict;
    dict.Set("subscription_url", subscription.subscription_url.spec());
    dict.Set("enabled", subscription.enabled);
    dict.Set("last_update_attempt",
             subscription.last_update_attempt.ToJsTime());
    dict.Set("last_successful_update_attempt",
             subscription.last_successful_update_attempt.ToJsTime());
    if (subscription.homepage) {
      dict.Set("homepage", *subscription.homepage);
    }
    if (subscription.title) {
      dict.Set("title", *subscription.title);
    }
    list_value.Append(std::move(dict));
  }
  CallJavascriptFunction("hns_adblock.onGetListSubscriptions", list_value);
}

}  // namespace

HnsAdblockUI::HnsAdblockUI(content::WebUI* web_ui, const std::string& name)
    : WebUIController(web_ui) {
  CreateAndAddWebUIDataSource(web_ui, name, kHnsAdblockGenerated,
                              kHnsAdblockGeneratedSize,
                              IDR_HNS_ADBLOCK_HTML);
  web_ui->AddMessageHandler(std::make_unique<AdblockDOMHandler>());
}

HnsAdblockUI::~HnsAdblockUI() = default;
