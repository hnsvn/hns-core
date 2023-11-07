// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/new_tab_page/hns_new_tab_message_handler.h"

#include <memory>
#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/json/json_writer.h"
#include "base/json/values_util.h"
#include "base/memory/weak_ptr.h"
#include "base/metrics/histogram_macros.h"
#include "base/threading/thread_restrictions.h"
#include "base/time/time.h"
#include "base/values.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/ntp_background/view_counter_service_factory.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/search_engines/pref_names.h"
#include "hns/browser/search_engines/search_engine_provider_util.h"
#include "hns/browser/ui/webui/new_tab_page/hns_new_tab_ui.h"
#include "hns/components/hns_ads/core/public/ads_util.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_perf_predictor/common/pref_names.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/ntp_background_images/browser/url_constants.h"
#include "hns/components/ntp_background_images/browser/view_counter_service.h"
#include "hns/components/ntp_background_images/common/pref_names.h"
#include "hns/components/p3a/utils.h"
#include "hns/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#include "hns/components/time_period_storage/weekly_storage.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/plural_string_handler.h"
#include "chrome/common/chrome_features.h"
#include "components/grit/hns_components_strings.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui_data_source.h"

using ntp_background_images::ViewCounterServiceFactory;
using ntp_background_images::prefs::kBrandedWallpaperNotificationDismissed;
using ntp_background_images::prefs::kNewTabPageShowBackgroundImage;
using ntp_background_images::prefs::
    kNewTabPageShowSponsoredImagesBackgroundImage;  // NOLINT

namespace {

bool IsPrivateNewTab(Profile* profile) {
  return profile->IsIncognitoProfile() || profile->IsGuestSession();
}

base::Value::Dict GetStatsDictionary(PrefService* prefs) {
  base::Value::Dict stats_data;
  stats_data.Set("adsBlockedStat",
                 base::Int64ToValue(prefs->GetUint64(kAdsBlocked) +
                                    prefs->GetUint64(kTrackersBlocked)));
  stats_data.Set("javascriptBlockedStat",
                 base::Int64ToValue(prefs->GetUint64(kJavascriptBlocked)));
  stats_data.Set("fingerprintingBlockedStat",
                 base::Int64ToValue(prefs->GetUint64(kFingerprintingBlocked)));
  stats_data.Set("bandwidthSavedStat",
                 base::Int64ToValue(prefs->GetUint64(
                     hns_perf_predictor::prefs::kBandwidthSavedBytes)));
  return stats_data;
}

base::Value::Dict GetPreferencesDictionary(PrefService* prefs) {
  base::Value::Dict pref_data;
  pref_data.Set("showBackgroundImage",
                prefs->GetBoolean(kNewTabPageShowBackgroundImage));
  pref_data.Set(
      "brandedWallpaperOptIn",
      prefs->GetBoolean(kNewTabPageShowSponsoredImagesBackgroundImage));
  pref_data.Set("showClock", prefs->GetBoolean(kNewTabPageShowClock));
  pref_data.Set("clockFormat", prefs->GetString(kNewTabPageClockFormat));
  pref_data.Set("showStats", prefs->GetBoolean(kNewTabPageShowStats));
  pref_data.Set("showToday",
                prefs->GetBoolean(hns_news::prefs::kNewTabPageShowToday));
  pref_data.Set("showRewards", prefs->GetBoolean(kNewTabPageShowRewards));
  pref_data.Set("isBrandedWallpaperNotificationDismissed",
                prefs->GetBoolean(kBrandedWallpaperNotificationDismissed));
  pref_data.Set("isHnsNewsOptedIn",
                prefs->GetBoolean(hns_news::prefs::kHnsNewsOptedIn));
  pref_data.Set("hideAllWidgets", prefs->GetBoolean(kNewTabPageHideAllWidgets));
  pref_data.Set("showHnsTalk", prefs->GetBoolean(kNewTabPageShowHnsTalk));
  return pref_data;
}

base::Value::Dict GetPrivatePropertiesDictionary(PrefService* prefs) {
  base::Value::Dict private_data;
  private_data.Set(
      "useAlternativePrivateSearchEngine",
      prefs->GetBoolean(kUseAlternativePrivateSearchEngineProvider));
  private_data.Set(
      "showAlternativePrivateSearchEngineToggle",
      prefs->GetBoolean(kShowAlternativePrivateSearchEngineProviderToggle));
  return private_data;
}

// TODO(petemill): Move p3a to own NTP component so it can
// be used by other platforms.

enum class NTPCustomizeUsage { kNeverOpened, kOpened, kOpenedAndEdited, kSize };

const char kNTPCustomizeUsageStatus[] =
    "hns.new_tab_page.customize_p3a_usage";

const char kNeedsBrowserUpgradeToServeAds[] = "needsBrowserUpgradeToServeAds";

}  // namespace

