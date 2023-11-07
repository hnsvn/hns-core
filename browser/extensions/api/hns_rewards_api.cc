/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/api/hns_rewards_api.h"

#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/strings/string_number_conversions.h"
#include "hns/browser/hns_adaptive_captcha/hns_adaptive_captcha_service_factory.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/hns_rewards/rewards_tab_helper.h"
#include "hns/browser/hns_rewards/rewards_util.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/ui/hns_rewards/rewards_panel_coordinator.h"
#include "hns/browser/ui/hns_rewards/tip_panel_coordinator.h"
#include "hns/common/extensions/api/hns_rewards.h"
#include "hns/components/hns_adaptive_captcha/hns_adaptive_captcha_service.h"
#include "hns/components/hns_ads/browser/ads_service.h"
#include "hns/components/hns_ads/core/public/ads_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_ads/core/public/targeting/geographical/subdivision/supported_subdivisions.h"
#include "hns/components/hns_rewards/browser/rewards_p3a.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_rewards/common/rewards_util.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "extensions/common/constants.h"

using hns_ads::AdsService;
using hns_ads::AdsServiceFactory;
using hns_rewards::RewardsPanelCoordinator;
using hns_rewards::RewardsService;
using hns_rewards::RewardsServiceFactory;
using hns_rewards::RewardsTabHelper;
using hns_rewards::TipPanelCoordinator;

namespace {

RewardsTabHelper* GetRewardsTabHelperForTabId(
    int tab_id,
    content::BrowserContext* browser_context) {
  DCHECK(browser_context);
  content::WebContents* web_contents = nullptr;
  bool found = extensions::ExtensionTabUtil::GetTabById(
      tab_id, browser_context, false, nullptr, nullptr, &web_contents, nullptr);
  if (!found || !web_contents) {
    return nullptr;
  }
  return RewardsTabHelper::FromWebContents(web_contents);
}

content::WebContents* WebContentsFromBrowserContext(
    int tab_id,
    content::BrowserContext* browser_context) {
  content::WebContents* contents = nullptr;
  extensions::ExtensionTabUtil::GetTabById(
      tab_id, Profile::FromBrowserContext(browser_context), false, nullptr,
      nullptr, &contents, nullptr);
  return contents;
}

RewardsPanelCoordinator* GetPanelCoordinator(
    content::WebContents* web_contents) {
  DCHECK(web_contents);
  auto* browser = chrome::FindBrowserWithWebContents(web_contents);
  return browser ? RewardsPanelCoordinator::FromBrowser(browser) : nullptr;
}

RewardsPanelCoordinator* GetPanelCoordinator(ExtensionFunction* function) {
  DCHECK(function);
  auto* web_contents = function->GetSenderWebContents();
  if (!web_contents) {
    return nullptr;
  }
  return GetPanelCoordinator(web_contents);
}

TipPanelCoordinator* GetTipPanelCoordinator(
    int tab_id,
    content::BrowserContext* browser_context) {
  auto* contents = WebContentsFromBrowserContext(tab_id, browser_context);
  if (!contents) {
    return nullptr;
  }

  auto* browser = chrome::FindBrowserWithWebContents(contents);
  if (!browser) {
    return nullptr;
  }

  return hns_rewards::TipPanelCoordinator::FromBrowser(browser);
}

std::string StringifyResult(
    hns_rewards::mojom::CreateRewardsWalletResult result) {
  switch (result) {
    case hns_rewards::mojom::CreateRewardsWalletResult::kSuccess:
      return "success";
    case hns_rewards::mojom::CreateRewardsWalletResult::
        kWalletGenerationDisabled:
      return "wallet-generation-disabled";
    case hns_rewards::mojom::CreateRewardsWalletResult::
        kGeoCountryAlreadyDeclared:
      return "country-already-declared";
    case hns_rewards::mojom::CreateRewardsWalletResult::kUnexpected:
      return "unexpected-error";
  }
}

}  // namespace

namespace extensions::api {

HnsRewardsIsSupportedFunction::~HnsRewardsIsSupportedFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsIsSupportedFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  bool is_supported = ::hns_rewards::IsSupportedForProfile(
      profile, ::hns_rewards::IsSupportedOptions::kSkipRegionCheck);
  return RespondNow(WithArguments(is_supported));
}

HnsRewardsIsUnsupportedRegionFunction::
    ~HnsRewardsIsUnsupportedRegionFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsIsUnsupportedRegionFunction::Run() {
  bool is_unsupported_region = ::hns_rewards::IsUnsupportedRegion();
  return RespondNow(WithArguments(is_unsupported_region));
}

HnsRewardsRecordNTPPanelTriggerFunction::
    ~HnsRewardsRecordNTPPanelTriggerFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsRecordNTPPanelTriggerFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  if (!profile->GetPrefs()->GetBoolean(::hns_rewards::prefs::kEnabled)) {
    rewards_service->GetP3AConversionMonitor()->RecordPanelTrigger(
        ::hns_rewards::p3a::PanelTrigger::kNTP);
  }

  return RespondNow(NoArguments());
}

