/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "build/build_config.h"

const char kAdblockHost[] = "adblock";
const char kAdblockInternalsHost[] = "adblock-internals";
const char kAdblockJS[] = "hns_adblock.js";
const char kSkusInternalsHost[] = "skus-internals";
#if BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
const char kIPFSWebUIHost[] = "ipfs-internals";
const char kIPFSWebUIURL[] = "chrome://ipfs-internals/";
#endif
const char kWebcompatReporterHost[] = "webcompat";
const char kRewardsPageHost[] = "rewards";
const char kRewardsInternalsHost[] = "rewards-internals";
const char kWelcomeHost[] = "welcome";
const char kWelcomeJS[] = "hns_welcome.js";
const char kHnsRewardsPanelURL[] = "chrome://rewards-panel.top-chrome";
const char kHnsRewardsPanelHost[] = "rewards-panel.top-chrome";
const char kHnsTipPanelURL[] = "chrome://tip-panel.top-chrome";
const char kHnsTipPanelHost[] = "tip-panel.top-chrome";
const char kHnsNewTabJS[] = "hns_new_tab.js";
const char kHnsNewsInternalsHost[] = "news-internals";
const char kHnsUIRewardsURL[] = "chrome://rewards/";
const char kHnsUIAdblockURL[] = "chrome://adblock/";
const char kHnsUIWebcompatReporterURL[] = "chrome://webcompat/";
const char kHnsUIWalletURL[] = "chrome://wallet/";
const char kHnsUIWalletOnboardingURL[] = "hns://wallet/crypto/onboarding";
const char kHnsUIWalletAccountCreationURL[] =
    "hns://wallet/crypto/accounts/add-account/create/";
const char kHnsUIWalletPanelURL[] = "chrome://wallet-panel.top-chrome/";
const char kWalletPanelHost[] = "wallet-panel.top-chrome";
const char kVPNPanelURL[] = "chrome-untrusted://vpn-panel.top-chrome/";
const char kVPNPanelHost[] = "vpn-panel.top-chrome";
const char kHnsUIWalletPageURL[] = "chrome://wallet/";
const char kWalletPageHost[] = "wallet";
#if BUILDFLAG(IS_ANDROID)
const char kWalletBuyPagePath[] = "/fund-wallet";
const char kWalletSendPagePath[] = "/send";
const char kWalletSwapPagePath[] = "/swap";
const char kWalletDepositPagePath[] = "/deposit-funds";
#endif  // BUILDFLAG(IS_ANDROID)
const char kExtensionSettingsURL[] = "hns://settings/extensions";
const char kWalletSettingsURL[] = "hns://settings/wallet";
const char kHnsSyncPath[] = "hnsSync";
const char kHnsSyncSetupPath[] = "hnsSync/setup";
const char kTorInternalsHost[] = "tor-internals";
const char kUntrustedLedgerHost[] = "ledger-bridge";
const char kUntrustedLedgerURL[] = "chrome-untrusted://ledger-bridge/";
const char kUntrustedNftHost[] = "nft-display";
const char kUntrustedNftURL[] = "chrome-untrusted://nft-display/";
const char kUntrustedMarketHost[] = "market-display";
const char kUntrustedMarketURL[] = "chrome-untrusted://market-display/";
const char kUntrustedTrezorHost[] = "trezor-bridge";
const char kUntrustedTrezorURL[] = "chrome-untrusted://trezor-bridge/";
const char kShieldsPanelURL[] = "chrome://hns-shields.top-chrome";
const char kShieldsPanelHost[] = "hns-shields.top-chrome";
const char kCookieListOptInHost[] = "cookie-list-opt-in.top-chrome";
const char kCookieListOptInURL[] = "chrome://cookie-list-opt-in.top-chrome";
const char kFederatedInternalsURL[] = "hns://federated-internals";
const char kFederatedInternalsHost[] = "federated-internals";
const char kContentFiltersPath[] = "shields/filters";
const char kPlaylistHost[] = "playlist";
const char kPlaylistURL[] = "chrome-untrusted://playlist/";
const char kPlaylistPlayerHost[] = "playlist-player";
const char kPlaylistPlayerURL[] = "chrome-untrusted://playlist-player/";
const char kSpeedreaderPanelURL[] = "chrome://hns-speedreader.top-chrome";
const char kSpeedreaderPanelHost[] = "hns-speedreader.top-chrome";
const char kShortcutsURL[] = "chrome://settings/system/shortcuts";
const char kChatUIURL[] = "chrome-untrusted://chat/";
const char kChatUIHost[] = "chat";
