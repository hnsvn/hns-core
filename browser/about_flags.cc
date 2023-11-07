/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// This file is included into //chrome/browser/about_flags.cc.

#include <initializer_list>

#include "base/strings/string_util.h"
#include "hns/browser/hns_browser_features.h"
#include "hns/browser/hns_features_internal_names.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/browser/ethereum_remote_client/features.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/hns_ads/browser/feature/custom_notification_ad_feature.h"
#include "hns/components/hns_ads/core/public/feature/hns_ads_feature.h"
#include "hns/components/hns_ads/core/public/feature/notification_ad_feature.h"
#include "hns/components/hns_component_updater/browser/features.h"
#include "hns/components/hns_federated/features.h"
#include "hns/components/hns_news/common/features.h"
#include "hns/components/hns_rewards/common/buildflags/buildflags.h"
#include "hns/components/hns_rewards/common/features.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/hns_sync/features.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/common/features.h"
#include "hns/components/de_amp/common/features.h"
#include "hns/components/debounce/common/features.h"
#include "hns/components/google_sign_in_permission/features.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/ntp_background_images/browser/features.h"
#include "hns/components/playlist/common/buildflags/buildflags.h"
#include "hns/components/request_otr/common/buildflags/buildflags.h"
#include "hns/components/skus/common/features.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/ui/ui_features.h"
#include "components/content_settings/core/common/features.h"
#include "components/flags_ui/feature_entry.h"
#include "components/flags_ui/feature_entry_macros.h"
#include "components/flags_ui/flags_state.h"
#include "components/translate/core/browser/translate_prefs.h"
#include "net/base/features.h"
#include "third_party/blink/public/common/features.h"

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/components/ai_chat/common/features.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/common/features.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/components/speedreader/common/features.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/features.h"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
#include "hns/components/playlist/common/features.h"
#endif

#if BUILDFLAG(ENABLE_REQUEST_OTR)
#include "hns/components/request_otr/common/features.h"
#endif

#if BUILDFLAG(IS_ANDROID)
#include "hns/browser/android/preferences/features.h"
#include "hns/browser/android/safe_browsing/features.h"
#else
#include "hns/components/commands/common/features.h"
#endif

#if BUILDFLAG(IS_WIN)
#include "sandbox/policy/features.h"
#endif

#define EXPAND_FEATURE_ENTRIES(...) __VA_ARGS__,

#if BUILDFLAG(ENABLE_HNS_VPN)
#define HNS_VPN_FEATURE_ENTRIES                         \
  EXPAND_FEATURE_ENTRIES({                                \
      kHnsVPNFeatureInternalName,                       \
      "Enable experimental Hns VPN",                    \
      "Experimental native VPN support",                  \
      kOsMac | kOsWin,                                    \
      FEATURE_VALUE_TYPE(hns_vpn::features::kHnsVPN), \
  })
#if BUILDFLAG(IS_WIN)

#define HNS_VPN_WIREGUARD_FEATURE_ENTRIES                                  \
  EXPAND_FEATURE_ENTRIES({                                                   \
      kHnsVPNWireguardFeatureInternalName,                                 \
      "Enable experimental WireGuard Hns VPN service",                     \
      "Experimental WireGuard VPN support. Deprecated.",                     \
      kOsWin,                                                                \
      FEATURE_VALUE_TYPE(hns_vpn::features::kHnsVPNUseWireguardService), \
  })
#define HNS_VPN_DNS_FEATURE_ENTRIES                                    \
  EXPAND_FEATURE_ENTRIES({                                               \
      kHnsVPNDnsFeatureInternalName,                                   \
      "Enable DoH for Hns VPN",                                        \
      "Override DoH settings with Cloudflare dns if necessary to avoid " \
      "leaking requests due to Smart Multi-Home Named Resolution",       \
      kOsWin,                                                            \
      FEATURE_VALUE_TYPE(hns_vpn::features::kHnsVPNDnsProtection),   \
  })
#else
#define HNS_VPN_DNS_FEATURE_ENTRIES
#define HNS_VPN_WIREGUARD_FEATURE_ENTRIES
#endif
#else
#define HNS_VPN_FEATURE_ENTRIES
#define HNS_VPN_DNS_FEATURE_ENTRIES
#define HNS_VPN_WIREGUARD_FEATURE_ENTRIES
#endif

#define HNS_SKU_SDK_FEATURE_ENTRIES                   \
  EXPAND_FEATURE_ENTRIES({                              \
      "skus-sdk",                                       \
      "Enable experimental SKU SDK",                    \
      "Experimental SKU SDK support",                   \
      kOsMac | kOsWin | kOsAndroid,                     \
      FEATURE_VALUE_TYPE(skus::features::kSkusFeature), \
  })

#define SPEEDREADER_FEATURE_ENTRIES                                        \
  IF_BUILDFLAG(                                                            \
      ENABLE_SPEEDREADER,                                                  \
      EXPAND_FEATURE_ENTRIES({                                             \
          "hns-speedreader",                                             \
          "Enable SpeedReader",                                            \
          "Enables faster loading of simplified article-style web pages.", \
          kOsDesktop | kOsAndroid,                                         \
          FEATURE_VALUE_TYPE(speedreader::kSpeedreaderFeature),            \
      }))