HnsRewardsOpenRewardsPanelFunction::~HnsRewardsOpenRewardsPanelFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsOpenRewardsPanelFunction::Run() {
  if (auto* coordinator = GetPanelCoordinator(this)) {
    coordinator->OpenRewardsPanel();
  }
  return RespondNow(NoArguments());
}

HnsRewardsShowRewardsSetupFunction::~HnsRewardsShowRewardsSetupFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsShowRewardsSetupFunction::Run() {
  if (auto* coordinator = GetPanelCoordinator(this)) {
    coordinator->ShowRewardsSetup();
  }
  return RespondNow(NoArguments());
}

HnsRewardsShowGrantCaptchaFunction::~HnsRewardsShowGrantCaptchaFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsShowGrantCaptchaFunction::Run() {
  auto params = hns_rewards::ShowGrantCaptcha::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  if (auto* coordinator = GetPanelCoordinator(this)) {
    coordinator->ShowGrantCaptcha(params->grant_id);
  }

  return RespondNow(NoArguments());
}

HnsRewardsUpdateMediaDurationFunction::
    ~HnsRewardsUpdateMediaDurationFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsUpdateMediaDurationFunction::Run() {
  absl::optional<hns_rewards::UpdateMediaDuration::Params> params =
      hns_rewards::UpdateMediaDuration::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  rewards_service->UpdateMediaDuration(params->window_id, params->publisher_key,
                                       params->duration, params->first_visit);

  return RespondNow(NoArguments());
}

HnsRewardsGetPublisherInfoFunction::~HnsRewardsGetPublisherInfoFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsGetPublisherInfoFunction::Run() {
  absl::optional<hns_rewards::GetPublisherInfo::Params> params =
      hns_rewards::GetPublisherInfo::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetPublisherInfo(
      params->publisher_key,
      base::BindOnce(&HnsRewardsGetPublisherInfoFunction::OnGetPublisherInfo,
                     this));

  return RespondLater();
}

void HnsRewardsGetPublisherInfoFunction::OnGetPublisherInfo(
    const ::hns_rewards::mojom::Result result,
    ::hns_rewards::mojom::PublisherInfoPtr info) {
  if (!info) {
    Respond(WithArguments(static_cast<int>(result)));
    return;
  }

  base::Value::Dict dict;
  dict.Set("publisherKey", info->id);
  dict.Set("name", info->name);
  dict.Set("percentage", static_cast<int>(info->percent));
  dict.Set("status", static_cast<int>(info->status));
  dict.Set("excluded", info->excluded ==
                           ::hns_rewards::mojom::PublisherExclude::EXCLUDED);
  dict.Set("url", info->url);
  dict.Set("provider", info->provider);
  dict.Set("favIconUrl", info->favicon_url);

  Respond(WithArguments(static_cast<int>(result), std::move(dict)));
}

HnsRewardsSetPublisherIdForTabFunction::
    ~HnsRewardsSetPublisherIdForTabFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsSetPublisherIdForTabFunction::Run() {
  auto params = hns_rewards::SetPublisherIdForTab::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  auto* tab_helper =
      GetRewardsTabHelperForTabId(params->tab_id, browser_context());

  if (tab_helper) {
    tab_helper->SetPublisherIdForTab(params->publisher_id);
  }

  return RespondNow(NoArguments());
}

HnsRewardsGetPublisherInfoForTabFunction::
    ~HnsRewardsGetPublisherInfoForTabFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetPublisherInfoForTabFunction::Run() {
  auto params = hns_rewards::GetPublisherInfoForTab::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  auto* profile = Profile::FromBrowserContext(browser_context());

  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  auto* tab_helper = GetRewardsTabHelperForTabId(params->tab_id, profile);
  if (!tab_helper) {
    return RespondNow(NoArguments());
  }

  std::string publisher_id = tab_helper->GetPublisherIdForTab();
  if (publisher_id.empty()) {
    return RespondNow(NoArguments());
  }

  rewards_service->GetPublisherPanelInfo(
      publisher_id,
      base::BindOnce(
          &HnsRewardsGetPublisherInfoForTabFunction::OnGetPublisherPanelInfo,
          this));

  return RespondLater();
}

void HnsRewardsGetPublisherInfoForTabFunction::OnGetPublisherPanelInfo(
    ::hns_rewards::mojom::Result result,
    ::hns_rewards::mojom::PublisherInfoPtr info) {
  if (!info) {
    Respond(NoArguments());
    return;
  }

  base::Value::Dict dict;
  dict.Set("publisherKey", info->id);
  dict.Set("name", info->name);
  dict.Set("percentage", static_cast<int>(info->percent));
  dict.Set("status", static_cast<int>(info->status));
  dict.Set("excluded", info->excluded ==
                           ::hns_rewards::mojom::PublisherExclude::EXCLUDED);
  dict.Set("url", info->url);
  dict.Set("provider", info->provider);
  dict.Set("favIconUrl", info->favicon_url);

  Respond(WithArguments(std::move(dict)));
}

