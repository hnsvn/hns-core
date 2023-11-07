/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_web_ui_controller_factory.h"

#include <memory>

#include "base/feature_list.h"
#include "base/memory/ptr_util.h"
#include "base/no_destructor.h"
#include "hns/browser/hns_rewards/rewards_util.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/browser/ui/webui/hns_adblock_internals_ui.h"
#include "hns/browser/ui/webui/hns_adblock_ui.h"
#include "hns/browser/ui/webui/hns_rewards_internals_ui.h"
#include "hns/browser/ui/webui/hns_rewards_page_ui.h"
#include "hns/browser/ui/webui/skus_internals_ui.h"
#include "hns/components/hns_federated/features.h"
#include "hns/components/hns_rewards/common/rewards_util.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/playlist/common/buildflags/buildflags.h"
#include "hns/components/skus/common/features.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/url_constants.h"
#include "components/optimization_guide/core/optimization_guide_features.h"
#include "components/optimization_guide/optimization_guide_internals/webui/url_constants.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/url_utils.h"
#include "url/gurl.h"

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/ui/webui/hns_news_internals/hns_news_internals_ui.h"
#include "hns/browser/ui/webui/hns_rewards/rewards_panel_ui.h"
#include "hns/browser/ui/webui/hns_rewards/tip_panel_ui.h"
#include "hns/browser/ui/webui/hns_settings_ui.h"
#include "hns/browser/ui/webui/hns_shields/cookie_list_opt_in_ui.h"
#include "hns/browser/ui/webui/hns_shields/shields_panel_ui.h"
#include "hns/browser/ui/webui/hns_wallet/wallet_page_ui.h"
#include "hns/browser/ui/webui/hns_wallet/wallet_panel_ui.h"
#include "hns/browser/ui/webui/new_tab_page/hns_new_tab_ui.h"
#include "hns/browser/ui/webui/private_new_tab_page/hns_private_new_tab_ui.h"
#include "hns/browser/ui/webui/speedreader/speedreader_toolbar_ui.h"
#include "hns/browser/ui/webui/webcompat_reporter/webcompat_reporter_ui.h"
#include "hns/browser/ui/webui/welcome_page/hns_welcome_ui.h"
#include "hns/components/hns_news/common/features.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/commands/common/features.h"
#endif

#if BUILDFLAG(IS_ANDROID)
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/browser/ui/webui/hns_wallet/android/android_wallet_page_ui.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#endif

#include "hns/browser/hns_vpn/vpn_utils.h"
#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ui/webui/ethereum_remote_client/ethereum_remote_client_ui.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/browser/ipfs/ipfs_service_factory.h"
#include "hns/components/ipfs/features.h"
#include "hns/components/ipfs/ipfs_utils.h"
#if BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
#include "hns/browser/ui/webui/ipfs_ui.h"
#endif  // BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
#endif  // BUILDFLAG(ENABLE_IPFS)

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
#include "hns/browser/ui/webui/playlist_ui.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/browser/ui/webui/tor_internals_ui.h"
#endif

using content::WebUI;
using content::WebUIController;