#define REQUEST_OTR_FEATURE_ENTRIES                                           \
  IF_BUILDFLAG(                                                               \
      ENABLE_REQUEST_OTR,                                                     \
      EXPAND_FEATURE_ENTRIES({                                                \
          "hns-request-otr-tab",                                            \
          "Enable Request-OTR Tab",                                           \
          "Suggest going off-the-record when visiting potentially sensitive " \
          "URLs",                                                             \
          kOsDesktop | kOsAndroid,                                            \
          FEATURE_VALUE_TYPE(request_otr::features::kHnsRequestOTRTab),     \
      }))

#define HNS_MODULE_FILENAME_PATCH                                            \
  IF_BUILDFLAG(                                                                \
      IS_WIN,                                                                  \
      EXPAND_FEATURE_ENTRIES({                                                 \
          "hns-module-filename-patch",                                       \
          "Enable Module Filename patch",                                      \
          "Enables patching of executable's name from hns.exe to "           \
          "chrome.exe in sandboxed processes.",                                \
          kOsWin,                                                              \
          FEATURE_VALUE_TYPE(sandbox::policy::features::kModuleFileNamePatch), \
      }))

#define HNS_REWARDS_GEMINI_FEATURE_ENTRIES                               \
  IF_BUILDFLAG(                                                            \
      ENABLE_GEMINI_WALLET,                                                \
      EXPAND_FEATURE_ENTRIES({                                             \
          "hns-rewards-gemini",                                          \
          "Enable Gemini for Hns Rewards",                               \
          "Enables support for Gemini as an external wallet provider for " \
          "Hns",                                                         \
          kOsDesktop,                                                      \
          FEATURE_VALUE_TYPE(hns_rewards::features::kGeminiFeature),     \
      }))

#define HNS_IPFS_FEATURE_ENTRIES                                   \
  IF_BUILDFLAG(ENABLE_IPFS,                                          \
               EXPAND_FEATURE_ENTRIES({                              \
                   "hns-ipfs",                                     \
                   "Enable IPFS",                                    \
                   "Enable native support of IPFS.",                 \
                   kOsDesktop | kOsAndroid,                          \
                   FEATURE_VALUE_TYPE(ipfs::features::kIpfsFeature), \
               }))

#define HNS_NATIVE_WALLET_FEATURE_ENTRIES                                   \
  EXPAND_FEATURE_ENTRIES(                                                     \
      {                                                                       \
          "enable-nft-pinning",                                               \
          "Enable NFT pinning",                                               \
          "Enable NFT pinning for Hns Wallet",                              \
          kOsDesktop,                                                         \
          FEATURE_VALUE_TYPE(                                                 \
              hns_wallet::features::kHnsWalletNftPinningFeature),         \
      },                                                                      \
      {                                                                       \
          "enable-panel-v2",                                                  \
          "Enable Panel v2",                                                  \
          "Enable Panel v2 for Hns Wallet",                                 \
          kOsDesktop,                                                         \
          FEATURE_VALUE_TYPE(                                                 \
              hns_wallet::features::kHnsWalletPanelV2Feature),            \
      },                                                                      \
      {                                                                       \
          "native-hns-wallet",                                              \
          "Enable Hns Wallet",                                              \
          "Native cryptocurrency wallet support without the use of "          \
          "extensions",                                                       \
          kOsDesktop | kOsAndroid,                                            \
          FEATURE_VALUE_TYPE(                                                 \
              hns_wallet::features::kNativeHnsWalletFeature),             \
      },                                                                      \
      {                                                                       \
          "hns-wallet-filecoin",                                            \
          "Enable Hns Wallet Filecoin support",                             \
          "Filecoin support for native Hns Wallet",                         \
          kOsDesktop | kOsAndroid,                                            \
          FEATURE_VALUE_TYPE(                                                 \
              hns_wallet::features::kHnsWalletFilecoinFeature),           \
      },                                                                      \
      {                                                                       \
          "hns-wallet-solana",                                              \
          "Enable Hns Wallet Solana support",                               \
          "Solana support for native Hns Wallet",                           \
          kOsDesktop | kOsAndroid,                                            \
          FEATURE_VALUE_TYPE(                                                 \
              hns_wallet::features::kHnsWalletSolanaFeature),             \
      },                                                                      \
      {                                                                       \
          "hns-wallet-solana-provider",                                     \
          "Enable Hns Wallet Solana provider support",                      \
          "Solana provider support for native Hns Wallet",                  \
          kOsDesktop | kOsAndroid,                                            \
          FEATURE_VALUE_TYPE(                                                 \
              hns_wallet::features::kHnsWalletSolanaProviderFeature),     \
      },                                                                      \
      {                                                                       \
          "hns-wallet-sns",                                                 \
          "Enable Solana Name Service support",                               \
          "Enable Solana Name Service(.sol) support for Wallet and omnibox "  \
          "address resolution",                                               \
          kOsDesktop | kOsAndroid,                                            \
          FEATURE_VALUE_TYPE(hns_wallet::features::kHnsWalletSnsFeature), \
      },                                                                      \
      {                                                                       \
          "hns-wallet-dapps-support",                                       \
          "Enable Hns Wallet Dapps support",                                \
          "Hns Wallet Dapps support",                                       \
          kOsDesktop | kOsAndroid,                                            \
          FEATURE_VALUE_TYPE(                                                 \
              hns_wallet::features::kHnsWalletDappsSupportFeature),       \
      })

