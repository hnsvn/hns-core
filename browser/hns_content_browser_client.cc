/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_content_browser_client.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "base/command_line.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/strings/strcat.h"
#include "base/system/sys_info.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/hns_browser_main_extra_parts.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/hns_shields/hns_shields_web_contents_observer.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/hns_wallet/hns_wallet_provider_delegate_impl.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"
#include "hns/browser/debounce/debounce_service_factory.h"
#include "hns/browser/ephemeral_storage/ephemeral_storage_service_factory.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/browser/net/hns_proxying_url_loader_factory.h"
#include "hns/browser/net/hns_proxying_web_socket.h"
#include "hns/browser/profiles/hns_renderer_updater.h"
#include "hns/browser/profiles/hns_renderer_updater_factory.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/skus/skus_service_factory.h"
#include "hns/browser/ui/webui/skus_internals_ui.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/hns_federated/features.h"
#include "hns/components/hns_rewards/browser/rewards_protocol_handler.h"
#include "hns/components/hns_search/browser/hns_search_default_host.h"
#include "hns/components/hns_search/browser/hns_search_default_host_private.h"
#include "hns/components/hns_search/browser/hns_search_fallback_host.h"
#include "hns/components/hns_search/common/hns_search_default.mojom.h"
#include "hns/components/hns_search/common/hns_search_fallback.mojom.h"
#include "hns/components/hns_search/common/hns_search_utils.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/hns_shields/browser/hns_farbling_service.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "hns/components/hns_shields/browser/domain_block_navigation_throttle.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/browser/hns_wallet_p3a_private.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/ethereum_provider_impl.h"
#include "hns/components/hns_wallet/browser/solana_provider_impl.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/hns_webtorrent/browser/buildflags/buildflags.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/cosmetic_filters/browser/cosmetic_filters_resources.h"
#include "hns/components/cosmetic_filters/common/cosmetic_filters.mojom.h"
#include "hns/components/de_amp/browser/de_amp_throttle.h"
#include "hns/components/debounce/browser/debounce_navigation_throttle.h"
#include "hns/components/decentralized_dns/content/decentralized_dns_navigation_throttle.h"
#include "hns/components/google_sign_in_permission/google_sign_in_permission_throttle.h"
#include "hns/components/google_sign_in_permission/google_sign_in_permission_util.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/playlist/common/buildflags/buildflags.h"
#include "hns/components/playlist/common/features.h"
#include "hns/components/request_otr/common/buildflags/buildflags.h"
#include "hns/components/skus/common/features.h"
#include "hns/components/skus/common/skus_internals.mojom.h"
#include "hns/components/skus/common/skus_sdk.mojom.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "hns/components/translate/core/common/hns_translate_switches.h"
#include "hns/grit/hns_generated_resources.h"
#include "hns/third_party/blink/renderer/hns_farbling_constants.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_browser_interface_binders.h"
#include "chrome/browser/chrome_content_browser_client.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_io_data.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/common/url_constants.h"
#include "chrome/grit/chromium_strings.h"
#include "components/content_settings/browser/page_specific_content_settings.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/embedder_support/switches.h"
#include "components/prefs/pref_service.h"
#include "components/services/heap_profiling/public/mojom/heap_profiling_client.mojom.h"
#include "components/user_prefs/user_prefs.h"
#include "components/version_info/version_info.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/browser_url_handler.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/browser/weak_document_ptr.h"
#include "content/public/browser/web_ui_browser_interface_broker_registry.h"
#include "content/public/browser/web_ui_controller_interface_binder.h"
#include "content/public/common/content_client.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/url_constants.h"
#include "extensions/buildflags/buildflags.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/self_owned_receiver.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "net/cookies/site_for_cookies.h"
#include "services/service_manager/public/cpp/binder_registry.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"
#include "third_party/blink/public/common/features.h"
#include "third_party/blink/public/common/loader/url_loader_throttle.h"
#include "third_party/blink/public/mojom/webpreferences/web_preferences.mojom.h"
#include "third_party/widevine/cdm/buildflags.h"
#include "ui/base/l10n/l10n_util.h"

#if BUILDFLAG(ENABLE_REQUEST_OTR)
#include "hns/browser/request_otr/request_otr_service_factory.h"
#include "hns/components/request_otr/browser/request_otr_navigation_throttle.h"
#include "hns/components/request_otr/browser/request_otr_storage_tab_helper.h"
#endif

using blink::web_pref::WebPreferences;
using hns_shields::HnsShieldsWebContentsObserver;
using hns_shields::ControlType;
using hns_shields::GetHnsShieldsEnabled;
using hns_shields::GetFingerprintingControlType;
using content::BrowserThread;
using content::ContentBrowserClient;
using content::RenderFrameHost;
using content::WebContents;

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "chrome/browser/extensions/chrome_content_browser_client_extensions_part.h"
#include "extensions/browser/extension_registry.h"
using extensions::ChromeContentBrowserClientExtensionsPart;
#endif

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/browser/ui/webui/ai_chat/ai_chat_ui.h"
#include "hns/components/ai_chat/common/features.h"
#include "hns/components/ai_chat/common/mojom/ai_chat.mojom.h"
#endif

#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
#include "hns/browser/extensions/hns_webtorrent_navigation_throttle.h"
#include "hns/components/hns_webtorrent/browser/content_browser_client_helper.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/browser/ipfs/content_browser_client_helper.h"
#include "hns/browser/ipfs/ipfs_service_factory.h"
#include "hns/browser/ipfs/ipfs_subframe_navigation_throttle.h"
#include "hns/components/ipfs/ipfs_constants.h"
#include "hns/components/ipfs/ipfs_navigation_throttle.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/browser/tor/onion_location_navigation_throttle_delegate.h"
#include "hns/browser/tor/tor_profile_service_factory.h"
#include "hns/components/tor/onion_location_navigation_throttle.h"
#include "hns/components/tor/tor_navigation_throttle.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/browser/speedreader/speedreader_service_factory.h"
#include "hns/browser/speedreader/speedreader_tab_helper.h"
#include "hns/components/speedreader/speedreader_throttle.h"
#include "hns/components/speedreader/speedreader_util.h"
#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/ui/webui/speedreader/speedreader_toolbar_ui.h"
#include "hns/components/speedreader/common/speedreader_toolbar.mojom.h"
#endif
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "hns/browser/hns_drm_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/browser/ui/webui/hns_vpn/vpn_panel_ui.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#endif

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_service.h"
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_service_factory.h"
#endif

