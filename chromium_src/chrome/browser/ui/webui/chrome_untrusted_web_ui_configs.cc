/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/webui/chrome_untrusted_web_ui_configs.h"

#include "base/feature_list.h"
#include "hns/browser/ui/webui/hns_wallet/ledger/ledger_ui.h"
#include "hns/browser/ui/webui/hns_wallet/market/market_ui.h"
#include "hns/browser/ui/webui/hns_wallet/nft/nft_ui.h"
#include "hns/browser/ui/webui/hns_wallet/trezor/trezor_ui.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/playlist/common/buildflags/buildflags.h"
#include "build/build_config.h"
#include "content/public/browser/webui_config_map.h"

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/browser/ui/webui/ai_chat/ai_chat_ui.h"
#include "hns/components/ai_chat/common/features.h"
#endif  // BUILDFLAG(ENABLE_AI_CHAT)

#if BUILDFLAG(ENABLE_HNS_VPN)
#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/ui/webui/hns_vpn/vpn_panel_ui.h"
#endif  // !BUILDFLAG(IS_ANDROID)
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#endif  // BUILDFLAG(ENABLE_HNS_VPN)

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
#include "hns/browser/ui/webui/playlist_ui.h"
#include "hns/components/playlist/common/features.h"
#endif

#define RegisterChromeUntrustedWebUIConfigs \
  RegisterChromeUntrustedWebUIConfigs_ChromiumImpl

#include "src/chrome/browser/ui/webui/chrome_untrusted_web_ui_configs.cc"

#undef RegisterChromeUntrustedWebUIConfigs

void RegisterChromeUntrustedWebUIConfigs() {
  RegisterChromeUntrustedWebUIConfigs_ChromiumImpl();
#if !BUILDFLAG(IS_ANDROID)
  content::WebUIConfigMap::GetInstance().AddUntrustedWebUIConfig(
      std::make_unique<ledger::UntrustedLedgerUIConfig>());
  content::WebUIConfigMap::GetInstance().AddUntrustedWebUIConfig(
      std::make_unique<market::UntrustedMarketUIConfig>());
  content::WebUIConfigMap::GetInstance().AddUntrustedWebUIConfig(
      std::make_unique<trezor::UntrustedTrezorUIConfig>());
  content::WebUIConfigMap::GetInstance().AddUntrustedWebUIConfig(
      std::make_unique<nft::UntrustedNftUIConfig>());
#if BUILDFLAG(ENABLE_HNS_VPN)
  if (hns_vpn::IsHnsVPNFeatureEnabled()) {
    content::WebUIConfigMap::GetInstance().AddUntrustedWebUIConfig(
        std::make_unique<UntrustedVPNPanelUIConfig>());
  }
#endif  // BUILDFLAG(ENABLE_HNS_VPN)
#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
  if (base::FeatureList::IsEnabled(playlist::features::kPlaylist)) {
    content::WebUIConfigMap::GetInstance().AddUntrustedWebUIConfig(
        std::make_unique<playlist::UntrustedPlaylistUIConfig>());
    content::WebUIConfigMap::GetInstance().AddUntrustedWebUIConfig(
        std::make_unique<playlist::UntrustedPlaylistPlayerUIConfig>());
  }
#endif  // BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
#endif  // !BUILDFLAG(IS_ANDROID)

#if BUILDFLAG(ENABLE_AI_CHAT)
  if (ai_chat::features::IsAIChatEnabled()) {
    content::WebUIConfigMap::GetInstance().AddUntrustedWebUIConfig(
        std::make_unique<UntrustedChatUIConfig>());
  }
#endif  // BUILDFLAG(ENABLE_AI_CHAT)
}