#define HNS_NEWS_FEATURE_ENTRIES                                             \
  EXPAND_FEATURE_ENTRIES(                                                      \
      {                                                                        \
          "hns-news-peek",                                                   \
          "Hns News prompts on New Tab Page",                                \
          "Prompt Hns News via the top featured article peeking up from "    \
          "the bottom of the New Tab Page, after a short delay.",              \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(hns_news::features::kHnsNewsCardPeekFeature), \
      },                                                                       \
      {                                                                        \
          "hns-news-feed-update",                                            \
          "Hns News Feed Update",                                            \
          "Use the updated Hns News feed",                                   \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(hns_news::features::kHnsNewsFeedUpdate),      \
      })

#define HNS_FEDERATED_FEATURE_ENTRIES                                        \
  EXPAND_FEATURE_ENTRIES({                                                     \
      "hns-federated",                                                       \
      "Enables local data collection for notification ad timing "              \
      "(hns-federated)",                                                     \
      "Starts local collection for notification ad timing data. This data is " \
      "stored locally and automatically erased after one month. No data "      \
      "leaves the client.",                                                    \
      kOsDesktop,                                                              \
      FEATURE_VALUE_TYPE(hns_federated::features::kFederatedLearning),       \
  })

#define CRYPTO_WALLETS_FEATURE_ENTRIES                                      \
  IF_BUILDFLAG(                                                             \
      ETHEREUM_REMOTE_CLIENT_ENABLED,                                       \
      EXPAND_FEATURE_ENTRIES({                                              \
          "ethereum_remote-client_new-installs",                            \
          "Enable Crypto Wallets option in settings",                       \
          "Crypto Wallets extension is deprecated but with this option it " \
          "can "                                                            \
          "still be enabled in settings. If it was previously used, this "  \
          "flag is "                                                        \
          "ignored.",                                                       \
          kOsDesktop,                                                       \
          FEATURE_VALUE_TYPE(ethereum_remote_client::features::             \
                                 kCryptoWalletsForNewInstallsFeature),      \
      }))

#define PLAYLIST_FEATURE_ENTRIES                                       \
  IF_BUILDFLAG(                                                        \
      ENABLE_PLAYLIST,                                                 \
      EXPAND_FEATURE_ENTRIES(                                          \
          {                                                            \
              kPlaylistFeatureInternalName,                            \
              "Playlist",                                              \
              "Enables Playlist",                                      \
              kOsMac | kOsWin | kOsLinux | kOsAndroid,                 \
              FEATURE_VALUE_TYPE(playlist::features::kPlaylist),       \
          },                                                           \
          {                                                            \
              kPlaylistFakeUAFeatureInternalName,                      \
              "PlaylistFakeUA",                                        \
              "Use fake UA for playlist",                              \
              kOsMac | kOsWin | kOsLinux | kOsAndroid,                 \
              FEATURE_VALUE_TYPE(playlist::features::kPlaylistFakeUA), \
          }))

#if !BUILDFLAG(IS_ANDROID)
#define HNS_COMMANDS_FEATURE_ENTRIES                                        \
  EXPAND_FEATURE_ENTRIES({                                                    \
      "hns-commands",                                                       \
      "Hns Commands",                                                       \
      "Enable experimental page for viewing and executing commands in Hns", \
      kOsWin | kOsMac | kOsLinux,                                             \
      FEATURE_VALUE_TYPE(commands::features::kHnsCommands),                 \
  })
#else
#define HNS_COMMANDS_FEATURE_ENTRIES
#endif

#if defined(TOOLKIT_VIEWS)
#define HNS_VERTICAL_TABS_FEATURE_ENTRY                                \
  EXPAND_FEATURE_ENTRIES({                                               \
      "hns-vertical-tabs",                                             \
      "Vertical tabs",                                                   \
      "Move tab strip to be a vertical panel on the side of the window " \
      "instead of horizontal at the top of the window.",                 \
      kOsWin | kOsMac | kOsLinux,                                        \
      FEATURE_VALUE_TYPE(tabs::features::kHnsVerticalTabs),            \
  })
#define HNS_VERTICAL_TABS_STICKY_PINNED_TABS_FEATURE_ENTRY                  \
  EXPAND_FEATURE_ENTRIES({                                                    \
      "hns-vertical-tabs-stick-pinned-tabs",                                \
      "Vertical tabs - sticky pinned tabs",                                   \
      "Pinned tabs will be on top of unpinned tabs regardless scroll state",  \
      kOsWin | kOsMac | kOsLinux,                                             \
      FEATURE_VALUE_TYPE(tabs::features::kHnsVerticalTabsStickyPinnedTabs), \
  })