#if BUILDFLAG(IS_ANDROID)
#include "hns/browser/ui/webui/hns_wallet/android/android_wallet_page_ui.h"
#endif  // BUILDFLAG(IS_ANDROID)

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/new_tab/new_tab_shows_navigation_throttle.h"
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
#include "hns/components/hns_new_tab_ui/hns_new_tab_page.mojom.h"
#include "hns/components/hns_news/common/hns_news.mojom.h"
#include "hns/components/hns_news/common/features.h"
#include "hns/components/hns_private_new_tab_ui/common/hns_private_new_tab.mojom.h"
#include "hns/components/hns_rewards/common/features.h"
#include "hns/components/hns_rewards/common/mojom/rewards_panel.mojom.h"
#include "hns/components/hns_rewards/common/mojom/tip_panel.mojom.h"
#include "hns/components/hns_shields/common/hns_shields_panel.mojom.h"
#include "hns/components/hns_shields/common/cookie_list_opt_in.mojom.h"
#include "hns/components/commands/common/commands.mojom.h"
#include "hns/components/commands/common/features.h"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
#include "hns/browser/playlist/playlist_service_factory.h"
#include "hns/components/playlist/browser/playlist_render_frame_browser_client.h"
#include "hns/components/playlist/browser/playlist_service.h"
#include "hns/components/playlist/common/mojom/playlist.mojom.h"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
#include "hns/browser/ui/webui/playlist_ui.h"
#endif  // BUILDFLAG(ENABLE_PLAYLIST_WEBUI)

namespace {

bool HandleURLReverseOverrideRewrite(GURL* url,
                                     content::BrowserContext* browser_context) {
  if (HnsContentBrowserClient::HandleURLOverrideRewrite(url,
                                                          browser_context)) {
    return true;
  }

  return false;
}

bool HandleURLRewrite(GURL* url, content::BrowserContext* browser_context) {
  if (HnsContentBrowserClient::HandleURLOverrideRewrite(url,
                                                          browser_context)) {
    return true;
  }

  return false;
}

void BindCosmeticFiltersResourcesOnTaskRunner(
    mojo::PendingReceiver<cosmetic_filters::mojom::CosmeticFiltersResources>
        receiver) {
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<cosmetic_filters::CosmeticFiltersResources>(
          g_hns_browser_process->ad_block_service()),
      std::move(receiver));
}

void BindCosmeticFiltersResources(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<cosmetic_filters::mojom::CosmeticFiltersResources>
        receiver) {
  g_hns_browser_process->ad_block_service()->GetTaskRunner()->PostTask(
      FROM_HERE, base::BindOnce(&BindCosmeticFiltersResourcesOnTaskRunner,
                                std::move(receiver)));
}

#if BUILDFLAG(ENABLE_PLAYLIST)
void BindPlaylistRenderFrameBrowserClient(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<playlist::mojom::PlaylistRenderFrameBrowserClient>
        receiver) {
  auto* playlist_service =
      playlist::PlaylistServiceFactory::GetForBrowserContext(
          frame_host->GetBrowserContext());
  if (!playlist_service) {
    // We don't support playlist on OTR profile.
    return;
  }
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<playlist::PlaylistRenderFrameBrowserClient>(
          frame_host->GetGlobalId(), playlist_service->GetWeakPtr()),
      std::move(receiver));
}
#endif  // BUILDFLAG(ENABLE_PLAYLIST)

void MaybeBindWalletP3A(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<hns_wallet::mojom::HnsWalletP3A> receiver) {
  auto* context = frame_host->GetBrowserContext();
  if (hns_wallet::IsAllowedForContext(frame_host->GetBrowserContext())) {
    hns_wallet::HnsWalletService* wallet_service =
        hns_wallet::HnsWalletServiceFactory::GetServiceForContext(context);
    DCHECK(wallet_service);
    wallet_service->GetHnsWalletP3A()->Bind(std::move(receiver));
  } else {
    // Dummy API to avoid reporting P3A for OTR contexts
    mojo::MakeSelfOwnedReceiver(
        std::make_unique<hns_wallet::HnsWalletP3APrivate>(),
        std::move(receiver));
  }
}

void MaybeBindEthereumProvider(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<hns_wallet::mojom::EthereumProvider> receiver) {
  auto* json_rpc_service =
      hns_wallet::JsonRpcServiceFactory::GetServiceForContext(
          frame_host->GetBrowserContext());

  if (!json_rpc_service) {
    return;
  }

  auto* tx_service = hns_wallet::TxServiceFactory::GetServiceForContext(
      frame_host->GetBrowserContext());
  if (!tx_service) {
    return;
  }

  auto* keyring_service =
      hns_wallet::KeyringServiceFactory::GetServiceForContext(
          frame_host->GetBrowserContext());
  if (!keyring_service) {
    return;
  }

  auto* hns_wallet_service =
      hns_wallet::HnsWalletServiceFactory::GetServiceForContext(
          frame_host->GetBrowserContext());
  if (!hns_wallet_service) {
    return;
  }

  content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(frame_host);
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<hns_wallet::EthereumProviderImpl>(
          HostContentSettingsMapFactory::GetForProfile(
              Profile::FromBrowserContext(frame_host->GetBrowserContext())),
          json_rpc_service, tx_service, keyring_service, hns_wallet_service,
          std::make_unique<hns_wallet::HnsWalletProviderDelegateImpl>(
              web_contents, frame_host),
          user_prefs::UserPrefs::Get(web_contents->GetBrowserContext())),
      std::move(receiver));
}