HnsRewardsGetPublisherPanelInfoFunction::
    ~HnsRewardsGetPublisherPanelInfoFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetPublisherPanelInfoFunction::Run() {
  absl::optional<hns_rewards::GetPublisherPanelInfo::Params> params =
      hns_rewards::GetPublisherPanelInfo::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->GetPublisherPanelInfo(
      params->publisher_key,
      base::BindOnce(
          &HnsRewardsGetPublisherPanelInfoFunction::OnGetPublisherPanelInfo,
          this));

  return RespondLater();
}

void HnsRewardsGetPublisherPanelInfoFunction::OnGetPublisherPanelInfo(
    const ::hns_rewards::mojom::Result result,
    ::hns_rewards::mojom::PublisherInfoPtr info) {
  if (!info) {
    Respond(WithArguments(static_cast<int>(result)));
    return;
  }

  base::Value::Dict dict;
  dict.Set("publisherKey", info->id);
  dict.Set("name", info->name);
  dict.Set("percentage", static_cast<int>(info->percent));
  dict.Set("status", static_cast<int>(info->status));
  dict.Set("excluded", info->excluded ==
                           ::hns_rewards::mojom::PublisherExclude::EXCLUDED);
  dict.Set("url", info->url);
  dict.Set("provider", info->provider);
  dict.Set("favIconUrl", info->favicon_url);

  Respond(WithArguments(static_cast<int>(result), std::move(dict)));
}

HnsRewardsSavePublisherInfoFunction::
    ~HnsRewardsSavePublisherInfoFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsSavePublisherInfoFunction::Run() {
  absl::optional<hns_rewards::SavePublisherInfo::Params> params =
      hns_rewards::SavePublisherInfo::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  auto publisher_info = ::hns_rewards::mojom::PublisherInfo::New();
  publisher_info->id = params->publisher_key;
  publisher_info->name = params->publisher_name;
  publisher_info->url = params->url;
  publisher_info->provider = params->media_type;
  publisher_info->favicon_url = params->fav_icon_url;

  rewards_service->SavePublisherInfo(
      params->window_id, std::move(publisher_info),
      base::BindOnce(
          &HnsRewardsSavePublisherInfoFunction::OnSavePublisherInfo, this));

  return RespondLater();
}

void HnsRewardsSavePublisherInfoFunction::OnSavePublisherInfo(
    const ::hns_rewards::mojom::Result result) {
  Respond(WithArguments(static_cast<int>(result)));
}

HnsRewardsTipSiteFunction::~HnsRewardsTipSiteFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsTipSiteFunction::Run() {
  absl::optional<hns_rewards::TipSite::Params> params =
      hns_rewards::TipSite::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  // Sanity check: don't allow tips in private / tor contexts,
  // although the command should not have been enabled in the first place.
  if (!hns::IsRegularProfile(browser_context())) {
    return RespondNow(Error("Cannot tip to site in a private context"));
  }

  auto* coordinator = GetTipPanelCoordinator(params->tab_id, browser_context());
  if (!coordinator) {
    return RespondNow(Error(tabs_constants::kTabNotFoundError,
                            base::NumberToString(params->tab_id)));
  }

  coordinator->ShowPanelForPublisher(params->publisher_key);
  return RespondNow(NoArguments());
}

HnsRewardsTipUserFunction::~HnsRewardsTipUserFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsTipUserFunction::Run() {
  absl::optional<hns_rewards::TipUser::Params> params =
      hns_rewards::TipUser::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  // Sanity check: don't allow tips in private / tor contexts,
  // although the command should not have been enabled in the first place.
  if (!hns::IsRegularProfile(browser_context())) {
    return RespondNow(Error("Cannot tip user in a private context"));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  bool rewards_enabled =
      profile->GetPrefs()->GetBoolean(::hns_rewards::prefs::kEnabled);

  if (!profile->GetPrefs()->GetBoolean(::hns_rewards::prefs::kEnabled)) {
    rewards_service->GetP3AConversionMonitor()->RecordPanelTrigger(
        ::hns_rewards::p3a::PanelTrigger::kInlineTip);
  }

  // If the user clicks the tipping button before having opted into the Rewards,
  // then the Rewards service would not have started the engine process yet. We
  // need to open the Rewards panel for the user to offer opting in.
  if (!rewards_enabled) {
    // Get web contents for this tab
    content::WebContents* contents =
        WebContentsFromBrowserContext(params->tab_id, browser_context());
    if (!contents) {
      return RespondNow(Error(tabs_constants::kTabNotFoundError,
                              base::NumberToString(params->tab_id)));
    }
    auto* coordinator = GetPanelCoordinator(contents);
    if (!coordinator) {
      return RespondNow(Error("Unable to open Rewards panel"));
    }
    coordinator->OpenRewardsPanel();
    return RespondNow(NoArguments());
  }

  AddRef();

  rewards_service->GetPublisherInfo(
      params->publisher_key,
      base::BindOnce(&HnsRewardsTipUserFunction::OnTipUserGetPublisherInfo,
                     this));

  return RespondNow(NoArguments());
}

void HnsRewardsTipUserFunction::OnTipUserGetPublisherInfo(
    const ::hns_rewards::mojom::Result result,
    ::hns_rewards::mojom::PublisherInfoPtr info) {
  if (result != ::hns_rewards::mojom::Result::OK &&
      result != ::hns_rewards::mojom::Result::NOT_FOUND) {
    Release();
    return;
  }

  if (result == ::hns_rewards::mojom::Result::OK) {
    ShowTipDialog();
    Release();
    return;
  }

  absl::optional<hns_rewards::TipUser::Params> params =
      hns_rewards::TipUser::Params::Create(args());

  auto publisher_info = ::hns_rewards::mojom::PublisherInfo::New();
  publisher_info->id = params->publisher_key;
  publisher_info->name = params->publisher_name;
  publisher_info->url = params->url;
  publisher_info->provider = params->media_type;
  publisher_info->favicon_url = params->fav_icon_url;

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    Release();
    return;
  }

  rewards_service->SavePublisherInfo(
      0, std::move(publisher_info),
      base::BindOnce(&HnsRewardsTipUserFunction::OnTipUserSavePublisherInfo,
                     this));
}

