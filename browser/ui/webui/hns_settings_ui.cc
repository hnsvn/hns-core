/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_settings_ui.h"

#include <memory>
#include <string>
#include <utility>

#include "base/feature_list.h"
#include "hns/browser/hns_rewards/rewards_util.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/ntp_background/view_counter_service_factory.h"
#include "hns/browser/resources/settings/grit/hns_settings_resources.h"
#include "hns/browser/resources/settings/grit/hns_settings_resources_map.h"
#include "hns/browser/resources/settings/shortcuts_page/grit/commands_generated_map.h"
#include "hns/browser/shell_integrations/buildflags/buildflags.h"
#include "hns/browser/ui/commands/accelerator_service_factory.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/webui/navigation_bar_data_provider.h"
#include "hns/browser/ui/webui/settings/hns_adblock_handler.h"
#include "hns/browser/ui/webui/settings/hns_appearance_handler.h"
#include "hns/browser/ui/webui/settings/hns_default_extensions_handler.h"
#include "hns/browser/ui/webui/settings/hns_privacy_handler.h"
#include "hns/browser/ui/webui/settings/hns_sync_handler.h"
#include "hns/browser/ui/webui/settings/hns_wallet_handler.h"
#include "hns/browser/ui/webui/settings/default_hns_shields_handler.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/common/features.h"
#include "hns/components/commands/common/commands.mojom.h"
#include "hns/components/commands/common/features.h"
#include "hns/components/ntp_background_images/browser/view_counter_service.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "hns/components/version_info/version_info.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/settings/metrics_reporting_handler.h"
#include "components/sync/base/command_line_switches.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/common/content_features.h"
#include "extensions/buildflags/buildflags.h"
#include "net/base/features.h"
#include "third_party/blink/public/common/features.h"

#if BUILDFLAG(ENABLE_PIN_SHORTCUT)
#include "hns/browser/ui/webui/settings/pin_shortcut_handler.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/components/speedreader/common/features.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/browser/hns_vpn/vpn_utils.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#if BUILDFLAG(IS_WIN)
#include "hns/browser/ui/webui/settings/hns_vpn/hns_vpn_handler.h"
#endif
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/browser/ui/webui/settings/hns_tor_handler.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "hns/browser/ui/webui/settings/hns_extensions_manifest_v2_handler.h"
#include "hns/browser/ui/webui/settings/hns_tor_snowflake_extension_handler.h"
#endif

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/browser/ui/webui/settings/hns_settings_leo_assistant_handler.h"
#include "hns/components/ai_chat/common/features.h"
#endif

using ntp_background_images::ViewCounterServiceFactory;

HnsSettingsUI::HnsSettingsUI(content::WebUI* web_ui,
                                 const std::string& host)
    : SettingsUI(web_ui) {
  web_ui->AddMessageHandler(
      std::make_unique<settings::MetricsReportingHandler>());
  web_ui->AddMessageHandler(std::make_unique<HnsPrivacyHandler>());
  web_ui->AddMessageHandler(std::make_unique<DefaultHnsShieldsHandler>());
  web_ui->AddMessageHandler(std::make_unique<HnsDefaultExtensionsHandler>());
  web_ui->AddMessageHandler(std::make_unique<HnsAppearanceHandler>());
  web_ui->AddMessageHandler(std::make_unique<HnsSyncHandler>());
  web_ui->AddMessageHandler(std::make_unique<HnsWalletHandler>());
  web_ui->AddMessageHandler(std::make_unique<HnsAdBlockHandler>());
#if BUILDFLAG(ENABLE_AI_CHAT)
  web_ui->AddMessageHandler(
      std::make_unique<settings::HnsLeoAssistantHandler>());
#endif
#if BUILDFLAG(ENABLE_TOR)
  web_ui->AddMessageHandler(std::make_unique<HnsTorHandler>());
#endif
#if BUILDFLAG(ENABLE_EXTENSIONS)
  web_ui->AddMessageHandler(
      std::make_unique<HnsTorSnowflakeExtensionHandler>());
  if (base::FeatureList::IsEnabled(kExtensionsManifestV2)) {
    web_ui->AddMessageHandler(
        std::make_unique<HnsExtensionsManifestV2Handler>());
  }
#endif
#if BUILDFLAG(ENABLE_PIN_SHORTCUT)
  web_ui->AddMessageHandler(std::make_unique<PinShortcutHandler>());
#endif
#if BUILDFLAG(IS_WIN) && BUILDFLAG(ENABLE_HNS_VPN)
  if (hns_vpn::IsHnsVPNEnabled(Profile::FromWebUI(web_ui))) {
    web_ui->AddMessageHandler(
        std::make_unique<HnsVpnHandler>(Profile::FromWebUI(web_ui)));
  }
#endif
}