// static
void HnsNewTabMessageHandler::RegisterLocalStatePrefs(
    PrefRegistrySimple* local_state) {
  local_state->RegisterIntegerPref(kNTPCustomizeUsageStatus, -1);
}

void HnsNewTabMessageHandler::RecordInitialP3AValues(
    PrefService* local_state) {
  p3a::RecordValueIfGreater<NTPCustomizeUsage>(
      NTPCustomizeUsage::kNeverOpened, "Hns.NTP.CustomizeUsageStatus",
      kNTPCustomizeUsageStatus, local_state);
}

// static
HnsNewTabMessageHandler* HnsNewTabMessageHandler::Create(
    content::WebUIDataSource* source,
    Profile* profile,
    bool was_invisible_and_restored) {
  //
  // Initial Values
  // Should only contain data that is static
  //
  auto* ads_service = hns_ads::AdsServiceFactory::GetForProfile(profile);
  // For safety, default |is_ads_supported_locale_| to true. Better to have
  // false positive than falsen egative,
  // in which case we would not show "opt out" toggle.
  bool is_ads_supported_locale = true;
  if (!ads_service) {
    LOG(ERROR) << "Ads service is not initialized!";
  } else {
    is_ads_supported_locale = hns_ads::IsSupportedRegion();
  }

  source->AddBoolean("featureFlagHnsNTPSponsoredImagesWallpaper",
                     is_ads_supported_locale);

  // Private Tab info
  if (IsPrivateNewTab(profile)) {
    source->AddBoolean("isTor", profile->IsTor());
    source->AddBoolean("isQwant", hns::IsRegionForQwant(profile));
  }
  return new HnsNewTabMessageHandler(profile, was_invisible_and_restored);
}

HnsNewTabMessageHandler::HnsNewTabMessageHandler(
    Profile* profile,
    bool was_invisible_and_restored)
    : profile_(profile),
      was_invisible_and_restored_(was_invisible_and_restored),
      weak_ptr_factory_(this) {
  ads_service_ = hns_ads::AdsServiceFactory::GetForProfile(profile_);
}

HnsNewTabMessageHandler::~HnsNewTabMessageHandler() = default;

