/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_settings_leo_assistant_handler.h"

#include <vector>

#include "base/containers/contains.h"
#include "hns/browser/ui/sidebar/sidebar_service_factory.h"
#include "hns/components/ai_chat/common/pref_names.h"
#include "hns/components/sidebar/sidebar_item.h"
#include "hns/components/sidebar/sidebar_service.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"

namespace {

const std::vector<sidebar::SidebarItem>::const_iterator FindAiChatSidebarItem(
    const std::vector<sidebar::SidebarItem>& items) {
  return base::ranges::find_if(items, [](const auto& item) {
    return item.built_in_item_type ==
           sidebar::SidebarItem::BuiltInItemType::kChatUI;
  });
}

bool ShowLeoAssistantIconVisibleIfNot(
    sidebar::SidebarService* sidebar_service) {
  const auto hidden_items = sidebar_service->GetHiddenDefaultSidebarItems();
  const auto item_hidden_iter = FindAiChatSidebarItem(hidden_items);

  if (item_hidden_iter != hidden_items.end()) {
    sidebar_service->AddItem(*item_hidden_iter);
    return true;
  }

  return false;
}

bool HideLeoAssistantIconIfNot(sidebar::SidebarService* sidebar_service) {
  const auto visible_items = sidebar_service->items();
  const auto item_visible_iter = FindAiChatSidebarItem(visible_items);

  if (item_visible_iter != visible_items.end()) {
    sidebar_service->RemoveItemAt(item_visible_iter - visible_items.begin());
    return true;
  }

  return false;
}

}  // namespace

namespace settings {

HnsLeoAssistantHandler::HnsLeoAssistantHandler() = default;
HnsLeoAssistantHandler::~HnsLeoAssistantHandler() = default;

void HnsLeoAssistantHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());

  web_ui()->RegisterMessageCallback(
      "toggleLeoIcon",
      base::BindRepeating(&HnsLeoAssistantHandler::HandleToggleLeoIcon,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getLeoIconVisibility",
      base::BindRepeating(&HnsLeoAssistantHandler::HandleGetLeoIconVisibility,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "resetLeoData",
      base::BindRepeating(&HnsLeoAssistantHandler::HandleResetLeoData,
                          base::Unretained(this)));
}

void HnsLeoAssistantHandler::OnJavascriptAllowed() {
  sidebar_service_observer_.Reset();
  sidebar_service_observer_.Observe(
      sidebar::SidebarServiceFactory::GetForProfile(profile_));
}

void HnsLeoAssistantHandler::OnJavascriptDisallowed() {
  sidebar_service_observer_.Reset();
}

void HnsLeoAssistantHandler::OnItemAdded(const sidebar::SidebarItem& item,
                                           size_t index) {
  if (item.built_in_item_type ==
      sidebar::SidebarItem::BuiltInItemType::kChatUI) {
    NotifyChatUiChanged(true);
  }
}

void HnsLeoAssistantHandler::OnItemRemoved(const sidebar::SidebarItem& item,
                                             size_t index) {
  if (item.built_in_item_type ==
      sidebar::SidebarItem::BuiltInItemType::kChatUI) {
    NotifyChatUiChanged(false);
  }
}

void HnsLeoAssistantHandler::NotifyChatUiChanged(const bool& is_leo_visible) {
  if (!IsJavascriptAllowed()) {
    return;
  }
  FireWebUIListener("settings-hns-leo-assistant-changed", is_leo_visible);
}

void HnsLeoAssistantHandler::HandleToggleLeoIcon(
    const base::Value::List& args) {
  auto* service = sidebar::SidebarServiceFactory::GetForProfile(profile_);

  AllowJavascript();
  if (!ShowLeoAssistantIconVisibleIfNot(service)) {
    HideLeoAssistantIconIfNot(service);
  }
}

void HnsLeoAssistantHandler::HandleGetLeoIconVisibility(
    const base::Value::List& args) {
  auto* service = sidebar::SidebarServiceFactory::GetForProfile(profile_);
  const auto hidden_items = service->GetHiddenDefaultSidebarItems();
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0], !base::Contains(hidden_items,
                               sidebar::SidebarItem::BuiltInItemType::kChatUI,
                               &sidebar::SidebarItem::built_in_item_type));
}

void HnsLeoAssistantHandler::HandleResetLeoData(
    const base::Value::List& args) {
  auto* service = sidebar::SidebarServiceFactory::GetForProfile(profile_);

  ShowLeoAssistantIconVisibleIfNot(service);
  profile_->GetPrefs()->SetBoolean(ai_chat::prefs::kHnsChatHasSeenDisclaimer,
                                   false);
  profile_->GetPrefs()->SetBoolean(
      ai_chat::prefs::kHnsChatAutoGenerateQuestions, false);

  AllowJavascript();
}

}  // namespace settings
