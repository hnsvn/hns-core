/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/welcome_page/hns_welcome_ui.h"

#include <algorithm>
#include <memory>
#include <string>

#include "base/feature_list.h"
#include "base/memory/raw_ptr.h"
#include "base/metrics/histogram_macros.h"
#include "base/strings/utf_string_conversions.h"
#include "base/task/single_thread_task_runner.h"
#include "hns/browser/ui/webui/hns_webui_source.h"
#include "hns/browser/ui/webui/settings/hns_import_bulk_data_handler.h"
#include "hns/browser/ui/webui/settings/hns_search_engines_handler.h"
#include "hns/common/importer/importer_constants.h"
#include "hns/components/hns_welcome/common/features.h"
#include "hns/components/hns_welcome/resources/grit/hns_welcome_generated_map.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/metrics/metrics_reporting_state.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/browser/ui/webui/settings/privacy_sandbox_handler.h"
#include "chrome/browser/ui/webui/settings/settings_default_browser_handler.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/webui_url_constants.h"
#include "chrome/grit/chromium_strings.h"
#include "components/country_codes/country_codes.h"
#include "components/grit/hns_components_resources.h"
#include "components/grit/hns_components_strings.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/gpu_data_manager.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "ui/base/l10n/l10n_util.h"

using content::WebUIMessageHandler;

namespace {

constexpr webui::LocalizedString kLocalizedStrings[] = {
    {"hnsWelcomeTitle", IDS_HNS_WELCOME_TITLE},
    {"hnsWelcomeDesc", IDS_HNS_WELCOME_DESC},
    {"hnsWelcomeImportSettingsTitle",
     IDS_HNS_WELCOME_IMPORT_SETTINGS_TITLE},
    {"hnsWelcomeImportSettingsDesc", IDS_HNS_WELCOME_IMPORT_SETTINGS_DESC},
    {"hnsWelcomeSelectProfileLabel", IDS_HNS_WELCOME_SELECT_PROFILE_LABEL},
    {"hnsWelcomeSelectProfileDesc", IDS_HNS_WELCOME_SELECT_PROFILE_DESC},
    {"hnsWelcomeImportButtonLabel", IDS_HNS_WELCOME_IMPORT_BUTTON_LABEL},
    {"hnsWelcomeImportProfilesButtonLabel",
     IDS_HNS_WELCOME_IMPORT_PROFILES_BUTTON_LABEL},
    {"hnsWelcomeSkipButtonLabel", IDS_HNS_WELCOME_SKIP_BUTTON_LABEL},
    {"hnsWelcomeBackButtonLabel", IDS_HNS_WELCOME_BACK_BUTTON_LABEL},
    {"hnsWelcomeNextButtonLabel", IDS_HNS_WELCOME_NEXT_BUTTON_LABEL},
    {"hnsWelcomeFinishButtonLabel", IDS_HNS_WELCOME_FINISH_BUTTON_LABEL},
    {"hnsWelcomeSetDefaultButtonLabel",
     IDS_HNS_WELCOME_SET_DEFAULT_BUTTON_LABEL},
    {"hnsWelcomeSelectAllButtonLabel",
     IDS_HNS_WELCOME_SELECT_ALL_BUTTON_LABEL},
    {"hnsWelcomeHelpImproveHnsTitle",
     IDS_HNS_WELCOME_HELP_IMPROVE_HNS_TITLE},
    {"hnsWelcomeSendReportsLabel", IDS_HNS_WELCOME_SEND_REPORTS_LABEL},
    {"hnsWelcomeSendInsightsLabel", IDS_HNS_WELCOME_SEND_INSIGHTS_LABEL},
    {"hnsWelcomeSetupCompleteLabel", IDS_HNS_WELCOME_SETUP_COMPLETE_LABEL},
    {"hnsWelcomeChangeSettingsNote", IDS_HNS_WELCOME_CHANGE_SETTINGS_NOTE},
    {"hnsWelcomePrivacyPolicyNote", IDS_HNS_WELCOME_PRIVACY_POLICY_NOTE},
    {"hnsWelcomeSelectThemeLabel", IDS_HNS_WELCOME_SELECT_THEME_LABEL},
    {"hnsWelcomeSelectThemeNote", IDS_HNS_WELCOME_SELECT_THEME_NOTE},
    {"hnsWelcomeSelectThemeSystemLabel",
     IDS_HNS_WELCOME_SELECT_THEME_SYSTEM_LABEL},
    {"hnsWelcomeSelectThemeLightLabel",
     IDS_HNS_WELCOME_SELECT_THEME_LIGHT_LABEL},
    {"hnsWelcomeSelectThemeDarkLabel",
     IDS_HNS_WELCOME_SELECT_THEME_DARK_LABEL}};

void OpenJapanWelcomePage(Profile* profile) {
  DCHECK(profile);
  Browser* browser = chrome::FindBrowserWithProfile(profile);
  if (browser) {
    content::OpenURLParams open_params(
        GURL("https://hns.com/ja/desktop-ntp-tutorial"), content::Referrer(),
        WindowOpenDisposition::NEW_BACKGROUND_TAB,
        ui::PAGE_TRANSITION_AUTO_TOPLEVEL, false);
    browser->OpenURL(open_params);
  }
}

void RecordP3AHistogram(int screen_number, bool finished) {
  int kCurrentScreen = 0;
  int kMaxScreens = 6;
  if (finished) {
    kCurrentScreen = kMaxScreens + 1;
  } else {
    kCurrentScreen = std::min(screen_number, kMaxScreens);
  }
  UMA_HISTOGRAM_EXACT_LINEAR("Hns.Welcome.InteractionStatus", kCurrentScreen,
                             kMaxScreens + 1);
}

// The handler for Javascript messages for the chrome://welcome page
class WelcomeDOMHandler : public WebUIMessageHandler {
 public:
  explicit WelcomeDOMHandler(Profile* profile);
  WelcomeDOMHandler(const WelcomeDOMHandler&) = delete;
  WelcomeDOMHandler& operator=(const WelcomeDOMHandler&) = delete;
  ~WelcomeDOMHandler() override;

  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

