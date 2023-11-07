/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_search_engines_handler.h"

#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "hns/browser/search_engines/search_engine_provider_util.h"
#include "chrome/browser/ui/search_engines/template_url_table_model.h"
#include "components/search_engines/template_url.h"

namespace settings {

namespace {
constexpr char kHnsSearchForTorKeyword[] =
    ":search.hns4u7jddbv7cyviptqjc7jusxh72uik7zt6adtckl5f4nwy2v72qd.onion";
}  // namespace

HnsSearchEnginesHandler::HnsSearchEnginesHandler(Profile* profile)
    : SearchEnginesHandler(profile) {}

HnsSearchEnginesHandler::~HnsSearchEnginesHandler() = default;

void HnsSearchEnginesHandler::RegisterMessages() {
  SearchEnginesHandler::RegisterMessages();

  web_ui()->RegisterMessageCallback(
      "getPrivateSearchEnginesList",
      base::BindRepeating(
          &HnsSearchEnginesHandler::HandleGetPrivateSearchEnginesList,
          base::Unretained(this)));
}

void HnsSearchEnginesHandler::OnModelChanged() {
  SearchEnginesHandler::OnModelChanged();

  hns::UpdateDefaultPrivateSearchProviderData(profile_);

  // Sync normal profile's search provider list with private profile
  // for using same list on both.
  FireWebUIListener("private-search-engines-changed",
                    GetPrivateSearchEnginesList());
}

void HnsSearchEnginesHandler::HandleGetPrivateSearchEnginesList(
    const base::Value::List& args) {
  CHECK_EQ(1U, args.size());
  AllowJavascript();
  ResolveJavascriptCallback(args[0], GetPrivateSearchEnginesList());
}

base::Value::List HnsSearchEnginesHandler::GetPrivateSearchEnginesList() {
  // Construct list with normal profile's default list.
  // Normal and Private profile use same default list.
  int last_default_engine_index =
      list_controller_.table_model()->last_search_engine_index();
  base::Value::List defaults;

  for (int i = 0; i < last_default_engine_index; ++i) {
    const TemplateURL* template_url = list_controller_.GetTemplateURL(i);
    // Don't show two hns search entries from settings to prevent confusion.
    // Hide hns search for tor entry from settings UI. User doesn't need to
    // select hns search tor entry for private profile.
    if (base::UTF16ToUTF8(template_url->keyword()) == kHnsSearchForTorKeyword)
      continue;

    base::Value::Dict dict;
    dict.Set("value", template_url->sync_guid());
    dict.Set("name", template_url->short_name());
    defaults.Append(std::move(dict));
  }

  return defaults;
}

base::Value::Dict HnsSearchEnginesHandler::GetSearchEnginesList() {
  auto search_engines_info = SearchEnginesHandler::GetSearchEnginesList();
  // Don't show two hns search entries from settings to prevent confusion.
  // Hide hns search for tor entry from settings UI. User doesn't need to
  // select hns search tor entry for normal profile.
  auto* defaults = search_engines_info.FindList("defaults");
  DCHECK(defaults);
  defaults->EraseIf([](const auto& val) {
    const auto& dict = val.GetDict();
    const std::string* keyword = dict.FindString("keyword");
    DCHECK(keyword);
    return *keyword == kHnsSearchForTorKeyword;
  });
  return search_engines_info;
}

}  // namespace settings