void MaybeBindSolanaProvider(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<hns_wallet::mojom::SolanaProvider> receiver) {
  auto* keyring_service =
      hns_wallet::KeyringServiceFactory::GetServiceForContext(
          frame_host->GetBrowserContext());
  if (!keyring_service) {
    return;
  }

  auto* hns_wallet_service =
      hns_wallet::HnsWalletServiceFactory::GetServiceForContext(
          frame_host->GetBrowserContext());
  if (!hns_wallet_service) {
    return;
  }

  auto* tx_service = hns_wallet::TxServiceFactory::GetServiceForContext(
      frame_host->GetBrowserContext());
  if (!tx_service) {
    return;
  }

  auto* json_rpc_service =
      hns_wallet::JsonRpcServiceFactory::GetServiceForContext(
          frame_host->GetBrowserContext());
  if (!json_rpc_service) {
    return;
  }

  auto* host_content_settings_map =
      HostContentSettingsMapFactory::GetForProfile(
          frame_host->GetBrowserContext());
  if (!host_content_settings_map) {
    return;
  }

  content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(frame_host);
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<hns_wallet::SolanaProviderImpl>(
          *host_content_settings_map, keyring_service, hns_wallet_service,
          tx_service, json_rpc_service,
          std::make_unique<hns_wallet::HnsWalletProviderDelegateImpl>(
              web_contents, frame_host)),
      std::move(receiver));
}

void BindHnsSearchFallbackHost(
    int process_id,
    mojo::PendingReceiver<hns_search::mojom::HnsSearchFallback> receiver) {
  content::RenderProcessHost* render_process_host =
      content::RenderProcessHost::FromID(process_id);
  if (!render_process_host) {
    return;
  }

  content::BrowserContext* context = render_process_host->GetBrowserContext();
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<hns_search::HnsSearchFallbackHost>(
          context->GetDefaultStoragePartition()
              ->GetURLLoaderFactoryForBrowserProcess()),
      std::move(receiver));
}

void BindHnsSearchDefaultHost(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<hns_search::mojom::HnsSearchDefault> receiver) {
  auto* context = frame_host->GetBrowserContext();
  auto* profile = Profile::FromBrowserContext(context);
  if (hns::IsRegularProfile(profile)) {
    auto* template_url_service =
        TemplateURLServiceFactory::GetForProfile(profile);
    const std::string host = frame_host->GetLastCommittedURL().host();
    mojo::MakeSelfOwnedReceiver(
        std::make_unique<hns_search::HnsSearchDefaultHost>(
            host, template_url_service, profile->GetPrefs()),
        std::move(receiver));
  } else {
    // Dummy API which always returns false for private contexts.
    mojo::MakeSelfOwnedReceiver(
        std::make_unique<hns_search::HnsSearchDefaultHostPrivate>(),
        std::move(receiver));
  }
}

#if BUILDFLAG(ENABLE_HNS_VPN)
void MaybeBindHnsVpnImpl(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<hns_vpn::mojom::ServiceHandler> receiver) {
  auto* context = frame_host->GetBrowserContext();
  hns_vpn::HnsVpnServiceFactory::BindForContext(context,
                                                    std::move(receiver));
}
#endif
void MaybeBindSkusSdkImpl(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<skus::mojom::SkusService> receiver) {
  auto* context = frame_host->GetBrowserContext();
  skus::SkusServiceFactory::BindForContext(context, std::move(receiver));
}

}  // namespace

HnsContentBrowserClient::HnsContentBrowserClient() = default;

HnsContentBrowserClient::~HnsContentBrowserClient() = default;

std::unique_ptr<content::BrowserMainParts>
HnsContentBrowserClient::CreateBrowserMainParts(bool is_integration_test) {
  std::unique_ptr<content::BrowserMainParts> main_parts =
      ChromeContentBrowserClient::CreateBrowserMainParts(is_integration_test);
  ChromeBrowserMainParts* chrome_main_parts =
      static_cast<ChromeBrowserMainParts*>(main_parts.get());
  chrome_main_parts->AddParts(std::make_unique<HnsBrowserMainExtraParts>());
  return main_parts;
}

void HnsContentBrowserClient::BrowserURLHandlerCreated(
    content::BrowserURLHandler* handler) {
#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
  handler->AddHandlerPair(&webtorrent::HandleMagnetURLRewrite,
                          content::BrowserURLHandler::null_handler());
  handler->AddHandlerPair(&webtorrent::HandleTorrentURLRewrite,
                          &webtorrent::HandleTorrentURLReverseRewrite);
#endif
#if BUILDFLAG(ENABLE_IPFS)
  handler->AddHandlerPair(&ipfs::HandleIPFSURLRewrite,
                          &ipfs::HandleIPFSURLReverseRewrite);
#endif
  handler->AddHandlerPair(&HandleURLRewrite, &HandleURLReverseOverrideRewrite);
  ChromeContentBrowserClient::BrowserURLHandlerCreated(handler);
}

void HnsContentBrowserClient::RenderProcessWillLaunch(
    content::RenderProcessHost* host) {
  Profile* profile = Profile::FromBrowserContext(host->GetBrowserContext());
  HnsRendererUpdaterFactory::GetForProfile(profile)->InitializeRenderer(host);

  ChromeContentBrowserClient::RenderProcessWillLaunch(host);
}

void HnsContentBrowserClient::
    RegisterAssociatedInterfaceBindersForRenderFrameHost(
        content::RenderFrameHost& render_frame_host,                // NOLINT
        blink::AssociatedInterfaceRegistry& associated_registry) {  // NOLINT
#if BUILDFLAG(ENABLE_WIDEVINE)
  associated_registry.AddInterface<
      hns_drm::mojom::HnsDRM>(base::BindRepeating(
      [](content::RenderFrameHost* render_frame_host,
         mojo::PendingAssociatedReceiver<hns_drm::mojom::HnsDRM> receiver) {
        HnsDrmTabHelper::BindHnsDRM(std::move(receiver), render_frame_host);
      },
      &render_frame_host));
#endif  // BUILDFLAG(ENABLE_WIDEVINE)

  associated_registry.AddInterface<
      hns_shields::mojom::HnsShieldsHost>(base::BindRepeating(
      [](content::RenderFrameHost* render_frame_host,
         mojo::PendingAssociatedReceiver<hns_shields::mojom::HnsShieldsHost>
             receiver) {
        hns_shields::HnsShieldsWebContentsObserver::BindHnsShieldsHost(
            std::move(receiver), render_frame_host);
      },
      &render_frame_host));

#if BUILDFLAG(ENABLE_SPEEDREADER)
  associated_registry.AddInterface<speedreader::mojom::SpeedreaderHost>(
      base::BindRepeating(
          [](content::RenderFrameHost* render_frame_host,
             mojo::PendingAssociatedReceiver<
                 speedreader::mojom::SpeedreaderHost> receiver) {
            speedreader::SpeedreaderTabHelper::BindSpeedreaderHost(
                std::move(receiver), render_frame_host);
          },
          &render_frame_host));
#endif

  ChromeContentBrowserClient::
      RegisterAssociatedInterfaceBindersForRenderFrameHost(render_frame_host,
                                                           associated_registry);
}