void HnsRewardsTipUserFunction::OnTipUserSavePublisherInfo(
    const ::hns_rewards::mojom::Result result) {
  if (result != ::hns_rewards::mojom::Result::OK) {
    Release();
    return;
  }

  ShowTipDialog();
  Release();
}

void HnsRewardsTipUserFunction::ShowTipDialog() {
  absl::optional<hns_rewards::TipUser::Params> params =
      hns_rewards::TipUser::Params::Create(args());
  if (!params) {
    Release();
    return;
  }

  auto* coordinator = GetTipPanelCoordinator(params->tab_id, browser_context());
  if (!coordinator) {
    Release();
    return;
  }

  coordinator->ShowPanelForInlineTip(params->publisher_key);
}

HnsRewardsIncludeInAutoContributionFunction::
    ~HnsRewardsIncludeInAutoContributionFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsIncludeInAutoContributionFunction::Run() {
  absl::optional<hns_rewards::IncludeInAutoContribution::Params> params =
      hns_rewards::IncludeInAutoContribution::Params::Create(args());
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (rewards_service) {
    rewards_service->SetPublisherExclude(params->publisher_key,
                                         params->exclude);
  }
  return RespondNow(NoArguments());
}

HnsRewardsGetPublisherDataFunction::~HnsRewardsGetPublisherDataFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsGetPublisherDataFunction::Run() {
  absl::optional<hns_rewards::GetPublisherData::Params> params =
      hns_rewards::GetPublisherData::Params::Create(args());
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (rewards_service) {
    rewards_service->GetPublisherActivityFromUrl(params->window_id, params->url,
                                                 params->favicon_url,
                                                 params->publisher_blob);
  }
  return RespondNow(NoArguments());
}

HnsRewardsGetRewardsParametersFunction::
    ~HnsRewardsGetRewardsParametersFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetRewardsParametersFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->GetRewardsParameters(base::BindOnce(
      &HnsRewardsGetRewardsParametersFunction::OnGetRewardsParameters, this));
  return RespondLater();
}

void HnsRewardsGetRewardsParametersFunction::OnGetRewardsParameters(
    ::hns_rewards::mojom::RewardsParametersPtr parameters) {
  base::Value::Dict data;

  if (!parameters) {
    data.Set("rate", 0.0);
    data.Set("monthlyTipChoices", base::Value::List());
    data.Set("autoContributeChoices", base::Value::List());
    data.Set("payoutStatus", base::Value::Dict());
    data.Set("walletProviderRegions", base::Value::Dict());
    data.Set("vbatExpired", false);
    return Respond(WithArguments(std::move(data)));
  }

  data.Set("rate", parameters->rate);
  base::Value::List monthly_choices;
  for (auto const& item : parameters->monthly_tip_choices) {
    monthly_choices.Append(item);
  }
  data.Set("monthlyTipChoices", std::move(monthly_choices));

  base::Value::List ac_choices;
  for (double const& choice : parameters->auto_contribute_choices) {
    ac_choices.Append(choice);
  }
  data.Set("autoContributeChoices", std::move(ac_choices));

  base::Value::Dict payout_status;
  for (const auto& [key, value] : parameters->payout_status) {
    payout_status.Set(key, value);
  }
  data.Set("payoutStatus", std::move(payout_status));

  base::Value::Dict provider_regions;
  for (const auto& [provider, regions] : parameters->wallet_provider_regions) {
    base::Value::List allow;
    for (const auto& country : regions->allow) {
      allow.Append(country);
    }
    base::Value::List block;
    for (const auto& country : regions->block) {
      block.Append(country);
    }
    base::Value::Dict regions_dict;
    regions_dict.Set("allow", std::move(allow));
    regions_dict.Set("block", std::move(block));
    provider_regions.Set(provider, std::move(regions_dict));
  }
  data.Set("walletProviderRegions", std::move(provider_regions));

  if (!parameters->vbat_deadline.is_null()) {
    data.Set("vbatDeadline", floor(parameters->vbat_deadline.ToDoubleT() *
                                   base::Time::kMillisecondsPerSecond));
  }
  data.Set("vbatExpired", parameters->vbat_expired);

  Respond(WithArguments(std::move(data)));
}