namespace {

// A function for creating a new WebUI. The caller owns the return value, which
// may be NULL (for example, if the URL refers to an non-existent extension).
typedef WebUIController* (*WebUIFactoryFunction)(WebUI* web_ui,
                                                 const GURL& url);

WebUIController* NewWebUI(WebUI* web_ui, const GURL& url) {
  auto host = url.host_piece();
  Profile* profile = Profile::FromBrowserContext(
      web_ui->GetWebContents()->GetBrowserContext());
  if (host == kAdblockHost) {
    return new HnsAdblockUI(web_ui, url.host());
  } else if (host == kAdblockInternalsHost) {
    return new HnsAdblockInternalsUI(web_ui, url.host());
  } else if (host == kSkusInternalsHost) {
    return new SkusInternalsUI(web_ui, url.host());
#if !BUILDFLAG(IS_ANDROID)
  } else if (host == kWebcompatReporterHost) {
    return new webcompat_reporter::WebcompatReporterUI(web_ui, url.host());
#endif
#if BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
  } else if (host == kIPFSWebUIHost &&
             ipfs::IpfsServiceFactory::IsIpfsEnabled(profile)) {
    return new IPFSUI(web_ui, url.host());
#endif
#if !BUILDFLAG(IS_ANDROID)
  } else if (host == kWalletPageHost &&
             hns_wallet::IsAllowedForContext(profile)) {
    if (hns_wallet::IsNativeWalletEnabled()) {
      auto default_wallet =
          hns_wallet::GetDefaultEthereumWallet(profile->GetPrefs());
      if (default_wallet == hns_wallet::mojom::DefaultWallet::CryptoWallets) {
        return new EthereumRemoteClientUI(web_ui, url.host());
      }
      return new WalletPageUI(web_ui);
    }
#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
    return new EthereumRemoteClientUI(web_ui, url.host());
#endif
  } else if (host == kWalletPanelHost &&
             hns_wallet::IsAllowedForContext(profile)) {
    return new WalletPanelUI(web_ui);
#endif  // BUILDFLAG(OS_ANDROID)
  } else if (host == kRewardsPageHost &&
             // We don't want to check for supported profile type here because
             // we want private windows to redirect to the regular profile.
             // Additionally, if Rewards aren't supported because of the OFAC
             // sanctions we want to show the page with an appropriate error
             // message.
             // Guest session will just show an error page.
             hns_rewards::IsSupported(
                 profile->GetPrefs(),
                 hns_rewards::IsSupportedOptions::kSkipRegionCheck)) {
    return new HnsRewardsPageUI(web_ui, url.host());
  } else if (host == kRewardsInternalsHost &&
             hns_rewards::IsSupportedForProfile(profile)) {
    return new HnsRewardsInternalsUI(web_ui, url.host());
#if !BUILDFLAG(IS_ANDROID)
  } else if (host == kHnsRewardsPanelHost &&
             hns_rewards::IsSupportedForProfile(profile)) {
    return new hns_rewards::RewardsPanelUI(web_ui);
  } else if (host == kHnsTipPanelHost &&
             hns_rewards::IsSupportedForProfile(profile)) {
    return new hns_rewards::TipPanelUI(web_ui);
  } else if (base::FeatureList::IsEnabled(
                 hns_news::features::kHnsNewsFeedUpdate) &&
             host == kHnsNewsInternalsHost) {
    return new HnsNewsInternalsUI(web_ui, url.host());
#endif  // !BUILDFLAG(IS_ANDROID)
#if !BUILDFLAG(IS_ANDROID)
  } else if (host == kWelcomeHost && !profile->IsGuestSession()) {
    return new HnsWelcomeUI(web_ui, url.host());
  } else if (host == chrome::kChromeUISettingsHost) {
    return new HnsSettingsUI(web_ui, url.host());
  } else if (host == chrome::kChromeUINewTabHost) {
    if (profile->IsIncognitoProfile() || profile->IsTor() ||
        profile->IsGuestSession()) {
      return new HnsPrivateNewTabUI(web_ui, url.host());
    }
    return new HnsNewTabUI(web_ui, url.host());
  } else if (host == kShieldsPanelHost) {
    return new ShieldsPanelUI(web_ui);
  } else if (host == kSpeedreaderPanelHost) {
    return new SpeedreaderToolbarUI(web_ui, url.host());
  } else if (host == kCookieListOptInHost) {
    if (base::FeatureList::IsEnabled(
            hns_shields::features::kHnsAdblockCookieListOptIn)) {
      return new CookieListOptInUI(web_ui);
    }
#endif  // !BUILDFLAG(IS_ANDROID)
#if BUILDFLAG(ENABLE_TOR)
  } else if (host == kTorInternalsHost) {
    return new TorInternalsUI(web_ui, url.host());
#endif
#if BUILDFLAG(IS_ANDROID)
  } else if (url.is_valid() && url.host() == kWalletPageHost) {
    return new AndroidWalletPageUI(web_ui, url);
#endif
  }
  return nullptr;
}

// Returns a function that can be used to create the right type of WebUI for a
// tab, based on its URL. Returns NULL if the URL doesn't have WebUI associated
// with it.
WebUIFactoryFunction GetWebUIFactoryFunction(WebUI* web_ui,
                                             Profile* profile,
                                             const GURL& url) {
  // This will get called a lot to check all URLs, so do a quick check of other
  // schemes to filter out most URLs.
  //
  // This has a narrow scoper scope than content::HasWebUIScheme(url) which also
  // allows both `chrome-untrusted` and `chrome-devtools`.
  if (!url.SchemeIs(content::kHnsUIScheme) &&
      !url.SchemeIs(content::kChromeUIScheme)) {
    return nullptr;
  }

  if (url.host_piece() == kAdblockHost ||
      url.host_piece() == kAdblockInternalsHost ||
      url.host_piece() == kWebcompatReporterHost ||
      (url.host_piece() == kSkusInternalsHost &&
       base::FeatureList::IsEnabled(skus::features::kSkusFeature)) ||
#if BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
      (url.host_piece() == kIPFSWebUIHost &&
       ipfs::IpfsServiceFactory::IsIpfsEnabled(profile)) ||
#endif  // BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
#if BUILDFLAG(IS_ANDROID)
      (url.is_valid() && url.host_piece() == kWalletPageHost &&
       (url.path() == kWalletSwapPagePath ||
        url.path() == kWalletSendPagePath || url.path() == kWalletBuyPagePath ||
        url.path() == kWalletDepositPagePath)) ||
#else
      (base::FeatureList::IsEnabled(
           hns_news::features::kHnsNewsFeedUpdate) &&
       url.host_piece() == kHnsNewsInternalsHost) ||
      ((url.host_piece() == kWalletPanelHost ||
        url.host_piece() == kWalletPageHost) &&
       hns_wallet::IsAllowedForContext(profile)) ||
      url.host_piece() == kHnsRewardsPanelHost ||
      url.host_piece() == kHnsTipPanelHost ||
      url.host_piece() == kSpeedreaderPanelHost ||
      // On Android New Tab is a native page implemented in Java, so no need in
      // WebUI.
      url.host_piece() == chrome::kChromeUINewTabHost ||
      url.host_piece() == chrome::kChromeUISettingsHost ||
      ((url.host_piece() == kWelcomeHost ||
        url.host_piece() == chrome::kChromeUIWelcomeURL) &&
       !profile->IsGuestSession()) ||
      url.host_piece() == kShieldsPanelHost ||
      (url.host_piece() == kCookieListOptInHost &&
       base::FeatureList::IsEnabled(
           hns_shields::features::kHnsAdblockCookieListOptIn)) ||
#endif  // BUILDFLAG(IS_ANDROID)
#if BUILDFLAG(ENABLE_TOR)
      url.host_piece() == kTorInternalsHost ||
#endif
      url.host_piece() == kRewardsPageHost ||
      url.host_piece() == kRewardsInternalsHost) {
    return &NewWebUI;
  }

  return nullptr;
}

bool ShouldBlockRewardsWebUI(content::BrowserContext* browser_context,
                             const GURL& url) {
  if (url.host_piece() != kRewardsPageHost &&
#if !BUILDFLAG(IS_ANDROID)
      url.host_piece() != kHnsRewardsPanelHost &&
      url.host_piece() != kHnsTipPanelHost &&
#endif  // !BUILDFLAG(IS_ANDROID)
      url.host_piece() != kRewardsInternalsHost) {
    return false;
  }

  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (profile) {
    if (!hns_rewards::IsSupportedForProfile(
            profile, url.host_piece() == kRewardsPageHost
                         ? hns_rewards::IsSupportedOptions::kSkipRegionCheck
                         : hns_rewards::IsSupportedOptions::kNone)) {
      return true;
    }
#if BUILDFLAG(IS_ANDROID)
    auto* prefs = profile->GetPrefs();
    if (prefs && prefs->GetBoolean(kSafetynetCheckFailed)) {
      return true;
    }
#endif  // BUILDFLAG(IS_ANDROID)
  }
  return false;
}

#if BUILDFLAG(IS_ANDROID)
bool ShouldBlockWalletWebUI(content::BrowserContext* browser_context,
                            const GURL& url) {
  if (!url.is_valid() || url.host() != kWalletPageHost) {
    return false;
  }
  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (!profile) {
    return false;
  }
  auto* keyring_service =
      hns_wallet::KeyringServiceFactory::GetServiceForContext(profile);
  return keyring_service && keyring_service->IsLockedSync();
}
#endif  // BUILDFLAG(IS_ANDROID)
}  // namespace