#else
#define HNS_VERTICAL_TABS_FEATURE_ENTRY
#define HNS_VERTICAL_TABS_STICKY_PINNED_TABS_FEATURE_ENTRY
#endif

#if BUILDFLAG(IS_LINUX)
#define HNS_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES               \
  EXPAND_FEATURE_ENTRIES({                                                    \
      "hns-change-active-tab-on-scroll-event",                              \
      "Change active tab on scroll event",                                    \
      "Change the active tab when scroll events occur on tab strip.",         \
      kOsLinux,                                                               \
      FEATURE_VALUE_TYPE(tabs::features::kHnsChangeActiveTabOnScrollEvent), \
  })
#else
#define HNS_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IS_ANDROID)
#define HNS_BACKGROUND_VIDEO_PLAYBACK_ANDROID                                \
  EXPAND_FEATURE_ENTRIES({                                                     \
      "hns-background-video-playback",                                       \
      "Background video playback",                                             \
      "Enables play audio from video in background when tab is not active or " \
      "device screen is turned off. Try to switch to desktop mode if this "    \
      "feature is not working.",                                               \
      kOsAndroid,                                                              \
      FEATURE_VALUE_TYPE(                                                      \
          preferences::features::kHnsBackgroundVideoPlayback),               \
  })
#define HNS_SAFE_BROWSING_ANDROID                                           \
  EXPAND_FEATURE_ENTRIES({                                                    \
      "hns-safe-browsing",                                                  \
      "Safe Browsing",                                                        \
      "Enables Google Safe Browsing for determining whether a URL has been "  \
      "marked as a known threat.",                                            \
      kOsAndroid,                                                             \
      FEATURE_VALUE_TYPE(safe_browsing::features::kHnsAndroidSafeBrowsing), \
  })
#else
#define HNS_BACKGROUND_VIDEO_PLAYBACK_ANDROID
#define HNS_SAFE_BROWSING_ANDROID
#endif  // BUILDFLAG(IS_ANDROID)

#if !BUILDFLAG(IS_ANDROID)
#define HNS_SHARED_PINNED_TABS                                  \
  EXPAND_FEATURE_ENTRIES({                                        \
      "hns-shared-pinned-tabs",                                 \
      "Shared pinned tab",                                        \
      "Pinned tabs are shared across windows",                    \
      kOsWin | kOsMac | kOsLinux,                                 \
      FEATURE_VALUE_TYPE(tabs::features::kHnsSharedPinnedTabs), \
  })
#else
#define HNS_SHARED_PINNED_TABS
#endif

#if BUILDFLAG(ENABLE_AI_CHAT)
#define HNS_AI_CHAT                                          \
  EXPAND_FEATURE_ENTRIES({                                     \
      "hns-ai-chat",                                         \
      "Hns AI Chat",                                         \
      "Summarize articles and engage in conversation with AI", \
      kOsWin | kOsMac | kOsLinux,                              \
      FEATURE_VALUE_TYPE(ai_chat::features::kAIChat),          \
  })
#else
#define HNS_AI_CHAT
#endif

// Keep the last item empty.
#define LAST_HNS_FEATURE_ENTRIES_ITEM