HnsRewardsCreateRewardsWalletFunction::
    ~HnsRewardsCreateRewardsWalletFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsCreateRewardsWalletFunction::Run() {
  auto params = hns_rewards::CreateRewardsWallet::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("RewardsService not available"));
  }

  rewards_service->CreateRewardsWallet(
      params->country,
      base::BindOnce(
          &HnsRewardsCreateRewardsWalletFunction::CreateRewardsWalletCallback,
          this));

  return RespondLater();
}

void HnsRewardsCreateRewardsWalletFunction::CreateRewardsWalletCallback(
    ::hns_rewards::mojom::CreateRewardsWalletResult result) {
  Respond(WithArguments(StringifyResult(result)));
}

HnsRewardsGetAvailableCountriesFunction::
    ~HnsRewardsGetAvailableCountriesFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetAvailableCountriesFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetAvailableCountries(base::BindOnce(
      &HnsRewardsGetAvailableCountriesFunction::GetAvailableCountriesCallback,
      this));

  return RespondLater();
}

void HnsRewardsGetAvailableCountriesFunction::GetAvailableCountriesCallback(
    std::vector<std::string> countries) {
  base::Value::List country_list;
  for (auto& country : countries) {
    country_list.Append(std::move(country));
  }
  Respond(WithArguments(std::move(country_list)));
}

HnsRewardsGetDefaultCountryFunction::
    ~HnsRewardsGetDefaultCountryFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsGetDefaultCountryFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  return RespondNow(WithArguments(rewards_service->GetCountryCode()));
}

HnsRewardsGetDeclaredCountryFunction::
    ~HnsRewardsGetDeclaredCountryFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetDeclaredCountryFunction::Run() {
  auto* prefs = Profile::FromBrowserContext(browser_context())->GetPrefs();
  std::string country = prefs->GetString(::hns_rewards::prefs::kDeclaredGeo);
  return RespondNow(WithArguments(std::move(country)));
}

HnsRewardsIsGrandfatheredUserFunction::
    ~HnsRewardsIsGrandfatheredUserFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsIsGrandfatheredUserFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  return RespondNow(WithArguments(
      rewards_service ? rewards_service->IsGrandfatheredUser() : false));
}

HnsRewardsGetUserTypeFunction::~HnsRewardsGetUserTypeFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsGetUserTypeFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(WithArguments(std::string()));
  }

  rewards_service->GetUserType(
      base::BindOnce(&HnsRewardsGetUserTypeFunction::Callback, this));

  return RespondLater();
}

void HnsRewardsGetUserTypeFunction::Callback(
    ::hns_rewards::mojom::UserType user_type) {
  auto map_user_type =
      [](::hns_rewards::mojom::UserType user_type) -> std::string {
    switch (user_type) {
      case ::hns_rewards::mojom::UserType::kLegacyUnconnected:
        return "legacy-unconnected";
      case ::hns_rewards::mojom::UserType::kConnected:
        return "connected";
      case ::hns_rewards::mojom::UserType::kUnconnected:
        return "unconnected";
    }
  };
  Respond(WithArguments(map_user_type(user_type)));
}

HnsRewardsGetPublishersVisitedCountFunction::
    ~HnsRewardsGetPublishersVisitedCountFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetPublishersVisitedCountFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(WithArguments(0));
  }

  rewards_service->GetPublishersVisitedCount(base::BindOnce(
      &HnsRewardsGetPublishersVisitedCountFunction::Callback, this));

  return RespondLater();
}

void HnsRewardsGetPublishersVisitedCountFunction::Callback(int count) {
  Respond(WithArguments(count));
}

HnsRewardsGetBalanceReportFunction::~HnsRewardsGetBalanceReportFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsGetBalanceReportFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  absl::optional<hns_rewards::GetBalanceReport::Params> params =
      hns_rewards::GetBalanceReport::Params::Create(args());

  rewards_service->GetBalanceReport(
      params->month, params->year,
      base::BindOnce(&HnsRewardsGetBalanceReportFunction::OnBalanceReport,
                     this));
  return RespondLater();
}

void HnsRewardsGetBalanceReportFunction::OnBalanceReport(
    const ::hns_rewards::mojom::Result result,
    ::hns_rewards::mojom::BalanceReportInfoPtr report) {
  base::Value::Dict data;
  data.Set("ads", report ? report->earning_from_ads : 0.0);
  data.Set("contribute", report ? report->auto_contribute : 0.0);
  data.Set("grant", report ? report->grants : 0.0);
  data.Set("tips", report ? report->one_time_donation : 0.0);
  data.Set("monthly", report ? report->recurring_donation : 0.0);
  Respond(WithArguments(std::move(data)));
}

HnsRewardsFetchPromotionsFunction::~HnsRewardsFetchPromotionsFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsFetchPromotionsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->FetchPromotions(base::BindOnce(
      &HnsRewardsFetchPromotionsFunction::OnPromotionsFetched, this));

  return RespondLater();
}