void HnsContentBrowserClient::RegisterWebUIInterfaceBrokers(
    content::WebUIBrowserInterfaceBrokerRegistry& registry) {
  ChromeContentBrowserClient::RegisterWebUIInterfaceBrokers(registry);
#if BUILDFLAG(ENABLE_HNS_VPN) && !BUILDFLAG(IS_ANDROID)
  if (hns_vpn::IsHnsVPNFeatureEnabled()) {
    registry.ForWebUI<VPNPanelUI>()
        .Add<hns_vpn::mojom::PanelHandlerFactory>();
  }
#endif

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
  if (base::FeatureList::IsEnabled(playlist::features::kPlaylist)) {
    registry.ForWebUI<playlist::PlaylistUI>()
        .Add<playlist::mojom::PageHandlerFactory>();
  }
#endif

#if BUILDFLAG(ENABLE_AI_CHAT)
  if (ai_chat::features::IsAIChatEnabled()) {
    registry.ForWebUI<AIChatUI>().Add<ai_chat::mojom::PageHandler>();
  }
#endif

  if (base::FeatureList::IsEnabled(skus::features::kSkusFeature)) {
    registry.ForWebUI<SkusInternalsUI>().Add<skus::mojom::SkusInternals>();
  }

#if !BUILDFLAG(IS_ANDROID)
  if (base::FeatureList::IsEnabled(
          hns_news::features::kHnsNewsFeedUpdate)) {
    registry.ForWebUI<HnsNewsInternalsUI>()
        .Add<hns_news::mojom::HnsNewsController>();
  }
#endif
}

bool HnsContentBrowserClient::AllowWorkerFingerprinting(
    const GURL& url,
    content::BrowserContext* browser_context) {
  return WorkerGetHnsFarblingLevel(url, browser_context) !=
         HnsFarblingLevel::MAXIMUM;
}

uint8_t HnsContentBrowserClient::WorkerGetHnsFarblingLevel(
    const GURL& url,
    content::BrowserContext* browser_context) {
  HostContentSettingsMap* host_content_settings_map =
      HostContentSettingsMapFactory::GetForProfile(browser_context);
  const bool shields_up =
      hns_shields::GetHnsShieldsEnabled(host_content_settings_map, url);
  if (!shields_up) {
    return HnsFarblingLevel::OFF;
  }
  auto fingerprinting_type = hns_shields::GetFingerprintingControlType(
      host_content_settings_map, url);
  if (fingerprinting_type == ControlType::BLOCK) {
    return HnsFarblingLevel::MAXIMUM;
  }
  if (fingerprinting_type == ControlType::ALLOW) {
    return HnsFarblingLevel::OFF;
  }
  return HnsFarblingLevel::BALANCED;
}

content::ContentBrowserClient::AllowWebBluetoothResult
HnsContentBrowserClient::AllowWebBluetooth(
    content::BrowserContext* browser_context,
    const url::Origin& requesting_origin,
    const url::Origin& embedding_origin) {
  if (!base::FeatureList::IsEnabled(blink::features::kHnsWebBluetoothAPI)) {
    return ContentBrowserClient::AllowWebBluetoothResult::
        BLOCK_GLOBALLY_DISABLED;
  }
  return ChromeContentBrowserClient::AllowWebBluetooth(
      browser_context, requesting_origin, embedding_origin);
}

bool HnsContentBrowserClient::CanCreateWindow(
    content::RenderFrameHost* opener,
    const GURL& opener_url,
    const GURL& opener_top_level_frame_url,
    const url::Origin& source_origin,
    content::mojom::WindowContainerType container_type,
    const GURL& target_url,
    const content::Referrer& referrer,
    const std::string& frame_name,
    WindowOpenDisposition disposition,
    const blink::mojom::WindowFeatures& features,
    bool user_gesture,
    bool opener_suppressed,
    bool* no_javascript_access) {
  // Check base implementation first
  bool can_create_window = ChromeContentBrowserClient::CanCreateWindow(
      opener, opener_url, opener_top_level_frame_url, source_origin,
      container_type, target_url, referrer, frame_name, disposition, features,
      user_gesture, opener_suppressed, no_javascript_access);

#if BUILDFLAG(ENABLE_REQUEST_OTR)
  // If the user has requested going off-the-record in this tab, don't allow
  // opening new windows via script
  if (content::WebContents* web_contents =
          content::WebContents::FromRenderFrameHost(opener)) {
    if (request_otr::
            RequestOTRStorageTabHelper* request_otr_storage_tab_helper =
                request_otr::RequestOTRStorageTabHelper::FromWebContents(
                    web_contents)) {
      if (request_otr_storage_tab_helper->has_requested_otr()) {
        *no_javascript_access = true;
      }
    }
  }
#endif

  return can_create_window && google_sign_in_permission::CanCreateWindow(
                                  opener, opener_url, target_url);
}

void HnsContentBrowserClient::ExposeInterfacesToRenderer(
    service_manager::BinderRegistry* registry,
    blink::AssociatedInterfaceRegistry* associated_registry,
    content::RenderProcessHost* render_process_host) {
  ChromeContentBrowserClient::ExposeInterfacesToRenderer(
      registry, associated_registry, render_process_host);
  registry->AddInterface(base::BindRepeating(&BindHnsSearchFallbackHost,
                                             render_process_host->GetID()),
                         content::GetUIThreadTaskRunner({}));
}

