/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_privacy_handler.h"

#include "base/functional/bind.h"
#include "base/values.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/de_amp/common/features.h"
#include "hns/components/debounce/common/features.h"
#include "hns/components/google_sign_in_permission/google_sign_in_permission_util.h"
#include "hns/components/p3a/pref_names.h"
#include "hns/components/request_otr/common/buildflags/buildflags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "third_party/blink/public/common/peerconnection/webrtc_ip_handling_policy.h"

#if BUILDFLAG(ENABLE_REQUEST_OTR)
#include "hns/components/request_otr/common/features.h"
#endif

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "hns/browser/gcm_driver/hns_gcm_channel_status.h"
#endif

HnsPrivacyHandler::HnsPrivacyHandler() {
  local_state_change_registrar_.Init(g_browser_process->local_state());
  local_state_change_registrar_.Add(
      kStatsReportingEnabled,
      base::BindRepeating(&HnsPrivacyHandler::OnStatsUsagePingEnabledChanged,
                          base::Unretained(this)));
  local_state_change_registrar_.Add(
      p3a::kP3AEnabled,
      base::BindRepeating(&HnsPrivacyHandler::OnP3AEnabledChanged,
                          base::Unretained(this)));
}

HnsPrivacyHandler::~HnsPrivacyHandler() {
  local_state_change_registrar_.RemoveAll();
}

void HnsPrivacyHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());

  web_ui()->RegisterMessageCallback(
      "setP3AEnabled", base::BindRepeating(&HnsPrivacyHandler::SetP3AEnabled,
                                           base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getP3AEnabled", base::BindRepeating(&HnsPrivacyHandler::GetP3AEnabled,
                                           base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setStatsUsagePingEnabled",
      base::BindRepeating(&HnsPrivacyHandler::SetStatsUsagePingEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getStatsUsagePingEnabled",
      base::BindRepeating(&HnsPrivacyHandler::GetStatsUsagePingEnabled,
                          base::Unretained(this)));
}

// static
void HnsPrivacyHandler::AddLoadTimeData(content::WebUIDataSource* data_source,
                                          Profile* profile) {
#if BUILDFLAG(USE_GCM_FROM_PLATFORM)
  data_source->AddBoolean("pushMessagingEnabledAtStartup", true);
#else
  gcm::HnsGCMChannelStatus* gcm_channel_status =
      gcm::HnsGCMChannelStatus::GetForProfile(profile);

  DCHECK(gcm_channel_status);
  data_source->AddBoolean("pushMessagingEnabledAtStartup",
                          gcm_channel_status->IsGCMEnabled());
#endif
  data_source->AddBoolean(
      "isDeAmpFeatureEnabled",
      base::FeatureList::IsEnabled(de_amp::features::kHnsDeAMP));
  data_source->AddBoolean(
      "isDebounceFeatureEnabled",
      base::FeatureList::IsEnabled(debounce::features::kHnsDebounce));
#if BUILDFLAG(ENABLE_REQUEST_OTR)
  data_source->AddBoolean(
      "isRequestOTRFeatureEnabled",
      base::FeatureList::IsEnabled(request_otr::features::kHnsRequestOTRTab));
#endif
  data_source->AddBoolean(
      "isGoogleSignInFeatureEnabled",
      google_sign_in_permission::IsGoogleSignInFeatureEnabled());
  data_source->AddBoolean(
      "isLocalhostAccessFeatureEnabled",
      base::FeatureList::IsEnabled(
          hns_shields::features::kHnsLocalhostAccessPermission));
}

void HnsPrivacyHandler::SetLocalStateBooleanEnabled(
    const std::string& path,
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  if (!args[0].is_bool()) {
    return;
  }

  bool enabled = args[0].GetBool();
  PrefService* local_state = g_browser_process->local_state();
  local_state->SetBoolean(path, enabled);
}

void HnsPrivacyHandler::GetLocalStateBooleanEnabled(
    const std::string& path,
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);

  PrefService* local_state = g_browser_process->local_state();
  bool enabled = local_state->GetBoolean(path);

  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(enabled));
}

void HnsPrivacyHandler::SetStatsUsagePingEnabled(
    const base::Value::List& args) {
  SetLocalStateBooleanEnabled(kStatsReportingEnabled, args);
}

void HnsPrivacyHandler::GetStatsUsagePingEnabled(
    const base::Value::List& args) {
  GetLocalStateBooleanEnabled(kStatsReportingEnabled, args);
}

void HnsPrivacyHandler::OnStatsUsagePingEnabledChanged() {
  if (IsJavascriptAllowed()) {
    PrefService* local_state = g_browser_process->local_state();
    bool enabled = local_state->GetBoolean(kStatsReportingEnabled);

    FireWebUIListener("stats-usage-ping-enabled-changed", base::Value(enabled));
  }
}

void HnsPrivacyHandler::SetP3AEnabled(const base::Value::List& args) {
  SetLocalStateBooleanEnabled(p3a::kP3AEnabled, args);
}

void HnsPrivacyHandler::GetP3AEnabled(const base::Value::List& args) {
  GetLocalStateBooleanEnabled(p3a::kP3AEnabled, args);
}

void HnsPrivacyHandler::OnP3AEnabledChanged() {
  if (IsJavascriptAllowed()) {
    PrefService* local_state = g_browser_process->local_state();
    bool enabled = local_state->GetBoolean(p3a::kP3AEnabled);

    FireWebUIListener("p3a-enabled-changed", base::Value(enabled));
  }
}