void HnsRewardsFetchPromotionsFunction::OnPromotionsFetched(
    std::vector<::hns_rewards::mojom::PromotionPtr> promotions) {
  base::Value::List list;
  for (auto& item : promotions) {
    base::Value::Dict dict;
    dict.Set("promotionId", item->id);
    dict.Set("type", static_cast<int>(item->type));
    dict.Set("status", static_cast<int>(item->status));
    dict.Set("createdAt", static_cast<double>(item->created_at));
    dict.Set("claimableUntil", static_cast<double>(item->claimable_until));
    dict.Set("expiresAt", static_cast<double>(item->expires_at));
    dict.Set("amount", item->approximate_value);
    list.Append(std::move(dict));
  }
  Respond(WithArguments(std::move(list)));
}

HnsRewardsClaimPromotionFunction::~HnsRewardsClaimPromotionFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsClaimPromotionFunction::Run() {
  absl::optional<hns_rewards::ClaimPromotion::Params> params =
      hns_rewards::ClaimPromotion::Params::Create(args());
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->ClaimPromotion(
      params->promotion_id,
      base::BindOnce(&HnsRewardsClaimPromotionFunction::OnClaimPromotion,
                     this, params->promotion_id));
  return RespondLater();
}

void HnsRewardsClaimPromotionFunction::OnClaimPromotion(
    const std::string& promotion_id,
    const ::hns_rewards::mojom::Result result,
    const std::string& captcha_image,
    const std::string& hint,
    const std::string& captcha_id) {
  base::Value::Dict data;
  data.Set("result", static_cast<int>(result));
  data.Set("promotionId", promotion_id);
  data.Set("captchaImage", captcha_image);
  data.Set("captchaId", captcha_id);
  data.Set("hint", hint);
  Respond(WithArguments(std::move(data)));
}

HnsRewardsAttestPromotionFunction::~HnsRewardsAttestPromotionFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsAttestPromotionFunction::Run() {
  absl::optional<hns_rewards::AttestPromotion::Params> params =
      hns_rewards::AttestPromotion::Params::Create(args());
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->AttestPromotion(
      params->promotion_id, params->solution,
      base::BindOnce(&HnsRewardsAttestPromotionFunction::OnAttestPromotion,
                     this, params->promotion_id));
  return RespondLater();
}

void HnsRewardsAttestPromotionFunction::OnAttestPromotion(
    const std::string& promotion_id,
    const ::hns_rewards::mojom::Result result,
    ::hns_rewards::mojom::PromotionPtr promotion) {
  base::Value::Dict data;
  data.Set("promotionId", promotion_id);

  if (!promotion) {
    Respond(WithArguments(static_cast<int>(result), std::move(data)));
    return;
  }

  data.Set("expiresAt", static_cast<double>(promotion->expires_at));
  data.Set("amount", static_cast<double>(promotion->approximate_value));
  data.Set("type", static_cast<int>(promotion->type));
  Respond(WithArguments(static_cast<int>(result), std::move(data)));
}

HnsRewardsSetAutoContributeEnabledFunction::
    ~HnsRewardsSetAutoContributeEnabledFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsSetAutoContributeEnabledFunction::Run() {
  absl::optional<hns_rewards::SetAutoContributeEnabled::Params> params =
      hns_rewards::SetAutoContributeEnabled::Params::Create(args());
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->SetAutoContributeEnabled(params->enabled);
  return RespondNow(NoArguments());
}

HnsRewardsGetACEnabledFunction::~HnsRewardsGetACEnabledFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsGetACEnabledFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetAutoContributeEnabled(
      base::BindOnce(&HnsRewardsGetACEnabledFunction::OnGetACEnabled, this));
  return RespondLater();
}

void HnsRewardsGetACEnabledFunction::OnGetACEnabled(bool enabled) {
  Respond(WithArguments(enabled));
}

HnsRewardsSaveRecurringTipFunction::~HnsRewardsSaveRecurringTipFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsSaveRecurringTipFunction::Run() {
  absl::optional<hns_rewards::SaveRecurringTip::Params> params =
      hns_rewards::SaveRecurringTip::Params::Create(args());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  rewards_service->SaveRecurringTip(
      params->publisher_key, params->new_amount,
      base::BindOnce(&HnsRewardsSaveRecurringTipFunction::OnSaveRecurringTip,
                     this));

  return RespondLater();
}

void HnsRewardsSaveRecurringTipFunction::OnSaveRecurringTip(
    ::hns_rewards::mojom::Result result) {
  Respond(result == ::hns_rewards::mojom::Result::OK
              ? NoArguments()
              : Error("Failed to save"));
}

HnsRewardsRemoveRecurringTipFunction::
    ~HnsRewardsRemoveRecurringTipFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsRemoveRecurringTipFunction::Run() {
  absl::optional<hns_rewards::RemoveRecurringTip::Params> params =
      hns_rewards::RemoveRecurringTip::Params::Create(args());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (rewards_service) {
    rewards_service->RemoveRecurringTip(params->publisher_key);
  }

  return RespondNow(NoArguments());
}