void HnsContentBrowserClient::RegisterBrowserInterfaceBindersForFrame(
    content::RenderFrameHost* render_frame_host,
    mojo::BinderMapWithContext<content::RenderFrameHost*>* map) {
  ChromeContentBrowserClient::RegisterBrowserInterfaceBindersForFrame(
      render_frame_host, map);
  map->Add<cosmetic_filters::mojom::CosmeticFiltersResources>(
      base::BindRepeating(&BindCosmeticFiltersResources));
  if (hns_search::IsDefaultAPIEnabled()) {
    map->Add<hns_search::mojom::HnsSearchDefault>(
        base::BindRepeating(&BindHnsSearchDefaultHost));
  }

  map->Add<hns_wallet::mojom::HnsWalletP3A>(
      base::BindRepeating(&MaybeBindWalletP3A));
  if (hns_wallet::IsAllowedForContext(
          render_frame_host->GetBrowserContext())) {
    if (hns_wallet::IsNativeWalletEnabled()) {
      map->Add<hns_wallet::mojom::EthereumProvider>(
          base::BindRepeating(&MaybeBindEthereumProvider));
      map->Add<hns_wallet::mojom::SolanaProvider>(
          base::BindRepeating(&MaybeBindSolanaProvider));
    }
  }

  map->Add<skus::mojom::SkusService>(
      base::BindRepeating(&MaybeBindSkusSdkImpl));
#if BUILDFLAG(ENABLE_HNS_VPN)
  map->Add<hns_vpn::mojom::ServiceHandler>(
      base::BindRepeating(&MaybeBindHnsVpnImpl));
#endif

#if BUILDFLAG(IS_ANDROID)
  content::RegisterWebUIControllerInterfaceBinder<
      hns_wallet::mojom::PageHandlerFactory, AndroidWalletPageUI>(map);
#endif

#if !BUILDFLAG(IS_ANDROID)
  content::RegisterWebUIControllerInterfaceBinder<
      hns_wallet::mojom::PageHandlerFactory, WalletPageUI>(map);
  content::RegisterWebUIControllerInterfaceBinder<
      hns_wallet::mojom::PanelHandlerFactory, WalletPanelUI>(map);
  content::RegisterWebUIControllerInterfaceBinder<
      hns_private_new_tab::mojom::PageHandler, HnsPrivateNewTabUI>(map);
  content::RegisterWebUIControllerInterfaceBinder<
      hns_shields::mojom::PanelHandlerFactory, ShieldsPanelUI>(map);
  if (base::FeatureList::IsEnabled(
          hns_shields::features::kHnsAdblockCookieListOptIn)) {
    content::RegisterWebUIControllerInterfaceBinder<
        hns_shields::mojom::CookieListOptInPageHandlerFactory,
        CookieListOptInUI>(map);
  }
  content::RegisterWebUIControllerInterfaceBinder<
      hns_rewards::mojom::PanelHandlerFactory, hns_rewards::RewardsPanelUI>(
      map);
  content::RegisterWebUIControllerInterfaceBinder<
      hns_rewards::mojom::TipPanelHandlerFactory, hns_rewards::TipPanelUI>(
      map);
  if (base::FeatureList::IsEnabled(commands::features::kHnsCommands)) {
    content::RegisterWebUIControllerInterfaceBinder<
        commands::mojom::CommandsService, HnsSettingsUI>(map);
  }
#endif

#if BUILDFLAG(ENABLE_AI_CHAT) && !BUILDFLAG(IS_ANDROID)
  if (ai_chat::features::IsAIChatEnabled() &&
      !render_frame_host->GetBrowserContext()->IsTor()) {
    content::RegisterWebUIControllerInterfaceBinder<ai_chat::mojom::PageHandler,
                                                    AIChatUI>(map);
  }
#endif

// Hns News
#if !BUILDFLAG(IS_ANDROID)
  content::RegisterWebUIControllerInterfaceBinder<
      hns_news::mojom::HnsNewsController, HnsNewTabUI>(map);
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER) && !BUILDFLAG(IS_ANDROID)
  content::RegisterWebUIControllerInterfaceBinder<
      speedreader::mojom::ToolbarFactory, SpeedreaderToolbarUI>(map);
#endif

#if !BUILDFLAG(IS_ANDROID)
  content::RegisterWebUIControllerInterfaceBinder<
      hns_new_tab_page::mojom::PageHandlerFactory, HnsNewTabUI>(map);
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
  map->Add<playlist::mojom::PlaylistRenderFrameBrowserClient>(
      base::BindRepeating(&BindPlaylistRenderFrameBrowserClient));
#endif  // BUILDFLAG(ENABLE_PLAYLIST)
}

bool HnsContentBrowserClient::HandleExternalProtocol(
    const GURL& url,
    content::WebContents::Getter web_contents_getter,
    int frame_tree_node_id,
    content::NavigationUIData* navigation_data,
    bool is_primary_main_frame,
    bool is_in_fenced_frame_tree,
    network::mojom::WebSandboxFlags sandbox_flags,
    ui::PageTransition page_transition,
    bool has_user_gesture,
    const absl::optional<url::Origin>& initiating_origin,
    content::RenderFrameHost* initiator_document,
    mojo::PendingRemote<network::mojom::URLLoaderFactory>* out_factory) {
#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
  if (webtorrent::IsMagnetProtocol(url)) {
    auto weak_initiator_document =
        initiator_document ? initiator_document->GetWeakDocumentPtr()
                           : content::WeakDocumentPtr();

    webtorrent::HandleMagnetProtocol(url, web_contents_getter, page_transition,
                                     has_user_gesture, is_in_fenced_frame_tree,
                                     initiating_origin,
                                     std::move(weak_initiator_document));
    return true;
  }
#endif

  if (hns_rewards::IsRewardsProtocol(url)) {
    hns_rewards::HandleRewardsProtocol(url, web_contents_getter,
                                         page_transition);
    return true;
  }

  return ChromeContentBrowserClient::HandleExternalProtocol(
      url, web_contents_getter, frame_tree_node_id, navigation_data,
      is_primary_main_frame, is_in_fenced_frame_tree, sandbox_flags,
      page_transition, has_user_gesture, initiating_origin, initiator_document,
      out_factory);
}

