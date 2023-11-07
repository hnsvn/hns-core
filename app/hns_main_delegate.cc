/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/app/hns_main_delegate.h"

#include <memory>
#include <string>
#include <unordered_set>

#include "base/base_switches.h"
#include "base/lazy_instance.h"
#include "base/path_service.h"
#include "base/time/time.h"
#include "hns/app/hns_command_line_helper.h"
#include "hns/browser/hns_content_browser_client.h"
#include "hns/common/resource_bundle_helper.h"
#include "hns/components/hns_component_updater/browser/features.h"
#include "hns/components/hns_component_updater/browser/switches.h"
#include "hns/components/constants/hns_switches.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/update_client/buildflags.h"
#include "hns/renderer/hns_content_renderer_client.h"
#include "hns/utility/hns_content_utility_client.h"
#include "build/build_config.h"
#include "chrome/common/chrome_features.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/common/chrome_switches.h"
#include "components/component_updater/component_updater_switches.h"
#include "components/embedder_support/switches.h"
#include "components/sync/base/command_line_switches.h"
#include "components/variations/variations_switches.h"
#include "content/public/common/content_switches.h"
#include "google_apis/gaia/gaia_switches.h"

namespace {

const char kHnsOriginTrialsPublicKey[] =
    "bYUKPJoPnCxeNvu72j4EmPuK7tr1PAC7SHh8ld9Mw3E=,"
    "fMS4mpO6buLQ/QMd+zJmxzty/VQ6B1EUZqoCU04zoRU=";

// staging "https://sync-v2.hnssoftware.com/v2" can be overriden by
// syncer::kSyncServiceURL manually
const char kHnsSyncServiceStagingURL[] =
    "https://sync-v2.hnssoftware.com/v2";

const char kDummyUrl[] = "https://no-thanks.invalid";

std::string GetUpdateURLHost() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(hns_component_updater::kUseGoUpdateDev) &&
      !base::FeatureList::IsEnabled(
          hns_component_updater::kUseDevUpdaterUrl)) {
    return BUILDFLAG(UPDATER_PROD_ENDPOINT);
  }
  return BUILDFLAG(UPDATER_DEV_ENDPOINT);
}

}  // namespace

#if !defined(CHROME_MULTIPLE_DLL_BROWSER)
base::LazyInstance<HnsContentRendererClient>::DestructorAtExit
    g_hns_content_renderer_client = LAZY_INSTANCE_INITIALIZER;
base::LazyInstance<HnsContentUtilityClient>::DestructorAtExit
    g_hns_content_utility_client = LAZY_INSTANCE_INITIALIZER;
#endif
#if !defined(CHROME_MULTIPLE_DLL_CHILD)
base::LazyInstance<HnsContentBrowserClient>::DestructorAtExit
    g_hns_content_browser_client = LAZY_INSTANCE_INITIALIZER;
#endif

HnsMainDelegate::HnsMainDelegate() : ChromeMainDelegate() {}

HnsMainDelegate::HnsMainDelegate(base::TimeTicks exe_entry_point_ticks)
    : ChromeMainDelegate(exe_entry_point_ticks) {}

HnsMainDelegate::~HnsMainDelegate() {}

content::ContentBrowserClient* HnsMainDelegate::CreateContentBrowserClient() {
#if defined(CHROME_MULTIPLE_DLL_CHILD)
  return NULL;
#else
  if (chrome_content_browser_client_ == nullptr) {
    chrome_content_browser_client_ =
        std::make_unique<HnsContentBrowserClient>();
  }
  return chrome_content_browser_client_.get();
#endif
}

content::ContentRendererClient*
HnsMainDelegate::CreateContentRendererClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return NULL;
#else
  return g_hns_content_renderer_client.Pointer();
#endif
}

content::ContentUtilityClient* HnsMainDelegate::CreateContentUtilityClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return NULL;
#else
  return g_hns_content_utility_client.Pointer();
#endif
}

void HnsMainDelegate::PreSandboxStartup() {
  ChromeMainDelegate::PreSandboxStartup();
#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_MAC)
  // Setup NativeMessagingHosts to point to the default Chrome locations
  // because that's where native apps will create them
  base::FilePath chrome_user_data_dir;
  base::FilePath native_messaging_dir;
#if BUILDFLAG(IS_MAC)
  base::PathService::Get(base::DIR_APP_DATA, &chrome_user_data_dir);
  chrome_user_data_dir = chrome_user_data_dir.Append("Google/Chrome");
  native_messaging_dir = base::FilePath(
      FILE_PATH_LITERAL("/Library/Google/Chrome/NativeMessagingHosts"));
#else
  chrome::GetDefaultUserDataDirectory(&chrome_user_data_dir);
  native_messaging_dir = base::FilePath(
      FILE_PATH_LITERAL("/etc/opt/chrome/native-messaging-hosts"));
#endif  // BUILDFLAG(IS_MAC)
  base::PathService::OverrideAndCreateIfNeeded(
      chrome::DIR_USER_NATIVE_MESSAGING,
      chrome_user_data_dir.Append(FILE_PATH_LITERAL("NativeMessagingHosts")),
      false, true);
  base::PathService::OverrideAndCreateIfNeeded(
      chrome::DIR_NATIVE_MESSAGING, native_messaging_dir, false, true);
#endif  // BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_MAC)

#if BUILDFLAG(IS_POSIX) && !BUILDFLAG(IS_MAC)
  base::PathService::OverrideAndCreateIfNeeded(
      chrome::DIR_POLICY_FILES,
      base::FilePath(FILE_PATH_LITERAL("/etc/hns/policies")), true, false);
#endif

  if (hns::SubprocessNeedsResourceBundle()) {
    hns::InitializeResourceBundle();
  }
}

absl::optional<int> HnsMainDelegate::PostEarlyInitialization(
    ChromeMainDelegate::InvokedIn invoked_in) {
  auto result = ChromeMainDelegate::PostEarlyInitialization(invoked_in);
  if (result.has_value()) {
    // An exit code is set. Stop initialization.
    return result;
  }

  HnsCommandLineHelper command_line(base::CommandLine::ForCurrentProcess());
  std::string update_url = GetUpdateURLHost();
  if (!update_url.empty()) {
    auto* cmd = base::CommandLine::ForCurrentProcess();
    std::string current_value;
    if (cmd->HasSwitch(switches::kComponentUpdater)) {
      current_value = cmd->GetSwitchValueASCII(switches::kComponentUpdater);
      cmd->RemoveSwitch(switches::kComponentUpdater);
    }
    if (!current_value.empty()) {
      current_value += ',';
    }

    command_line.AppendSwitchASCII(
        switches::kComponentUpdater,
        (current_value + "url-source=" + update_url).c_str());
  }
  return result;
}