WebUI::TypeID HnsWebUIControllerFactory::GetWebUIType(
    content::BrowserContext* browser_context,
    const GURL& url) {
  if (ShouldBlockRewardsWebUI(browser_context, url)) {
    return WebUI::kNoWebUI;
  }
#if BUILDFLAG(IS_ANDROID)
  if (ShouldBlockWalletWebUI(browser_context, url)) {
    return WebUI::kNoWebUI;
  }
#endif  // BUILDFLAG(IS_ANDROID)
#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
  if (playlist::PlaylistUI::ShouldBlockPlaylistWebUI(browser_context, url)) {
    return WebUI::kNoWebUI;
  }
#endif

  // Early return to prevent upstream create its WebUI.
  if (url.host_piece() == optimization_guide_internals::
                              kChromeUIOptimizationGuideInternalsHost &&
      !optimization_guide::features::IsOptimizationHintsEnabled()) {
    return WebUI::kNoWebUI;
  }

  Profile* profile = Profile::FromBrowserContext(browser_context);
  WebUIFactoryFunction function =
      GetWebUIFactoryFunction(nullptr, profile, url);
  if (function) {
    return reinterpret_cast<WebUI::TypeID>(function);
  }
  return ChromeWebUIControllerFactory::GetWebUIType(browser_context, url);
}

std::unique_ptr<WebUIController>
HnsWebUIControllerFactory::CreateWebUIControllerForURL(WebUI* web_ui,
                                                         const GURL& url) {
  Profile* profile = Profile::FromWebUI(web_ui);
  WebUIFactoryFunction function = GetWebUIFactoryFunction(web_ui, profile, url);
  if (!function) {
    return ChromeWebUIControllerFactory::CreateWebUIControllerForURL(web_ui,
                                                                     url);
  }

  return base::WrapUnique((*function)(web_ui, url));
}

// static
HnsWebUIControllerFactory* HnsWebUIControllerFactory::GetInstance() {
  static base::NoDestructor<HnsWebUIControllerFactory> instance;
  return instance.get();
}

HnsWebUIControllerFactory::HnsWebUIControllerFactory() = default;

HnsWebUIControllerFactory::~HnsWebUIControllerFactory() = default;