void HnsContentBrowserClient::AppendExtraCommandLineSwitches(
    base::CommandLine* command_line,
    int child_process_id) {
  ChromeContentBrowserClient::AppendExtraCommandLineSwitches(command_line,
                                                             child_process_id);
  std::string process_type =
      command_line->GetSwitchValueASCII(switches::kProcessType);
  if (process_type == switches::kRendererProcess) {
    uint64_t session_token =
        12345;  // the kinda thing an idiot would have on his luggage

    // Command line parameters from the browser process are propagated to the
    // renderers *after* ContentBrowserClient::AppendExtraCommandLineSwitches()
    // is called from RenderProcessHostImpl::AppendRendererCommandLine(). This
    // means we have to inspect the main browser process' parameters for the
    // |switches::kTestType| as it will be too soon to find it on command_line.
    const base::CommandLine& browser_command_line =
        *base::CommandLine::ForCurrentProcess();
    if (!browser_command_line.HasSwitch(switches::kTestType)) {
      content::RenderProcessHost* process =
          content::RenderProcessHost::FromID(child_process_id);
      Profile* profile =
          process ? Profile::FromBrowserContext(process->GetBrowserContext())
                  : nullptr;
      session_token =
          g_hns_browser_process->hns_farbling_service()->session_token(
              profile && !profile->IsOffTheRecord());
    }
    command_line->AppendSwitchASCII("hns_session_token",
                                    base::NumberToString(session_token));

    // Switches to pass to render processes.
    static const char* const kSwitchNames[] = {
        translate::switches::kHnsTranslateUseGoogleEndpoint,
    };
    command_line->CopySwitchesFrom(browser_command_line, kSwitchNames);
  }
}

std::vector<std::unique_ptr<blink::URLLoaderThrottle>>
HnsContentBrowserClient::CreateURLLoaderThrottles(
    const network::ResourceRequest& request,
    content::BrowserContext* browser_context,
    const content::WebContents::Getter& wc_getter,
    content::NavigationUIData* navigation_ui_data,
    int frame_tree_node_id) {
  auto result = ChromeContentBrowserClient::CreateURLLoaderThrottles(
      request, browser_context, wc_getter, navigation_ui_data,
      frame_tree_node_id);
  content::WebContents* contents = wc_getter.Run();

  if (contents) {
    const bool isMainFrame =
        request.resource_type ==
        static_cast<int>(blink::mojom::ResourceType::kMainFrame);

    // Speedreader
#if BUILDFLAG(ENABLE_SPEEDREADER)
    auto* tab_helper =
        speedreader::SpeedreaderTabHelper::FromWebContents(contents);
    if (tab_helper && isMainFrame) {
      auto* speedreader_service =
          speedreader::SpeedreaderServiceFactory::GetForBrowserContext(
              browser_context);
      std::unique_ptr<speedreader::SpeedReaderThrottle> throttle =
          speedreader::SpeedReaderThrottle::MaybeCreateThrottleFor(
              g_hns_browser_process->speedreader_rewriter_service(),
              speedreader_service, tab_helper->GetWeakPtr(), request.url,
              base::SingleThreadTaskRunner::GetCurrentDefault());
      if (throttle) {
        result.push_back(std::move(throttle));
      }
    }
#endif  // ENABLE_SPEEDREADER

    if (isMainFrame) {
      // De-AMP
      if (auto de_amp_throttle = de_amp::DeAmpThrottle::MaybeCreateThrottleFor(
              base::SingleThreadTaskRunner::GetCurrentDefault(), request,
              wc_getter)) {
        result.push_back(std::move(de_amp_throttle));
      }
    }

    if (auto google_sign_in_permission_throttle =
            google_sign_in_permission::GoogleSignInPermissionThrottle::
                MaybeCreateThrottleFor(request, wc_getter)) {
      result.push_back(std::move(google_sign_in_permission_throttle));
    }
  }

  return result;
}

bool HnsContentBrowserClient::WillCreateURLLoaderFactory(
    content::BrowserContext* browser_context,
    content::RenderFrameHost* frame,
    int render_process_id,
    URLLoaderFactoryType type,
    const url::Origin& request_initiator,
    absl::optional<int64_t> navigation_id,
    ukm::SourceIdObj ukm_source_id,
    mojo::PendingReceiver<network::mojom::URLLoaderFactory>* factory_receiver,
    mojo::PendingRemote<network::mojom::TrustedURLLoaderHeaderClient>*
        header_client,
    bool* bypass_redirect_checks,
    bool* disable_secure_dns,
    network::mojom::URLLoaderFactoryOverridePtr* factory_override,
    scoped_refptr<base::SequencedTaskRunner> navigation_response_task_runner) {
  bool use_proxy = false;
  // TODO(iefremov): Skip proxying for certain requests?
  use_proxy = HnsProxyingURLLoaderFactory::MaybeProxyRequest(
      browser_context, frame,
      type == URLLoaderFactoryType::kNavigation ? -1 : render_process_id,
      factory_receiver, navigation_response_task_runner);

  use_proxy |= ChromeContentBrowserClient::WillCreateURLLoaderFactory(
      browser_context, frame, render_process_id, type, request_initiator,
      std::move(navigation_id), ukm_source_id, factory_receiver, header_client,
      bypass_redirect_checks, disable_secure_dns, factory_override,
      navigation_response_task_runner);

  return use_proxy;
}

bool HnsContentBrowserClient::WillInterceptWebSocket(
    content::RenderFrameHost* frame) {
  return (frame != nullptr);
}

void HnsContentBrowserClient::CreateWebSocket(
    content::RenderFrameHost* frame,
    content::ContentBrowserClient::WebSocketFactory factory,
    const GURL& url,
    const net::SiteForCookies& site_for_cookies,
    const absl::optional<std::string>& user_agent,
    mojo::PendingRemote<network::mojom::WebSocketHandshakeClient>
        handshake_client) {
  auto* proxy = HnsProxyingWebSocket::ProxyWebSocket(
      frame, std::move(factory), url, site_for_cookies, user_agent,
      std::move(handshake_client));

  if (ChromeContentBrowserClient::WillInterceptWebSocket(frame)) {
    ChromeContentBrowserClient::CreateWebSocket(
        frame, proxy->web_socket_factory(), url, site_for_cookies, user_agent,
        proxy->handshake_client().Unbind());
  } else {
    proxy->Start();
  }
}