HnsSettingsUI::~HnsSettingsUI() = default;

// static
void HnsSettingsUI::AddResources(content::WebUIDataSource* html_source,
                                   Profile* profile) {
  for (size_t i = 0; i < kHnsSettingsResourcesSize; ++i) {
    html_source->AddResourcePath(kHnsSettingsResources[i].path,
                                 kHnsSettingsResources[i].id);
  }

  // These resource files are generated from the files in
  // hns/browser/resources/settings/shortcuts_page
  // They are generated separately so they can use React and our Leo
  // components, and the React DOM is mounted inside a Web Component, so it
  // doesn't interfere with the Polymer tree/styles.
  if (base::FeatureList::IsEnabled(commands::features::kHnsCommands)) {
    for (size_t i = 0; i < kCommandsGeneratedSize; ++i) {
      html_source->AddResourcePath(kCommandsGenerated[i].path,
                                   kCommandsGenerated[i].id);
    }
  }

  html_source->AddBoolean("isSyncDisabled", !syncer::IsSyncAllowedByFlag());
  html_source->AddString(
      "hnsProductVersion",
      version_info::GetHnsVersionWithoutChromiumMajorVersion());
  NavigationBarDataProvider::Initialize(html_source, profile);
  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile)) {
    service->InitializeWebUIDataSource(html_source);
  }
  html_source->AddBoolean(
      "isIdleDetectionFeatureEnabled",
      base::FeatureList::IsEnabled(features::kIdleDetection));
  html_source->AddBoolean(
      "isHnsWebSerialApiEnabled",
      base::FeatureList::IsEnabled(blink::features::kHnsWebSerialAPI));
#if BUILDFLAG(ENABLE_HNS_VPN)
  html_source->AddBoolean("isHnsVPNEnabled",
                          hns_vpn::IsHnsVPNEnabled(profile));
#endif
#if BUILDFLAG(ENABLE_SPEEDREADER)
  html_source->AddBoolean(
      "isSpeedreaderFeatureEnabled",
      base::FeatureList::IsEnabled(speedreader::kSpeedreaderFeature));
#endif
  html_source->AddBoolean(
      "isNativeHnsWalletFeatureEnabled",
      base::FeatureList::IsEnabled(
          hns_wallet::features::kNativeHnsWalletFeature));
  html_source->AddBoolean("isHnsWalletAllowed",
                          hns_wallet::IsAllowedForContext(profile));
  html_source->AddBoolean("isForgetFirstPartyStorageFeatureEnabled",
                          base::FeatureList::IsEnabled(
                              net::features::kHnsForgetFirstPartyStorage));
  html_source->AddBoolean("isHnsRewardsSupported",
                          hns_rewards::IsSupportedForProfile(profile));
  html_source->AddBoolean(
      "areShortcutsSupported",
      base::FeatureList::IsEnabled(commands::features::kHnsCommands));

  if (ShouldDisableCSPForTesting()) {
    html_source->DisableContentSecurityPolicy();
  }

  html_source->AddBoolean("shouldExposeElementsForTesting",
                          ShouldExposeElementsForTesting());

  html_source->AddBoolean("enable_extensions", BUILDFLAG(ENABLE_EXTENSIONS));

  html_source->AddBoolean("extensionsManifestV2Feature",
                          base::FeatureList::IsEnabled(kExtensionsManifestV2));
#if defined(TOOLKIT_VIEWS)
  html_source->AddBoolean(
      "verticalTabStripFeatureEnabled",
      base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs));
#endif

#if BUILDFLAG(ENABLE_AI_CHAT)
  html_source->AddBoolean("isLeoAssistantAllowed",
                          ai_chat::features::IsAIChatEnabled());
#else
  html_source->AddBoolean("isLeoAssistantAllowed", false);
#endif
}

// static
bool& HnsSettingsUI::ShouldDisableCSPForTesting() {
  static bool disable_csp = false;
  return disable_csp;
}

// static
bool& HnsSettingsUI::ShouldExposeElementsForTesting() {
  static bool expose_elements = false;
  return expose_elements;
}

void HnsSettingsUI::BindInterface(
    mojo::PendingReceiver<commands::mojom::CommandsService> pending_receiver) {
  commands::AcceleratorServiceFactory::GetForContext(
      web_ui()->GetWebContents()->GetBrowserContext())
      ->BindInterface(std::move(pending_receiver));
}