#define HNS_ABOUT_FLAGS_FEATURE_ENTRIES                                      \
  EXPAND_FEATURE_ENTRIES(                                                      \
      {                                                                        \
          "use-dev-updater-url",                                               \
          "Use dev updater url",                                               \
          "Use the dev url for the component updater. This is for internal "   \
          "testing only.",                                                     \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_component_updater::kUseDevUpdaterUrl),      \
      },                                                                       \
      {                                                                        \
          "allow-certain-client-hints",                                        \
          "Allow certain request client hints",                                \
          "Allows setting certain request client hints (sec-ch-ua, "           \
          "sec-ch-ua-mobile, sec-ch-ua-platform)",                             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kAllowCertainClientHints),       \
      },                                                                       \
      {                                                                        \
          "clamp-platform-version-client-hint",                                \
          "Clamp platform version client hint",                                \
          "Clamps the patch field of the platform version client hint",        \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              blink::features::kClampPlatformVersionClientHint),               \
      },                                                                       \
      {                                                                        \
          "hns-ntp-branded-wallpaper-demo",                                  \
          "New Tab Page Demo Branded Wallpaper",                               \
          "Force dummy data for the Branded Wallpaper New Tab Page "           \
          "Experience. View rate and user opt-in conditionals will still be "  \
          "followed to decide when to display the Branded Wallpaper.",         \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              ntp_background_images::features::kHnsNTPBrandedWallpaperDemo), \
      },                                                                       \
      {                                                                        \
          "hns-adblock-cname-uncloaking",                                    \
          "Enable CNAME uncloaking",                                           \
          "Take DNS CNAME records into account when making network request "   \
          "blocking decisions.",                                               \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsAdblockCnameUncloaking),          \
      },                                                                       \
      {                                                                        \
          "hns-adblock-collapse-blocked-elements",                           \
          "Collapse HTML elements with blocked source attributes",             \
          "Cause iframe and img elements to be collapsed if the URL of their " \
          "src attribute is blocked",                                          \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsAdblockCollapseBlockedElements),  \
      },                                                                       \
      {                                                                        \
          "hns-adblock-cookie-list-default",                                 \
          "Treat 'Easylist-Cookie List' as a default list source",             \
          "Enables the 'Easylist-Cookie List' regional list if its toggle in " \
          "hns://adblock hasn't otherwise been modified",                    \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsAdblockCookieListDefault),        \
      },                                                                       \
      {                                                                        \
          "hns-adblock-cookie-list-opt-in",                                  \
          "Show an opt-in bubble for the 'Easylist-Cookie List' filter",       \
          "When enabled, a bubble will be displayed inviting the user to "     \
          "enable the 'Easylist-Cookie List' filter for blocking cookie "      \
          "consent dialogs",                                                   \
          kOsDesktop | kOsAndroid,                                             \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsAdblockCookieListOptIn),          \
      },                                                                       \
      {                                                                        \
          "hns-adblock-cosmetic-filtering",                                  \
          "Enable cosmetic filtering",                                         \
          "Enable support for cosmetic filtering",                             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsAdblockCosmeticFiltering),        \
      },                                                                       \
      {                                                                        \
          "hns-adblock-csp-rules",                                           \
          "Enable support for CSP rules",                                      \
          "Applies additional CSP rules to pages for which a $csp rule has "   \
          "been loaded from a filter list",                                    \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_shields::features::kHnsAdblockCspRules),  \
      },                                                                       \
      {                                                                        \
          "hns-adblock-default-1p-blocking",                                 \
          "Shields first-party network blocking",                              \
          "Allow Hns Shields to block first-party network requests in "      \
          "Standard blocking mode",                                            \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsAdblockDefault1pBlocking),        \
      },                                                                       \
      {                                                                        \
          "hns-adblock-mobile-notifications-list-default",                   \
          "Treat 'Fanboy's Mobile Notifications List' as a default list "      \
          "source",                                                            \
                                                                               \
          "Enables the 'Fanboy's Mobile Notifications List' regional list if " \
          "its toggle in hns://adblock hasn't otherwise been modified",      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_shields::features::                         \
                                 kHnsAdblockMobileNotificationsListDefault), \
      },                                                                       \
      {                                                                        \
          "hns-adblock-scriptlet-debug-logs",                                \
          "Enable debug logging for scriptlet injections",                     \
          "Enable console debugging for scriptlets injected by cosmetic "      \
          "filtering, exposing additional information that can be useful for " \
          "filter authors.",                                                   \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsAdblockScriptletDebugLogs),       \
      },                                                                       \
      {                                                                        \
          "hns-dark-mode-block",                                             \
          "Enable dark mode blocking fingerprinting protection",               \
          "Always report light mode when fingerprinting protections set to "   \
          "Strict",                                                            \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_shields::features::kHnsDarkModeBlock),    \
      },                                                                       \
      {                                                                        \
          "hns-domain-block",                                                \
          "Enable domain blocking",                                            \
          "Enable support for blocking domains with an interstitial page",     \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_shields::features::kHnsDomainBlock),      \
      },                                                                       \
      {                                                                        \
          "hns-domain-block-1pes",                                           \
          "Enable domain blocking using First Party Ephemeral Storage",        \
          "When visiting a blocked domain, Hns will try to enable "          \
          "Ephemeral Storage for a first party context, meaning neither "      \
          "cookies nor localStorage data will be persisted after a website "   \
          "is closed. Ephemeral Storage will be auto-enabled only if no data " \
          "was previously stored for a website",                               \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_shields::features::kHnsDomainBlock1PES),  \
      },                                                                       \
      {                                                                        \
          "hns-debounce",                                                    \
          "Enable debouncing",                                                 \
          "Enable support for skipping top-level redirect tracking URLs",      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(debounce::features::kHnsDebounce),              \
      },                                                                       \
      {                                                                        \
          "hns-de-amp",                                                      \
          "Enable De-AMP",                                                     \
          "Enable De-AMPing feature",                                          \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(de_amp::features::kHnsDeAMP),                   \
      },                                                                       \
      {                                                                        \
          "hns-google-sign-in-permission",                                   \
          "Enable Google Sign-In Permission Prompt",                           \
          "Enable permissioning access to legacy Google Sign-In",              \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(google_sign_in_permission::features::             \
                                 kHnsGoogleSignInPermission),                \
      },                                                                       \
      {                                                                        \
          "hns-localhost-access-permission",                                 \
          "Enable Localhost access permission prompt",                         \
          "Enable permissioning access to localhost connections",              \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsLocalhostAccessPermission),       \
      },                                                                       \
      {                                                                        \
          "hns-extension-network-blocking",                                  \
          "Enable extension network blocking",                                 \
          "Enable blocking for network requests initiated by extensions",      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kHnsExtensionNetworkBlocking),        \
      },                                                                       \
      {                                                                        \
          "hns-reduce-language",                                             \
          "Reduce language identifiability",                                   \
          "Reduce the identifiability of my language preferences",             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_shields::features::kHnsReduceLanguage),   \
      },                                                                       \
      {                                                                        \
          "hns-cosmetic-filtering-sync-load",                                \
          "Enable sync loading of cosmetic filter rules",                      \
          "Enable sync loading of cosmetic filter rules",                      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_shields::features::kCosmeticFilteringSyncLoad),            \
      },                                                                       \
      {                                                                        \
          "hns-super-referral",                                              \
          "Enable Hns Super Referral",                                       \
          "Use custom theme for Hns Super Referral",                         \
          flags_ui::kOsMac | flags_ui::kOsWin | flags_ui::kOsAndroid,          \
          FEATURE_VALUE_TYPE(ntp_background_images::features::                 \
                                 kHnsNTPSuperReferralWallpaper),             \
      },                                                                       \
      {                                                                        \
          "hns-ephemeral-storage",                                           \
          "Enable Ephemeral Storage",                                          \
          "Use ephemeral storage for third-party frames",                      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kHnsEphemeralStorage),           \
      },                                                                       \
      {                                                                        \
          "hns-ephemeral-storage-keep-alive",                                \
          "Ephemeral Storage Keep Alive",                                      \
          "Keep ephemeral storage partitions alive for a specified time "      \
          "after all tabs for that origin are closed",                         \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kHnsEphemeralStorageKeepAlive),  \
      },                                                                       \
      {                                                                        \
          "hns-first-party-ephemeral-storage",                               \
          "Enable First Party Ephemeral Storage",                              \
          "Enable support for First Party Ephemeral Storage using "            \
          "SESSION_ONLY cookie setting",                                       \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kHnsFirstPartyEphemeralStorage), \
      },                                                                       \
      {                                                                        \
          "hns-forget-first-party-storage",                                  \
          "Enable First Party Storage Cleanup support",                        \
          "Add cookie blocking mode which allows Hns to cleanup first "      \
          "party storage (Cookies, DOM Storage) on website close",             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kHnsForgetFirstPartyStorage),    \
      },                                                                       \
      {                                                                        \
          "hns-rewards-vbat-notice",                                         \
          "Enable Hns Rewards VBAT notices",                                 \
          "Enables notices in the Hns Rewards UI about VBAT deadlines.",     \
          kOsDesktop | kOsAndroid,                                             \
          FEATURE_VALUE_TYPE(hns_rewards::features::kVBatNoticeFeature),     \
      },                                                                       \
      {                                                                        \
          "hns-rewards-verbose-logging",                                     \
          "Enable Hns Rewards verbose logging",                              \
          "Enables detailed logging of Hns Rewards system events to a log "  \
          "file stored on your device. Please note that this log file could "  \
          "include information such as browsing history and credentials such " \
          "as passwords and access tokens depending on your activity. Please " \
          "do not share it unless asked to by Hns staff.",                   \
          kOsDesktop | kOsAndroid,                                             \
          FEATURE_VALUE_TYPE(hns_rewards::features::kVerboseLoggingFeature), \
      },                                                                       \
      {                                                                        \
          "hns-rewards-allow-unsupported-wallet-providers",                  \
          "Always show Hns Rewards custodial connection options",            \
                                                                               \
          "Allows all custodial options to be selected in Hns Rewards, "     \
          "including those not supported for your Rewards country.",           \
          kOsDesktop | kOsAndroid,                                             \
          FEATURE_VALUE_TYPE(hns_rewards::features::                         \
                                 kAllowUnsupportedWalletProvidersFeature),     \
      },                                                                       \
      {                                                                        \
          "hns-ads-should-launch-hns-ads-as-an-in-process-service",        \
          "Launch Hns Ads as an in-process service",                         \
          "Launch Hns Ads as an in-process service removing the utility "    \
          "process.",                                                          \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_ads::kShouldLaunchHnsAdsAsAnInProcessServiceFeature),    \
      },                                                                       \
      {                                                                        \
          "hns-ads-should-always-run-hns-ads-service",                     \
          "Should always run Hns Ads service",                               \
          "Always run Hns Ads service to support triggering ad events when " \
          "Hns Private Ads are disabled.",                                   \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_ads::kShouldAlwaysRunHnsAdsServiceFeature),              \
      },                                                                       \
      {                                                                        \
          "hns-ads-should-always-trigger-new-tab-page-ad-events",            \
          "Should always trigger new tab page ad events",                      \
          "Support triggering new tab page ad events if Hns Private Ads "    \
          "are disabled. Requires "                                            \
          "#hns-ads-should-always-run-hns-ads-service to be enabled.",     \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_ads::kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature),  \
      },                                                                       \
      {                                                                        \
          "hns-ads-should-support-search-result-ads",                        \
          "Support Search Result Ads feature",                                 \
          "Should be used in combination with "                                \
          "#hns-ads-should-always-trigger-search-result-ad-events and "      \
          "#hns-ads-should-always-run-hns-ads-service",                    \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_ads::kShouldSupportSearchResultAdsFeature), \
      },                                                                       \
      {                                                                        \
          "hns-ads-should-always-trigger-search-result-ad-events",           \
          "Should always trigger search result ad events",                     \
          "Support triggering search result ad events if Hns Private Ads "   \
          "are disabled. Requires "                                            \
          "#hns-ads-should-always-run-hns-ads-service to be enabled.",     \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_ads::                                                      \
                  kShouldAlwaysTriggerHnsSearchResultAdEventsFeature),       \
      },                                                                       \
      {                                                                        \
          "hns-ads-custom-push-notifications-ads",                           \
          "Enable Hns Ads custom push notifications",                        \
          "Enable Hns Ads custom push notifications to support rich media",  \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_ads::kCustomNotificationAdFeature),         \
      },                                                                       \
      {                                                                        \
          "hns-ads-allowed-to-fallback-to-custom-push-notification-ads",     \
          "Allow Hns Ads to fallback from native to custom push "            \
          "notifications",                                                     \
          "Allow Hns Ads to fallback from native to custom push "            \
          "notifications on operating systems which do not support native "    \
          "notifications",                                                     \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_ads::kAllowedToFallbackToCustomNotificationAdFeature),     \
      },                                                                       \
      {                                                                        \
          "hns-sync-v2",                                                     \
          "Enable Hns Sync v2",                                              \
          "Hns Sync v2 integrates with chromium sync engine with Hns "     \
          "specific authentication flow and enforce client side encryption",   \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(hns_sync::features::kHnsSync),                \
      },                                                                       \
      {                                                                        \
          "file-system-access-api",                                            \
          "File System Access API",                                            \
          "Enables the File System Access API, giving websites access to the " \
          "file system",                                                       \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(blink::features::kFileSystemAccessAPI),           \
      },                                                                       \
      {                                                                        \
          "hns-web-bluetooth-api",                                           \
          "Web Bluetooth API",                                                 \
          "Enables the Web Bluetooth API, giving websites access to "          \
          "Bluetooth devices",                                                 \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kHnsWebBluetoothAPI),          \
      },                                                                       \
      {                                                                        \
          "hns-web-serial-api",                                              \
          "Web Serial API",                                                    \
          "Enables the Web Serial API, allowing websites to request access "   \
          "to serial ports ",                                                  \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(blink::features::kHnsWebSerialAPI),             \
      },                                                                       \
      {                                                                        \
          "navigator-connection-attribute",                                    \
          "Enable navigator.connection attribute",                             \
          "Enables the navigator.connection API. Enabling this API will "      \
          "allow sites to learn information about your network and internet "  \
          "connection. Trackers can use this information to fingerprint your " \
          "browser, or to infer when you are traveling or at home.",           \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kNavigatorConnectionAttribute),  \
      },                                                                       \
      {                                                                        \
          "restrict-websockets-pool",                                          \
          "Restrict WebSockets pool",                                          \
          "Limits simultaneous active WebSockets connections per eTLD+1",      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kRestrictWebSocketsPool),        \
      },                                                                       \
      {                                                                        \
          "allow-incognito-permission-inheritance",                            \
          "Allow permission inheritance in incognito profiles",                \
          "When enabled, most permissions set in a normal profile will be "    \
          "inherited in incognito profile if they are less permissive, for "   \
          "ex. Geolocation BLOCK will be automatically set to BLOCK in "       \
          "incognito.",                                                        \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              content_settings::kAllowIncognitoPermissionInheritance),         \
      },                                                                       \
      {                                                                        \
          "hns-block-screen-fingerprinting",                                 \
          "Block screen fingerprinting",                                       \
          "Prevents JavaScript and CSS from learning the user's screen "       \
          "dimensions or window position.",                                    \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              blink::features::kHnsBlockScreenFingerprinting),               \
      },                                                                       \
      {                                                                        \
          "hns-tor-windows-https-only",                                      \
          "Use HTTPS-Only Mode in Private Windows with Tor",                   \
          "Prevents Private Windows with Tor from making any insecure HTTP "   \
          "connections without warning the user first.",                       \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kHnsTorWindowsHttpsOnly),        \
      },                                                                       \
      {                                                                        \
          "hns-round-time-stamps",                                           \
          "Round time stamps",                                                 \
          "Prevents JavaScript from getting access to high-resolution clocks " \
          "by rounding all DOMHighResTimeStamps to the nearest millisecond.",  \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kHnsRoundTimeStamps),          \
      },                                                                       \
      {                                                                        \
          "translate",                                                         \
          "Enable Chromium Translate feature",                                 \
          "Should be used with hns-translate-go, see the description here.", \
          kOsDesktop | kOsAndroid,                                             \
          FEATURE_VALUE_TYPE(translate::kTranslate),                           \
      },                                                                       \
      {                                                                        \
          "hns-sync-history-diagnostics",                                    \
          "Enable Hns Sync History Diagnostics",                             \
          "Hns Sync History Diagnostics flag displays additional sync "      \
          "related information on History page",                               \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              hns_sync::features::kHnsSyncHistoryDiagnostics),             \
      },                                                                       \
      {                                                                        \
          "restrict-event-source-pool",                                        \
          "Restrict Event Source Pool",                                        \
          "Limits simultaneous active WebSockets connections per eTLD+1",      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kRestrictEventSourcePool),       \
      },                                                                       \
      {                                                                        \
          "hns-sync-send-all-history",                                       \
          "Send All History to Hns Sync",                                    \
          "With Send All History flag all sync entries are sent to Sync "      \
          "server including transitions of link, bookmark, reload, etc",       \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(hns_sync::features::kHnsSyncSendAllHistory),  \
      },                                                                       \
      {                                                                        \
          "hns-copy-clean-link-by-default",                                  \
          "Override default copy hotkey with copy clean link",                 \
          "Sanitize url before copying, replaces default ctrl+c hotkey for "   \
          "url ",                                                              \
          kOsWin | kOsLinux | kOsMac,                                          \
          FEATURE_VALUE_TYPE(features::kHnsCopyCleanLinkByDefault),          \
      },                                                                       \
      {                                                                        \
          "hns-global-privacy-control-enabled",                              \
          "Enable Global Privacy Control",                                     \
          "Enable the Sec-GPC request header and the "                         \
          "navigator.globalPrivacyControl JS API",                             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kHnsGlobalPrivacyControl),     \
      },                                                                       \
      {                                                                        \
          "https-by-default",                                                  \
          "Use HTTPS by Default",                                              \
          "Attempt to connect to all websites using HTTPS before falling "     \
          "back to HTTP.",                                                     \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kHnsHttpsByDefault),             \
      },                                                                       \
      {                                                                        \
          "hns-override-download-danger-level",                              \
          "Override download danger level",                                    \
          "Disables download warnings for files which are considered "         \
          "dangerous when Safe Browsing is disabled. Use at your own risks. "  \
          "Not recommended.",                                                  \
          kOsWin | kOsLinux | kOsMac,                                          \
          FEATURE_VALUE_TYPE(features::kHnsOverrideDownloadDangerLevel),     \
      })                                                                       \
  HNS_IPFS_FEATURE_ENTRIES                                                   \
  HNS_NATIVE_WALLET_FEATURE_ENTRIES                                          \
  HNS_NEWS_FEATURE_ENTRIES                                                   \
  CRYPTO_WALLETS_FEATURE_ENTRIES                                               \
  HNS_REWARDS_GEMINI_FEATURE_ENTRIES                                         \
  HNS_VPN_FEATURE_ENTRIES                                                    \
  HNS_VPN_DNS_FEATURE_ENTRIES                                                \
  HNS_VPN_WIREGUARD_FEATURE_ENTRIES                                          \
  HNS_SKU_SDK_FEATURE_ENTRIES                                                \
  SPEEDREADER_FEATURE_ENTRIES                                                  \
  REQUEST_OTR_FEATURE_ENTRIES                                                  \
  HNS_MODULE_FILENAME_PATCH                                                  \
  HNS_FEDERATED_FEATURE_ENTRIES                                              \
  PLAYLIST_FEATURE_ENTRIES                                                     \
  HNS_COMMANDS_FEATURE_ENTRIES                                               \
  HNS_VERTICAL_TABS_FEATURE_ENTRY                                            \
  HNS_VERTICAL_TABS_STICKY_PINNED_TABS_FEATURE_ENTRY                         \
  HNS_BACKGROUND_VIDEO_PLAYBACK_ANDROID                                      \
  HNS_SAFE_BROWSING_ANDROID                                                  \
  HNS_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES                      \
  HNS_SHARED_PINNED_TABS                                                     \
  HNS_AI_CHAT                                                                \
  LAST_HNS_FEATURE_ENTRIES_ITEM  // Keep it as the last item.
namespace flags_ui {
namespace {

// Unused function to reference Hns feature entries for clang checks.
[[maybe_unused]] void UseHnsAboutFlags() {
  // These vars are declared in anonymous namespace in
  // //chrome/browser/about_flags.cc. We declare them here manually to
  // instantiate HNS_ABOUT_FLAGS_FEATURE_ENTRIES without errors.
  constexpr int kOsAll = 0;
  constexpr int kOsDesktop = 0;

  static_assert(
      std::initializer_list<FeatureEntry>{HNS_ABOUT_FLAGS_FEATURE_ENTRIES}
          .size());
}

// Called to skip feature entries on hns://flags page without affecting
// features state.
bool HnsShouldSkipConditionalFeatureEntry(
    const flags_ui::FlagsStorage* storage,
    const FeatureEntry& entry) {
#if BUILDFLAG(ENABLE_HNS_VPN_WIREGUARD) && BUILDFLAG(IS_WIN)
  if (base::EqualsCaseInsensitiveASCII(kHnsVPNWireguardFeatureInternalName,
                                       entry.internal_name)) {
    return true;
  }
#endif
  return false;
}

}  // namespace
}  // namespace flags_ui