void HnsContentBrowserClient::MaybeHideReferrer(
    content::BrowserContext* browser_context,
    const GURL& request_url,
    const GURL& document_url,
    blink::mojom::ReferrerPtr* referrer) {
  DCHECK(referrer && !referrer->is_null());
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (document_url.SchemeIs(kChromeExtensionScheme) ||
      request_url.SchemeIs(kChromeExtensionScheme)) {
    return;
  }
#endif
  if (document_url.SchemeIs(content::kChromeUIScheme) ||
      request_url.SchemeIs(content::kChromeUIScheme)) {
    return;
  }

  Profile* profile = Profile::FromBrowserContext(browser_context);
  const bool allow_referrers = hns_shields::AreReferrersAllowed(
      HostContentSettingsMapFactory::GetForProfile(profile), document_url);
  const bool shields_up = hns_shields::GetHnsShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile), document_url);

  content::Referrer new_referrer;
  if (hns_shields::MaybeChangeReferrer(allow_referrers, shields_up,
                                         (*referrer)->url, request_url,
                                         &new_referrer)) {
    (*referrer)->url = new_referrer.url;
    (*referrer)->policy = new_referrer.policy;
  }
}

GURL HnsContentBrowserClient::GetEffectiveURL(
    content::BrowserContext* browser_context,
    const GURL& url) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (!profile) {
    return url;
  }

#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (extensions::ChromeContentBrowserClientExtensionsPart::
          AreExtensionsDisabledForProfile(profile)) {
    return url;
  }
  return ChromeContentBrowserClientExtensionsPart::GetEffectiveURL(profile,
                                                                   url);
#else
  return url;
#endif
}

// [static]
bool HnsContentBrowserClient::HandleURLOverrideRewrite(
    GURL* url,
    content::BrowserContext* browser_context) {
  // Some of these rewrites are for WebUI pages with URL that has moved.
  // After rewrite happens, GetWebUIFactoryFunction() will work as expected.
  // (see browser\ui\webui\hns_web_ui_controller_factory.cc for more info)
  //
  // Scope of schema is intentionally narrower than content::HasWebUIScheme(url)
  // which also allows both `chrome-untrusted` and `chrome-devtools`.
  if (!url->SchemeIs(content::kHnsUIScheme) &&
      !url->SchemeIs(content::kChromeUIScheme)) {
    return false;
  }

  // hns://sync => hns://settings/hnsSync
  if (url->host() == chrome::kChromeUISyncHost) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kChromeUIScheme);
    replacements.SetHostStr(chrome::kChromeUISettingsHost);
    replacements.SetPathStr(kHnsSyncPath);
    *url = url->ReplaceComponents(replacements);
    return true;
  }

#if !BUILDFLAG(IS_ANDROID)
  // hns://adblock => hns://settings/shields/filters
  if (url->host() == kAdblockHost) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kChromeUIScheme);
    replacements.SetHostStr(chrome::kChromeUISettingsHost);
    replacements.SetPathStr(kContentFiltersPath);
    *url = url->ReplaceComponents(replacements);
    return false;
  }
#endif

  // no special win10 welcome page
  if (url->host() == chrome::kChromeUIWelcomeHost) {
    *url = GURL(chrome::kChromeUIWelcomeURL);
    return true;
  }

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED) && BUILDFLAG(ENABLE_EXTENSIONS)
  auto* prefs = user_prefs::UserPrefs::Get(browser_context);
  hns_wallet::mojom::DefaultWallet default_wallet =
      hns_wallet::GetDefaultEthereumWallet(prefs);
  if (!hns_wallet::IsNativeWalletEnabled() ||
      default_wallet == hns_wallet::mojom::DefaultWallet::CryptoWallets) {
    // If the Crypto Wallets extension is loaded, then it replaces the WebUI
    auto* service =
        EthereumRemoteClientServiceFactory::GetForContext(browser_context);
    if (service->IsCryptoWalletsReady() &&
        url->SchemeIs(content::kChromeUIScheme) &&
        url->host() == ethereum_remote_client_host) {
      auto* registry = extensions::ExtensionRegistry::Get(browser_context);
      if (registry && registry->ready_extensions().GetByID(
                          ethereum_remote_client_extension_id)) {
        *url = GURL(ethereum_remote_client_base_url);
        return true;
      }
    }
  }
#endif

  return false;
}