 private:
  void HandleImportNowRequested(const base::Value::List& args);
  void HandleRecordP3A(const base::Value::List& args);
  void HandleGetDefaultBrowser(const base::Value::List& args);
  void SetLocalStateBooleanEnabled(const std::string& path,
                                   const base::Value::List& args);
  void OnGetDefaultBrowser(shell_integration::DefaultWebClientState state,
                           const std::u16string& name);
  void SetP3AEnabled(const base::Value::List& args);
  void HandleOpenSettingsPage(const base::Value::List& args);
  void HandleSetMetricsReportingEnabled(const base::Value::List& args);
  Browser* GetBrowser();

  int screen_number_ = 0;
  bool finished_ = false;
  bool skipped_ = false;
  std::u16string default_browser_name_;
  raw_ptr<Profile> profile_ = nullptr;
  base::WeakPtrFactory<WelcomeDOMHandler> weak_ptr_factory_{this};
};

// Converts Chromium country ID to 2 digit country string
// For more info see src/components/country_codes/country_codes.h
std::string CountryIDToCountryString(int country_id) {
  if (country_id == country_codes::kCountryIDUnknown)
    return std::string();

  char chars[3] = {static_cast<char>(country_id >> 8),
                   static_cast<char>(country_id), 0};
  std::string country_string(chars);
  DCHECK_EQ(country_string.size(), 2U);
  return country_string;
}

}  // namespace

HnsWelcomeUI::HnsWelcomeUI(content::WebUI* web_ui, const std::string& name)
    : WebUIController(web_ui) {
  content::WebUIDataSource* source = CreateAndAddWebUIDataSource(
      web_ui, name, kHnsWelcomeGenerated, kHnsWelcomeGeneratedSize,
      IDR_HNS_WELCOME_HTML,
      /*disable_trusted_types_csp=*/true);

  // Lottie animations tick on a worker thread and requires the document CSP to
  // be set to "worker-src blob: 'self';".
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::WorkerSrc,
      "worker-src blob: chrome://resources 'self';");

  web_ui->AddMessageHandler(
      std::make_unique<WelcomeDOMHandler>(Profile::FromWebUI(web_ui)));
  web_ui->AddMessageHandler(
      std::make_unique<settings::HnsImportBulkDataHandler>());
  web_ui->AddMessageHandler(
      std::make_unique<settings::DefaultBrowserHandler>());  // set default
                                                             // browser

  Profile* profile = Profile::FromWebUI(web_ui);
  // added to allow front end to read/modify default search engine
  web_ui->AddMessageHandler(
      std::make_unique<settings::HnsSearchEnginesHandler>(profile));

  // Open additional page in Japanese region
  int country_id = country_codes::GetCountryIDFromPrefs(profile->GetPrefs());
  if (!profile->GetPrefs()->GetBoolean(prefs::kHasSeenWelcomePage)) {
    if (country_id == country_codes::CountryStringToCountryID("JP")) {
      base::SingleThreadTaskRunner::GetCurrentDefault()->PostDelayedTask(
          FROM_HERE, base::BindOnce(&OpenJapanWelcomePage, profile),
          base::Seconds(3));
    }
  }

  for (const auto& str : kLocalizedStrings) {
    std::u16string l10n_str =
        hns_l10n::GetLocalizedResourceUTF16String(str.id);
    source->AddString(str.name, l10n_str);
  }

  // Variables considered when determining which onboarding cards to show
  source->AddString("countryString", CountryIDToCountryString(country_id));
  source->AddBoolean(
      "showRewardsCard",
      base::FeatureList::IsEnabled(hns_welcome::features::kShowRewardsCard));

  source->AddBoolean(
      "hardwareAccelerationEnabledAtStartup",
      content::GpuDataManager::GetInstance()->HardwareAccelerationEnabled());

  profile->GetPrefs()->SetBoolean(prefs::kHasSeenWelcomePage, true);

  AddBackgroundColorToSource(source, web_ui->GetWebContents());
}

HnsWelcomeUI::~HnsWelcomeUI() = default;