HnsRewardsGetRecurringTipsFunction::~HnsRewardsGetRecurringTipsFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsGetRecurringTipsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetRecurringTips(base::BindOnce(
      &HnsRewardsGetRecurringTipsFunction::OnGetRecurringTips, this));
  return RespondLater();
}

void HnsRewardsGetRecurringTipsFunction::OnGetRecurringTips(
    std::vector<::hns_rewards::mojom::PublisherInfoPtr> list) {
  base::Value::Dict result;
  base::Value::List recurring_tips;

  if (!list.empty()) {
    for (const auto& item : list) {
      base::Value::Dict tip;
      tip.Set("publisherKey", item->id);
      tip.Set("amount", item->weight);
      recurring_tips.Append(std::move(tip));
    }
  }

  result.Set("recurringTips", std::move(recurring_tips));
  Respond(WithArguments(std::move(result)));
}

HnsRewardsRefreshPublisherFunction::~HnsRewardsRefreshPublisherFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsRefreshPublisherFunction::Run() {
  absl::optional<hns_rewards::RefreshPublisher::Params> params =
      hns_rewards::RefreshPublisher::Params::Create(args());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(WithArguments(false, std::string()));
  }
  rewards_service->RefreshPublisher(
      params->publisher_key,
      base::BindOnce(&HnsRewardsRefreshPublisherFunction::OnRefreshPublisher,
                     this));
  return RespondLater();
}

void HnsRewardsRefreshPublisherFunction::OnRefreshPublisher(
    const ::hns_rewards::mojom::PublisherStatus status,
    const std::string& publisher_key) {
  Respond(WithArguments(static_cast<int>(status), publisher_key));
}

HnsRewardsGetAllNotificationsFunction::
    ~HnsRewardsGetAllNotificationsFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetAllNotificationsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  base::Value::List list;

  if (!rewards_service) {
    return RespondNow(WithArguments(std::move(list)));
  }

  auto notifications = rewards_service->GetAllNotifications();

  for (auto const& notification : notifications) {
    base::Value::Dict item;
    item.Set("id", notification.second.id_);
    item.Set("type", notification.second.type_);
    item.Set("timestamp", static_cast<double>(notification.second.timestamp_));

    base::Value::List args;
    for (auto const& arg : notification.second.args_) {
      args.Append(arg);
    }

    item.Set("args", std::move(args));
    list.Append(std::move(item));
  }

  return RespondNow(WithArguments(std::move(list)));
}

HnsRewardsGetInlineTippingPlatformEnabledFunction::
    ~HnsRewardsGetInlineTippingPlatformEnabledFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetInlineTippingPlatformEnabledFunction::Run() {
  absl::optional<hns_rewards::GetInlineTippingPlatformEnabled::Params>
      params = hns_rewards::GetInlineTippingPlatformEnabled::Params::Create(
          args());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(WithArguments(false));
  }

  rewards_service->GetInlineTippingPlatformEnabled(
      params->key,
      base::BindOnce(&HnsRewardsGetInlineTippingPlatformEnabledFunction::
                         OnInlineTipSetting,
                     this));
  return RespondLater();
}

void HnsRewardsGetInlineTippingPlatformEnabledFunction::OnInlineTipSetting(
    bool value) {
  Respond(WithArguments(value));
}

HnsRewardsFetchBalanceFunction::~HnsRewardsFetchBalanceFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsFetchBalanceFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->FetchBalance(
      base::BindOnce(&HnsRewardsFetchBalanceFunction::OnFetchBalance, this));

  return RespondLater();
}

void HnsRewardsFetchBalanceFunction::OnFetchBalance(
    base::expected<::hns_rewards::mojom::BalancePtr,
                   ::hns_rewards::mojom::FetchBalanceError> result) {
  const auto balance = std::move(result).value_or(nullptr);
  Respond(balance ? WithArguments(balance->total) : NoArguments());
}

HnsRewardsGetExternalWalletProvidersFunction::
    ~HnsRewardsGetExternalWalletProvidersFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetExternalWalletProvidersFunction::Run() {
  base::Value::List providers;

  auto* profile = Profile::FromBrowserContext(browser_context());
  if (auto* rewards_service = RewardsServiceFactory::GetForProfile(profile)) {
    for (auto& provider : rewards_service->GetExternalWalletProviders()) {
      providers.Append(provider);
    }
  }
  return RespondNow(WithArguments(std::move(providers)));
}

HnsRewardsGetExternalWalletFunction::
    ~HnsRewardsGetExternalWalletFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsGetExternalWalletFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->GetExternalWallet(base::BindOnce(
      &HnsRewardsGetExternalWalletFunction::OnGetExternalWallet, this));
  return RespondLater();
}

void HnsRewardsGetExternalWalletFunction::OnGetExternalWallet(
    base::expected<::hns_rewards::mojom::ExternalWalletPtr,
                   ::hns_rewards::mojom::GetExternalWalletError> result) {
  auto wallet = std::move(result).value_or(nullptr);
  if (!wallet) {
    return Respond(NoArguments());
  }

  base::Value::Dict data;
  data.Set("type", wallet->type);
  data.Set("address", wallet->address);
  data.Set("status", static_cast<int>(wallet->status));
  data.Set("userName", wallet->user_name);
  data.Set("accountUrl", wallet->account_url);
  data.Set("loginUrl", wallet->login_url);
  data.Set("activityUrl", wallet->activity_url);

  Respond(WithArguments(std::move(data)));
}

