/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_adblock_handler.h"

#include <memory>
#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/json/values_util.h"
#include "base/values.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/components/hns_adblock/resources/grit/hns_adblock_generated_map.h"
#include "hns/components/hns_shields/browser/ad_block_custom_filters_provider.h"
#include "hns/components/hns_shields/browser/ad_block_regional_service_manager.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_navigator_params.h"
#include "chrome/browser/ui/singleton_tabs.h"
#include "content/public/browser/web_ui.h"
#include "ui/base/l10n/time_format.h"

HnsAdBlockHandler::HnsAdBlockHandler() = default;

HnsAdBlockHandler::~HnsAdBlockHandler() = default;

void HnsAdBlockHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  web_ui()->RegisterMessageCallback(
      "hns_adblock.getRegionalLists",
      base::BindRepeating(&HnsAdBlockHandler::GetRegionalLists,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.enableFilterList",
      base::BindRepeating(&HnsAdBlockHandler::EnableFilterList,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.getListSubscriptions",
      base::BindRepeating(&HnsAdBlockHandler::GetListSubscriptions,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.getCustomFilters",
      base::BindRepeating(&HnsAdBlockHandler::GetCustomFilters,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.addSubscription",
      base::BindRepeating(&HnsAdBlockHandler::AddSubscription,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.setSubscriptionEnabled",
      base::BindRepeating(&HnsAdBlockHandler::SetSubscriptionEnabled,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.updateSubscription",
      base::BindRepeating(&HnsAdBlockHandler::UpdateSubscription,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.deleteSubscription",
      base::BindRepeating(&HnsAdBlockHandler::DeleteSubscription,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.viewSubscription",
      base::BindRepeating(&HnsAdBlockHandler::ViewSubscriptionSource,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "hns_adblock.updateCustomFilters",
      base::BindRepeating(&HnsAdBlockHandler::UpdateCustomFilters,
                          base::Unretained(this)));
}

void HnsAdBlockHandler::OnJavascriptAllowed() {
  service_observer_.Observe(g_hns_browser_process->ad_block_service()
                                ->subscription_service_manager());
}

void HnsAdBlockHandler::OnJavascriptDisallowed() {
  service_observer_.Reset();
}

void HnsAdBlockHandler::OnServiceUpdateEvent() {
  if (!IsJavascriptAllowed()) {
    return;
  }

  RefreshSubscriptionsList();
}

void HnsAdBlockHandler::GetRegionalLists(const base::Value::List& args) {
  AllowJavascript();
  auto regional_lists = g_hns_browser_process->ad_block_service()
                            ->regional_service_manager()
                            ->GetRegionalLists();

  ResolveJavascriptCallback(args[0], regional_lists);
}

void HnsAdBlockHandler::EnableFilterList(const base::Value::List& args) {
  DCHECK_EQ(args.size(), 2U);

  if (!args[0].is_string() || !args[1].is_bool())
    return;

  std::string uuid = args[0].GetString();
  bool enabled = args[1].GetBool();

  g_hns_browser_process->ad_block_service()
      ->regional_service_manager()
      ->EnableFilterList(uuid, enabled);
}

void HnsAdBlockHandler::GetListSubscriptions(const base::Value::List& args) {
  AllowJavascript();
  ResolveJavascriptCallback(args[0], GetSubscriptions());
}

void HnsAdBlockHandler::GetCustomFilters(const base::Value::List& args) {
  AllowJavascript();
  const std::string custom_filters = g_hns_browser_process->ad_block_service()
                                         ->custom_filters_provider()
                                         ->GetCustomFilters();

  ResolveJavascriptCallback(args[0], base::Value(custom_filters));
}

void HnsAdBlockHandler::AddSubscription(const base::Value::List& args) {
  DCHECK_EQ(args.size(), 1U);
  AllowJavascript();
  if (!args[0].is_string())
    return;

  std::string subscription_url_string = args[0].GetString();
  const GURL subscription_url = GURL(subscription_url_string);

  if (!subscription_url.is_valid())
    return;

  g_hns_browser_process->ad_block_service()
      ->subscription_service_manager()
      ->CreateSubscription(subscription_url);

  RefreshSubscriptionsList();
}

void HnsAdBlockHandler::SetSubscriptionEnabled(
    const base::Value::List& args) {
  DCHECK_EQ(args.size(), 2U);
  AllowJavascript();
  if (!args[0].is_string() || !args[1].is_bool())
    return;

  std::string subscription_url_string = args[0].GetString();
  bool enabled = args[1].GetBool();
  const GURL subscription_url = GURL(subscription_url_string);
  if (!subscription_url.is_valid())
    return;
  g_hns_browser_process->ad_block_service()
      ->subscription_service_manager()
      ->EnableSubscription(subscription_url, enabled);

  RefreshSubscriptionsList();
}

void HnsAdBlockHandler::UpdateSubscription(const base::Value::List& args) {
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
      ->RefreshSubscription(subscription_url, true);
}

void HnsAdBlockHandler::DeleteSubscription(const base::Value::List& args) {
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

void HnsAdBlockHandler::ViewSubscriptionSource(
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

  auto* browser =
      chrome::FindBrowserWithWebContents(web_ui()->GetWebContents());
  ShowSingletonTabOverwritingNTP(browser, file_url);
}

void HnsAdBlockHandler::UpdateCustomFilters(const base::Value::List& args) {
  if (!args[0].is_string())
    return;

  std::string custom_filters = args[0].GetString();
  g_hns_browser_process->ad_block_service()
      ->custom_filters_provider()
      ->UpdateCustomFilters(custom_filters);
}

void HnsAdBlockHandler::RefreshSubscriptionsList() {
  FireWebUIListener("hns_adblock.onGetListSubscriptions", GetSubscriptions());
}

base::Value::List HnsAdBlockHandler::GetSubscriptions() {
  auto list_subscriptions = g_hns_browser_process->ad_block_service()
                                ->subscription_service_manager()
                                ->GetSubscriptions();

  base::Value::List list_value;
  base::Time now = base::Time::Now();

  for (const auto& subscription : list_subscriptions) {
    base::Value::Dict dict;

    base::TimeDelta relative_time_delta =
        now - subscription.last_successful_update_attempt;

    auto time_str = ui::TimeFormat::Simple(
        ui::TimeFormat::Format::FORMAT_ELAPSED,
        ui::TimeFormat::Length::LENGTH_LONG, relative_time_delta);

    dict.Set("subscription_url", subscription.subscription_url.spec());
    dict.Set("enabled", subscription.enabled);
    dict.Set("last_update_attempt",
             subscription.last_update_attempt.ToJsTime());
    dict.Set("last_successful_update_attempt",
             subscription.last_successful_update_attempt.ToJsTime());
    dict.Set("last_updated_pretty_text", time_str);
    if (subscription.homepage) {
      dict.Set("homepage", *subscription.homepage);
    }
    if (subscription.title && !subscription.title->empty()) {
      dict.Set("title", *subscription.title);
    } else {
      dict.Set("title", subscription.subscription_url.spec());
    }

    list_value.Append(std::move(dict));
  }

  return list_value;
}
