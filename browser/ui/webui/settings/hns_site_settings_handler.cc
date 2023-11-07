/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_site_settings_handler.h"

#include <string>

#include "base/values.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "chrome/browser/ui/webui/settings/site_settings_helper.h"
#include "components/grit/hns_components_strings.h"
#include "ui/base/l10n/l10n_util.h"

namespace settings {

namespace {
constexpr char kIsValidKey[] = "isValid";
constexpr char kReasonKey[] = "reason";
}  // namespace

HnsSiteSettingsHandler::HnsSiteSettingsHandler(Profile* profile)
    : SiteSettingsHandler(profile) {}

HnsSiteSettingsHandler::~HnsSiteSettingsHandler() = default;

void HnsSiteSettingsHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "isPatternValidForType",
      base::BindRepeating(
          &HnsSiteSettingsHandler::HandleIsPatternValidForType,
          base::Unretained(this)));

  SiteSettingsHandler::RegisterMessages();
}

bool HnsSiteSettingsHandler::IsPatternValidForHnsContentType(
    ContentSettingsType content_type,
    const std::string& pattern_string) {
  if (content_type != ContentSettingsType::HNS_SHIELDS)
    return true;
  return (pattern_string.find('*') == std::string::npos);
}

void HnsSiteSettingsHandler::HandleIsPatternValidForType(
    const base::Value::List& args) {
  CHECK_EQ(3U, args.size());
  const base::Value& callback_id = args[0];
  const std::string& pattern_string = args[1].GetString();
  const std::string& type = args[2].GetString();

  ContentSettingsType content_type =
      site_settings::ContentSettingsTypeFromGroupName(type);

  if (!IsPatternValidForHnsContentType(content_type, pattern_string)) {
    base::Value::Dict return_value;
    return_value.Set(kIsValidKey, base::Value(false));
    return_value.Set(kReasonKey, base::Value(l10n_util::GetStringUTF8(
                                     IDS_HNS_SHIELDS_NOT_VALID_ADDRESS)));
    AllowJavascript();
    ResolveJavascriptCallback(callback_id, return_value);
    return;
  }

  SiteSettingsHandler::HandleIsPatternValidForType(args);
}

}  // namespace settings