void HnsNewTabMessageHandler::RegisterMessages() {
  // TODO(petemill): This MessageHandler can be split up to
  // individual MessageHandlers for each individual topic area,
  // should other WebUI pages wish to consume the APIs:
  // - Stats
  // - Preferences
  // - PrivatePage properties
  auto plural_string_handler = std::make_unique<PluralStringHandler>();
  plural_string_handler->AddLocalizedString("hnsNewsSourceCount",
                                            IDS_HNS_NEWS_SOURCE_COUNT);
  plural_string_handler->AddLocalizedString("rewardsPublisherCountText",
                                            IDS_REWARDS_PUBLISHER_COUNT_TEXT);
  web_ui()->AddMessageHandler(std::move(plural_string_handler));

  web_ui()->RegisterMessageCallback(
      "getNewTabPagePreferences",
      base::BindRepeating(&HnsNewTabMessageHandler::HandleGetPreferences,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabPageStats",
      base::BindRepeating(&HnsNewTabMessageHandler::HandleGetStats,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabPagePrivateProperties",
      base::BindRepeating(
          &HnsNewTabMessageHandler::HandleGetPrivateProperties,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabAdsData",
      base::BindRepeating(&HnsNewTabMessageHandler::HandleGetNewTabAdsData,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "toggleAlternativePrivateSearchEngine",
      base::BindRepeating(&HnsNewTabMessageHandler::
                              HandleToggleAlternativeSearchEngineProvider,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "saveNewTabPagePref",
      base::BindRepeating(&HnsNewTabMessageHandler::HandleSaveNewTabPagePref,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "registerNewTabPageView",
      base::BindRepeating(
          &HnsNewTabMessageHandler::HandleRegisterNewTabPageView,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "brandedWallpaperLogoClicked",
      base::BindRepeating(
          &HnsNewTabMessageHandler::HandleBrandedWallpaperLogoClicked,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getWallpaperData",
      base::BindRepeating(&HnsNewTabMessageHandler::HandleGetWallpaperData,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "customizeClicked",
      base::BindRepeating(&HnsNewTabMessageHandler::HandleCustomizeClicked,
                          base::Unretained(this)));
}

void HnsNewTabMessageHandler::OnJavascriptAllowed() {
  // Observe relevant preferences
  PrefService* prefs = profile_->GetPrefs();
  pref_change_registrar_.Init(prefs);
  // Stats
  pref_change_registrar_.Add(
      kAdsBlocked,
      base::BindRepeating(&HnsNewTabMessageHandler::OnStatsChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kTrackersBlocked,
      base::BindRepeating(&HnsNewTabMessageHandler::OnStatsChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kJavascriptBlocked,
      base::BindRepeating(&HnsNewTabMessageHandler::OnStatsChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kHttpsUpgrades,
      base::BindRepeating(&HnsNewTabMessageHandler::OnStatsChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kFingerprintingBlocked,
      base::BindRepeating(&HnsNewTabMessageHandler::OnStatsChanged,
                          base::Unretained(this)));

  if (IsPrivateNewTab(profile_)) {
    // Private New Tab Page preferences
    pref_change_registrar_.Add(
        kUseAlternativePrivateSearchEngineProvider,
        base::BindRepeating(
            &HnsNewTabMessageHandler::OnPrivatePropertiesChanged,
            base::Unretained(this)));
  }
  // News
  pref_change_registrar_.Add(
      hns_news::prefs::kHnsNewsOptedIn,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  // New Tab Page preferences
  pref_change_registrar_.Add(
      kNewTabPageShowBackgroundImage,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kNewTabPageShowSponsoredImagesBackgroundImage,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kNewTabPageShowClock,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kNewTabPageClockFormat,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kNewTabPageShowStats,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      hns_news::prefs::kNewTabPageShowToday,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kNewTabPageShowRewards,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kBrandedWallpaperNotificationDismissed,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kNewTabPageShowHnsTalk,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kNewTabPageHideAllWidgets,
      base::BindRepeating(&HnsNewTabMessageHandler::OnPreferencesChanged,
                          base::Unretained(this)));

  if (ads_service_) {
    ads_service_observation_.Reset();
    ads_service_observation_.Observe(ads_service_);
  }
}

void HnsNewTabMessageHandler::OnJavascriptDisallowed() {
  pref_change_registrar_.RemoveAll();
  ads_service_observation_.Reset();
  weak_ptr_factory_.InvalidateWeakPtrs();
}

void HnsNewTabMessageHandler::HandleGetPreferences(
    const base::Value::List& args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPreferencesDictionary(prefs);
  ResolveJavascriptCallback(args[0], data);
}

void HnsNewTabMessageHandler::HandleGetStats(const base::Value::List& args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetStatsDictionary(prefs);
  ResolveJavascriptCallback(args[0], data);
}

void HnsNewTabMessageHandler::HandleGetPrivateProperties(
    const base::Value::List& args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPrivatePropertiesDictionary(prefs);
  ResolveJavascriptCallback(args[0], data);
}

void HnsNewTabMessageHandler::HandleGetNewTabAdsData(
    const base::Value::List& args) {
  AllowJavascript();

  ResolveJavascriptCallback(args[0], GetAdsDataDictionary());
}

void HnsNewTabMessageHandler::HandleToggleAlternativeSearchEngineProvider(
    const base::Value::List& args) {
  // Alternative search related code will not be used.
  // Cleanup "toggleAlternativePrivateSearchEngine" message handler when it's
  // deleted from NTP Webui.
  // https://github.com/hnsvn/hns-browser/issues/23493
  NOTREACHED();
}

void HnsNewTabMessageHandler::HandleSaveNewTabPagePref(
    const base::Value::List& args) {
  if (args.size() != 2) {
    LOG(ERROR) << "Invalid input";
    return;
  }
  p3a::RecordValueIfGreater<NTPCustomizeUsage>(
      NTPCustomizeUsage::kOpenedAndEdited, "Hns.NTP.CustomizeUsageStatus",
      kNTPCustomizeUsageStatus, g_browser_process->local_state());
  PrefService* prefs = profile_->GetPrefs();
  // Collect args
  std::string settingsKeyInput = args[0].GetString();
  auto settingsValue = args[1].Clone();
  std::string settingsKey;

  // Handle string settings
  if (settingsValue.is_string()) {
    const auto settingsValueString = settingsValue.GetString();
    if (settingsKeyInput == "clockFormat") {
      settingsKey = kNewTabPageClockFormat;
    } else {
      LOG(ERROR) << "Invalid setting key";
      return;
    }
    prefs->SetString(settingsKey, settingsValueString);
    return;
  }

  // Handle bool settings
  if (!settingsValue.is_bool()) {
    LOG(ERROR) << "Invalid value type";
    return;
  }
  const auto settingsValueBool = settingsValue.GetBool();
  if (settingsKeyInput == "showBackgroundImage") {
    settingsKey = kNewTabPageShowBackgroundImage;
  } else if (settingsKeyInput == "brandedWallpaperOptIn") {
    // TODO(simonhong): I think above |brandedWallpaperOptIn| should be changed
    // to |sponsoredImagesWallpaperOptIn|.
    settingsKey = kNewTabPageShowSponsoredImagesBackgroundImage;
  } else if (settingsKeyInput == "showClock") {
    settingsKey = kNewTabPageShowClock;
  } else if (settingsKeyInput == "showStats") {
    settingsKey = kNewTabPageShowStats;
  } else if (settingsKeyInput == "showToday") {
    settingsKey = hns_news::prefs::kNewTabPageShowToday;
  } else if (settingsKeyInput == "isHnsNewsOptedIn") {
    settingsKey = hns_news::prefs::kHnsNewsOptedIn;
  } else if (settingsKeyInput == "showRewards") {
    settingsKey = kNewTabPageShowRewards;
  } else if (settingsKeyInput == "isBrandedWallpaperNotificationDismissed") {
    settingsKey = kBrandedWallpaperNotificationDismissed;
  } else if (settingsKeyInput == "hideAllWidgets") {
    settingsKey = kNewTabPageHideAllWidgets;
  } else if (settingsKeyInput == "showHnsTalk") {
    settingsKey = kNewTabPageShowHnsTalk;
  } else {
    LOG(ERROR) << "Invalid setting key";
    return;
  }
  prefs->SetBoolean(settingsKey, settingsValueBool);

  // P3A can only be recorded after profile is updated
  if (settingsKeyInput == "showBackgroundImage" ||
      settingsKeyInput == "brandedWallpaperOptIn") {
    hns::RecordSponsoredImagesEnabledP3A(profile_);
  }
}

void HnsNewTabMessageHandler::HandleRegisterNewTabPageView(
    const base::Value::List& args) {
  AllowJavascript();

  // Decrement original value only if there's actual branded content and we are
  // not restoring invisible (hidden or occluded) browser tabs.
  if (was_invisible_and_restored_) {
    was_invisible_and_restored_ = false;
    return;
  }

  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile_))
    service->RegisterPageView();
}

void HnsNewTabMessageHandler::HandleBrandedWallpaperLogoClicked(
    const base::Value::List& args) {
  AllowJavascript();
  if (args.size() != 1) {
    LOG(ERROR) << "Invalid input";
    return;
  }

  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile_)) {
    const auto& arg = args[0].GetDict();
    auto* creative_instance_id =
        arg.FindString(ntp_background_images::kCreativeInstanceIDKey);
    auto* destination_url = arg.FindStringByDottedPath(
        ntp_background_images::kLogoDestinationURLPath);
    auto* wallpaper_id =
        arg.FindStringByDottedPath(ntp_background_images::kWallpaperIDKey);

    DCHECK(creative_instance_id);
    DCHECK(destination_url);
    DCHECK(wallpaper_id);

    service->BrandedWallpaperLogoClicked(
        creative_instance_id ? *creative_instance_id : "",
        destination_url ? *destination_url : "",
        wallpaper_id ? *wallpaper_id : "");
  }
}

void HnsNewTabMessageHandler::HandleGetWallpaperData(
    const base::Value::List& args) {
  AllowJavascript();

  auto* service = ViewCounterServiceFactory::GetForProfile(profile_);
  base::Value::Dict wallpaper;

  if (!service) {
    ResolveJavascriptCallback(args[0], wallpaper);
    return;
  }

  absl::optional<base::Value::Dict> data =
      service->GetCurrentWallpaperForDisplay();

  if (!data) {
    ResolveJavascriptCallback(args[0], wallpaper);
    return;
  }

  const auto is_background =
      data->FindBool(ntp_background_images::kIsBackgroundKey);
  DCHECK(is_background);

  constexpr char kBackgroundWallpaperKey[] = "backgroundWallpaper";
  if (is_background.value()) {
    wallpaper.Set(kBackgroundWallpaperKey, std::move(*data));
    ResolveJavascriptCallback(args[0], wallpaper);
    return;
  }

  // Even though we show sponsored image, we should pass "Background wallpaper"
  // data so that NTP customization menu can know which wallpaper is selected by
  // users.
  auto backgroundWallpaper = service->GetCurrentWallpaper();
  wallpaper.Set(kBackgroundWallpaperKey,
                backgroundWallpaper
                    ? base::Value(std::move(*backgroundWallpaper))
                    : base::Value());

  const std::string* creative_instance_id =
      data->FindString(ntp_background_images::kCreativeInstanceIDKey);
  const std::string* wallpaper_id =
      data->FindString(ntp_background_images::kWallpaperIDKey);
  service->BrandedWallpaperWillBeDisplayed(
      wallpaper_id ? *wallpaper_id : "",
      creative_instance_id ? *creative_instance_id : "");

  constexpr char kBrandedWallpaperKey[] = "brandedWallpaper";
  wallpaper.Set(kBrandedWallpaperKey, std::move(*data));
  ResolveJavascriptCallback(args[0], wallpaper);
}

void HnsNewTabMessageHandler::HandleCustomizeClicked(
    const base::Value::List& args) {
  AllowJavascript();
  p3a::RecordValueIfGreater<NTPCustomizeUsage>(
      NTPCustomizeUsage::kOpened, "Hns.NTP.CustomizeUsageStatus",
      kNTPCustomizeUsageStatus, g_browser_process->local_state());
}

void HnsNewTabMessageHandler::OnPrivatePropertiesChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPrivatePropertiesDictionary(prefs);
  FireWebUIListener("private-tab-data-updated", data);
}

void HnsNewTabMessageHandler::OnStatsChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetStatsDictionary(prefs);
  FireWebUIListener("stats-updated", data);
}

void HnsNewTabMessageHandler::OnPreferencesChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPreferencesDictionary(prefs);
  FireWebUIListener("preferences-changed", data);
}

base::Value::Dict HnsNewTabMessageHandler::GetAdsDataDictionary() const {
  base::Value::Dict ads_data;

  bool needs_browser_update_to_see_ads = false;
  if (ads_service_) {
    needs_browser_update_to_see_ads =
        ads_service_->NeedsBrowserUpgradeToServeAds();
  }
  ads_data.Set(kNeedsBrowserUpgradeToServeAds, needs_browser_update_to_see_ads);

  return ads_data;
}

void HnsNewTabMessageHandler::OnNeedsBrowserUpgradeToServeAds() {
  FireWebUIListener("new-tab-ads-data-updated", GetAdsDataDictionary());
}