std::vector<std::unique_ptr<content::NavigationThrottle>>
HnsContentBrowserClient::CreateThrottlesForNavigation(
    content::NavigationHandle* handle) {
  std::vector<std::unique_ptr<content::NavigationThrottle>> throttles =
      ChromeContentBrowserClient::CreateThrottlesForNavigation(handle);

#if !BUILDFLAG(IS_ANDROID)
  std::unique_ptr<content::NavigationThrottle> ntp_shows_navigation_throttle =
      NewTabShowsNavigationThrottle::MaybeCreateThrottleFor(handle);
  if (ntp_shows_navigation_throttle) {
    throttles.push_back(std::move(ntp_shows_navigation_throttle));
  }
#endif

#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
  throttles.push_back(
      std::make_unique<extensions::HnsWebTorrentNavigationThrottle>(handle));
#endif

  content::BrowserContext* context =
      handle->GetWebContents()->GetBrowserContext();

#if BUILDFLAG(ENABLE_TOR)
  std::unique_ptr<content::NavigationThrottle> tor_navigation_throttle =
      tor::TorNavigationThrottle::MaybeCreateThrottleFor(handle,
                                                         context->IsTor());
  if (tor_navigation_throttle) {
    throttles.push_back(std::move(tor_navigation_throttle));
  }
  std::unique_ptr<tor::OnionLocationNavigationThrottleDelegate>
      onion_location_navigation_throttle_delegate =
          std::make_unique<tor::OnionLocationNavigationThrottleDelegate>();
  std::unique_ptr<content::NavigationThrottle>
      onion_location_navigation_throttle =
          tor::OnionLocationNavigationThrottle::MaybeCreateThrottleFor(
              handle, TorProfileServiceFactory::IsTorDisabled(context),
              std::move(onion_location_navigation_throttle_delegate),
              context->IsTor());
  if (onion_location_navigation_throttle) {
    throttles.push_back(std::move(onion_location_navigation_throttle));
  }
#endif

#if BUILDFLAG(ENABLE_IPFS)
  throttles.insert(
      throttles.begin(),
      ipfs::IpfsSubframeNavigationThrottle::CreateThrottleFor(handle));
  std::unique_ptr<content::NavigationThrottle> ipfs_navigation_throttle =
      ipfs::IpfsNavigationThrottle::MaybeCreateThrottleFor(
          handle, ipfs::IpfsServiceFactory::GetForContext(context),
          user_prefs::UserPrefs::Get(context),
          g_browser_process->GetApplicationLocale());
  if (ipfs_navigation_throttle) {
    throttles.push_back(std::move(ipfs_navigation_throttle));
  }
#endif

  std::unique_ptr<content::NavigationThrottle>
      decentralized_dns_navigation_throttle =
          decentralized_dns::DecentralizedDnsNavigationThrottle::
              MaybeCreateThrottleFor(handle, g_browser_process->local_state(),
                                     g_browser_process->GetApplicationLocale());
  if (decentralized_dns_navigation_throttle) {
    throttles.push_back(std::move(decentralized_dns_navigation_throttle));
  }

  // Debounce
  if (auto debounce_throttle =
          debounce::DebounceNavigationThrottle::MaybeCreateThrottleFor(
              handle, debounce::DebounceServiceFactory::GetForBrowserContext(
                          context))) {
    throttles.push_back(std::move(debounce_throttle));
  }

  // The HostContentSettingsMap might be null for some irregular profiles, e.g.
  // the System Profile.
  auto* host_content_settings_map =
      HostContentSettingsMapFactory::GetForProfile(context);
  if (host_content_settings_map) {
    if (std::unique_ptr<content::NavigationThrottle>
            domain_block_navigation_throttle = hns_shields::
                DomainBlockNavigationThrottle::MaybeCreateThrottleFor(
                    handle, g_hns_browser_process->ad_block_service(),
                    g_hns_browser_process->ad_block_service()
                        ->custom_filters_provider(),
                    EphemeralStorageServiceFactory::GetForContext(context),
                    host_content_settings_map,
                    g_browser_process->GetApplicationLocale())) {
      throttles.push_back(std::move(domain_block_navigation_throttle));
    }
  }

#if BUILDFLAG(ENABLE_REQUEST_OTR)
  // Request Off-The-Record
  if (auto request_otr_throttle =
          request_otr::RequestOTRNavigationThrottle::MaybeCreateThrottleFor(
              handle,
              request_otr::RequestOTRServiceFactory::GetForBrowserContext(
                  context),
              EphemeralStorageServiceFactory::GetForContext(context),
              Profile::FromBrowserContext(context)->GetPrefs(),
              g_browser_process->GetApplicationLocale())) {
    throttles.push_back(std::move(request_otr_throttle));
  }
#endif

  return throttles;
}

bool PreventDarkModeFingerprinting(WebContents* web_contents,
                                   WebPreferences* prefs) {
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  // The HostContentSettingsMap might be null for some irregular profiles, e.g.
  // the System Profile.
  auto* host_content_settings_map =
      HostContentSettingsMapFactory::GetForProfile(profile);
  if (!host_content_settings_map) {
    return false;
  }
  const GURL url = web_contents->GetLastCommittedURL();
  const bool shields_up =
      hns_shields::GetHnsShieldsEnabled(host_content_settings_map, url);
  auto fingerprinting_type = hns_shields::GetFingerprintingControlType(
      host_content_settings_map, url);
  // https://github.com/hnsvn/hns-browser/issues/15265
  // Always use color scheme Light if fingerprinting mode strict
  if (base::FeatureList::IsEnabled(
          hns_shields::features::kHnsDarkModeBlock) &&
      shields_up && fingerprinting_type == ControlType::BLOCK &&
      prefs->preferred_color_scheme !=
          blink::mojom::PreferredColorScheme::kLight) {
    prefs->preferred_color_scheme = blink::mojom::PreferredColorScheme::kLight;
    return true;
  }
  return false;
}

bool HnsContentBrowserClient::OverrideWebPreferencesAfterNavigation(
    WebContents* web_contents,
    WebPreferences* prefs) {
  bool changed =
      ChromeContentBrowserClient::OverrideWebPreferencesAfterNavigation(
          web_contents, prefs);
  return PreventDarkModeFingerprinting(web_contents, prefs) || changed;
}

void HnsContentBrowserClient::OverrideWebkitPrefs(WebContents* web_contents,
                                                    WebPreferences* web_prefs) {
  ChromeContentBrowserClient::OverrideWebkitPrefs(web_contents, web_prefs);
  PreventDarkModeFingerprinting(web_contents, web_prefs);
  // This will stop NavigatorPlugins from returning fixed plugins data and will
  // allow us to return our farbled data
  web_prefs->allow_non_empty_navigator_plugins = true;

#if BUILDFLAG(ENABLE_PLAYLIST)
  if (auto* playlist_service =
          playlist::PlaylistServiceFactory::GetForBrowserContext(
              web_contents->GetBrowserContext())) {
    playlist_service->ConfigureWebPrefsForBackgroundWebContents(web_contents,
                                                                web_prefs);
  }
#endif
}

blink::UserAgentMetadata HnsContentBrowserClient::GetUserAgentMetadata() {
  blink::UserAgentMetadata metadata =
      ChromeContentBrowserClient::GetUserAgentMetadata();
  // Expect the brand version lists to have brand version, chromium_version, and
  // greased version.
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(embedder_support::kUserAgent)) {
    return metadata;
  }
  DCHECK_EQ(3UL, metadata.brand_version_list.size());
  DCHECK_EQ(3UL, metadata.brand_full_version_list.size());
  // Zero out the last 3 version components in full version list versions.
  for (auto& brand_version : metadata.brand_full_version_list) {
    base::Version version(brand_version.version);
    brand_version.version =
        base::StrCat({base::NumberToString(version.components()[0]), ".0.0.0"});
  }
  // Zero out the last 3 version components in full version.
  base::Version version(metadata.full_version);
  metadata.full_version =
      base::StrCat({base::NumberToString(version.components()[0]), ".0.0.0"});
  return metadata;
}