HnsRewardsGetRewardsEnabledFunction::
    ~HnsRewardsGetRewardsEnabledFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsGetRewardsEnabledFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  bool enabled =
      profile->GetPrefs()->GetBoolean(::hns_rewards::prefs::kEnabled);
  return RespondNow(WithArguments(enabled));
}

HnsRewardsGetAdsAccountStatementFunction::
    ~HnsRewardsGetAdsAccountStatementFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetAdsAccountStatementFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);

  if (!ads_service) {
    return RespondNow(Error("Ads service is not initialized"));
  }

  AddRef();  // Balanced in OnGetAdsAccountStatement().

  ads_service->GetStatementOfAccounts(base::BindOnce(
      &HnsRewardsGetAdsAccountStatementFunction::OnGetAdsAccountStatement,
      this));
  return RespondLater();
}

void HnsRewardsGetAdsAccountStatementFunction::OnGetAdsAccountStatement(
    hns_ads::mojom::StatementInfoPtr statement) {
  if (!statement) {
    Respond(WithArguments(false));
  } else {
    base::Value::Dict dict;
    dict.Set("nextPaymentDate",
             statement->next_payment_date.ToDoubleT() * 1000);
    dict.Set("adsReceivedThisMonth", statement->ads_received_this_month);
    dict.Set("minEarningsThisMonth", statement->min_earnings_this_month);
    dict.Set("maxEarningsThisMonth", statement->max_earnings_this_month);
    dict.Set("minEarningsLastMonth", statement->min_earnings_last_month);
    dict.Set("maxEarningsLastMonth", statement->max_earnings_last_month);

    Respond(WithArguments(true, std::move(dict)));
  }

  Release();  // Balanced in Run()
}

HnsRewardsIsInitializedFunction::~HnsRewardsIsInitializedFunction() =
    default;

ExtensionFunction::ResponseAction HnsRewardsIsInitializedFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  return RespondNow(
      WithArguments(rewards_service && rewards_service->IsInitialized()));
}
HnsRewardsGetScheduledCaptchaInfoFunction::
    ~HnsRewardsGetScheduledCaptchaInfoFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsGetScheduledCaptchaInfoFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* hns_adaptive_captcha_service =
      hns_adaptive_captcha::HnsAdaptiveCaptchaServiceFactory::GetForProfile(
          profile);
  if (!hns_adaptive_captcha_service) {
    return RespondNow(
        Error("Adaptive captcha service called from incognito or unsupported "
              "profile"));
  }

  std::string url;
  bool max_attempts_exceeded = false;
  hns_adaptive_captcha_service->GetScheduledCaptchaInfo(
      &url, &max_attempts_exceeded);

  base::Value::Dict dict;
  dict.Set("url", url);
  dict.Set("maxAttemptsExceeded", max_attempts_exceeded);

  return RespondNow(WithArguments(std::move(dict)));
}

HnsRewardsUpdateScheduledCaptchaResultFunction::
    ~HnsRewardsUpdateScheduledCaptchaResultFunction() = default;

ExtensionFunction::ResponseAction
HnsRewardsUpdateScheduledCaptchaResultFunction::Run() {
  auto params =
      hns_rewards::UpdateScheduledCaptchaResult::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* hns_adaptive_captcha_service =
      hns_adaptive_captcha::HnsAdaptiveCaptchaServiceFactory::GetForProfile(
          profile);
  if (!hns_adaptive_captcha_service) {
    return RespondNow(
        Error("Adaptive captcha service called from incognito or unsupported "
              "profile"));
  }

  hns_adaptive_captcha_service->UpdateScheduledCaptchaResult(params->result);

  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (ads_service && params->result) {
    ads_service->NotifyDidSolveAdaptiveCaptcha();
  }

  return RespondNow(NoArguments());
}

HnsRewardsGetPrefsFunction::~HnsRewardsGetPrefsFunction() = default;

ExtensionFunction::ResponseAction HnsRewardsGetPrefsFunction::Run() {
  auto* rewards_service = RewardsServiceFactory::GetForProfile(
      Profile::FromBrowserContext(browser_context()));

  if (!rewards_service)
    return RespondNow(Error("Rewards service is not initialized"));

  rewards_service->GetAutoContributeProperties(base::BindRepeating(
      &HnsRewardsGetPrefsFunction::GetAutoContributePropertiesCallback,
      this));

  return RespondLater();
}

void HnsRewardsGetPrefsFunction::GetAutoContributePropertiesCallback(
    ::hns_rewards::mojom::AutoContributePropertiesPtr properties) {
  base::Value::Dict prefs;
  prefs.Set("autoContributeEnabled",
            properties ? properties->enabled_contribute : false);
  prefs.Set("autoContributeAmount", properties ? properties->amount : 0.0);
  Respond(WithArguments(std::move(prefs)));
}

}  // namespace extensions::api
