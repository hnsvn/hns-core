/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_browser_command_controller.h"

#include <utility>
#include <vector>

#include "base/containers/contains.h"
#include "base/feature_list.h"
#include "base/notreached.h"
#include "hns/app/hns_command_ids.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/ui/hns_pages.h"
#include "hns/browser/ui/browser_commands.h"
#include "hns/browser/ui/sidebar/sidebar_utils.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/components/hns_rewards/common/rewards_util.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/commands/common/features.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/playlist/common/buildflags/buildflags.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "components/sync/base/command_line_switches.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/browser/hns_vpn/vpn_utils.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/components/speedreader/common/features.h"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
#include "hns/components/playlist/common/features.h"
#endif

namespace {

bool IsHnsCommands(int id) {
  return id >= IDC_HNS_COMMANDS_START && id <= IDC_HNS_COMMANDS_LAST;
}

bool IsHnsOverrideCommands(int id) {
  static std::vector<int> override_commands({
      IDC_NEW_WINDOW,
      IDC_NEW_INCOGNITO_WINDOW,
  });
  return base::Contains(override_commands, id);
}

}  // namespace

namespace chrome {

HnsBrowserCommandController::HnsBrowserCommandController(Browser* browser)
    : BrowserCommandController(browser),
      browser_(*browser),
      hns_command_updater_(nullptr) {
  InitHnsCommandState();
#if BUILDFLAG(ENABLE_HNS_VPN)
  if (auto* vpn_service = hns_vpn::HnsVpnServiceFactory::GetForProfile(
          browser_->profile())) {
    Observe(vpn_service);
  }
#endif
}

HnsBrowserCommandController::~HnsBrowserCommandController() = default;

bool HnsBrowserCommandController::SupportsCommand(int id) const {
  return IsHnsCommands(id) ? hns_command_updater_.SupportsCommand(id)
                             : BrowserCommandController::SupportsCommand(id);
}

bool HnsBrowserCommandController::IsCommandEnabled(int id) const {
  return IsHnsCommands(id) ? hns_command_updater_.IsCommandEnabled(id)
                             : BrowserCommandController::IsCommandEnabled(id);
}

bool HnsBrowserCommandController::ExecuteCommandWithDisposition(
    int id,
    WindowOpenDisposition disposition,
    base::TimeTicks time_stamp) {
  return IsHnsCommands(id) || IsHnsOverrideCommands(id)
             ? ExecuteHnsCommandWithDisposition(id, disposition, time_stamp)
             : BrowserCommandController::ExecuteCommandWithDisposition(
                   id, disposition, time_stamp);
}

void HnsBrowserCommandController::AddCommandObserver(
    int id,
    CommandObserver* observer) {
  IsHnsCommands(id)
      ? hns_command_updater_.AddCommandObserver(id, observer)
      : BrowserCommandController::AddCommandObserver(id, observer);
}

void HnsBrowserCommandController::RemoveCommandObserver(
    int id,
    CommandObserver* observer) {
  IsHnsCommands(id)
      ? hns_command_updater_.RemoveCommandObserver(id, observer)
      : BrowserCommandController::RemoveCommandObserver(id, observer);
}

void HnsBrowserCommandController::RemoveCommandObserver(
    CommandObserver* observer) {
  hns_command_updater_.RemoveCommandObserver(observer);
  BrowserCommandController::RemoveCommandObserver(observer);
}

bool HnsBrowserCommandController::UpdateCommandEnabled(int id, bool state) {
  return IsHnsCommands(id)
             ? hns_command_updater_.UpdateCommandEnabled(id, state)
             : BrowserCommandController::UpdateCommandEnabled(id, state);
}

void HnsBrowserCommandController::InitHnsCommandState() {
  // Sync, Rewards, and Wallet pages don't work in tor(guest) sessions.
  // They also don't work in private windows but they are redirected
  // to a normal window in this case.
  const bool is_guest_session = browser_->profile()->IsGuestSession();
  if (!is_guest_session) {
    // If Rewards is not supported due to OFAC sanctions we still want to show
    // the menu item.
    if (hns_rewards::IsSupported(browser_->profile()->GetPrefs())) {
      UpdateCommandForHnsRewards();
    }
    if (hns_wallet::IsAllowed(browser_->profile()->GetPrefs())) {
      UpdateCommandForHnsWallet();
    }
    if (syncer::IsSyncAllowedByFlag()) {
      UpdateCommandForHnsSync();
    }
  }
  UpdateCommandForWebcompatReporter();
#if BUILDFLAG(ENABLE_TOR)
  UpdateCommandForTor();
#endif
  UpdateCommandForSidebar();
  UpdateCommandForHnsVPN();
  UpdateCommandForPlaylist();
#if BUILDFLAG(ENABLE_HNS_VPN)
  if (hns_vpn::IsAllowedForContext(browser_->profile())) {
    hns_vpn_pref_change_registrar_.Init(browser_->profile()->GetPrefs());
    hns_vpn_pref_change_registrar_.Add(
        hns_vpn::prefs::kManagedHnsVPNDisabled,
        base::BindRepeating(
            &HnsBrowserCommandController::UpdateCommandForHnsVPN,
            base::Unretained(this)));
  }
#endif
  bool add_new_profile_enabled = !is_guest_session;
  bool open_guest_profile_enabled = !is_guest_session;
  if (!is_guest_session) {
    if (PrefService* local_state = g_browser_process->local_state()) {
      add_new_profile_enabled =
          local_state->GetBoolean(prefs::kBrowserAddPersonEnabled);
      open_guest_profile_enabled =
          local_state->GetBoolean(prefs::kBrowserGuestModeEnabled);
    }
  }
  UpdateCommandEnabled(IDC_ADD_NEW_PROFILE, add_new_profile_enabled);
  UpdateCommandEnabled(IDC_OPEN_GUEST_PROFILE, open_guest_profile_enabled);
  UpdateCommandEnabled(IDC_COPY_CLEAN_LINK, true);
  UpdateCommandEnabled(IDC_TOGGLE_TAB_MUTE, true);

#if BUILDFLAG(ENABLE_SPEEDREADER)
  if (base::FeatureList::IsEnabled(speedreader::kSpeedreaderFeature)) {
    UpdateCommandEnabled(IDC_SPEEDREADER_ICON_ONCLICK, true);
    UpdateCommandEnabled(IDC_DISTILL_PAGE, false);
  }
#endif
#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
  UpdateCommandEnabled(IDC_APP_MENU_IPFS_OPEN_FILES, true);
#endif
  UpdateCommandEnabled(IDC_HNS_BOOKMARK_BAR_SUBMENU, true);

  UpdateCommandEnabled(
      IDC_TOGGLE_VERTICAL_TABS,
      base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs));
  UpdateCommandEnabled(
      IDC_TOGGLE_VERTICAL_TABS_WINDOW_TITLE,
      base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs));
  UpdateCommandEnabled(
      IDC_TOGGLE_VERTICAL_TABS_EXPANDED,
      base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs));

  UpdateCommandEnabled(IDC_CONFIGURE_HNS_NEWS,
                       !browser_->profile()->IsOffTheRecord());

  UpdateCommandEnabled(
      IDC_CONFIGURE_SHORTCUTS,
      base::FeatureList::IsEnabled(commands::features::kHnsCommands));

  UpdateCommandEnabled(IDC_SHOW_HNS_TALK, true);
  UpdateCommandEnabled(IDC_TOGGLE_SHIELDS, true);
  UpdateCommandEnabled(IDC_TOGGLE_JAVASCRIPT, true);
  UpdateCommandEnabled(IDC_GROUP_TABS_ON_CURRENT_ORIGIN, true);
  UpdateCommandEnabled(IDC_MOVE_GROUP_TO_NEW_WINDOW, true);
  UpdateCommandEnabled(IDC_CLOSE_DUPLICATE_TABS, true);
}

void HnsBrowserCommandController::UpdateCommandForHnsRewards() {
  UpdateCommandEnabled(IDC_SHOW_HNS_REWARDS, true);
}

void HnsBrowserCommandController::UpdateCommandForWebcompatReporter() {
  UpdateCommandEnabled(IDC_SHOW_HNS_WEBCOMPAT_REPORTER, true);
}

#if BUILDFLAG(ENABLE_TOR)
void HnsBrowserCommandController::UpdateCommandForTor() {
  // Enable new tor connection only for tor profile.
  UpdateCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE,
                       browser_->profile()->IsTor());
  UpdateCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR,
                       !hns::IsTorDisabledForProfile(browser_->profile()));
}
#endif

void HnsBrowserCommandController::UpdateCommandForSidebar() {
  if (sidebar::CanUseSidebar(&*browser_)) {
    UpdateCommandEnabled(IDC_SIDEBAR_SHOW_OPTION_MENU, true);
    UpdateCommandEnabled(IDC_SIDEBAR_TOGGLE_POSITION, true);
    UpdateCommandEnabled(IDC_TOGGLE_SIDEBAR, true);
  }
}

void HnsBrowserCommandController::UpdateCommandForHnsVPN() {
#if BUILDFLAG(ENABLE_HNS_VPN)
  if (!hns_vpn::IsHnsVPNEnabled(browser_->profile())) {
    UpdateCommandEnabled(IDC_SHOW_HNS_VPN_PANEL, false);
    UpdateCommandEnabled(IDC_HNS_VPN_MENU, false);
    UpdateCommandEnabled(IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON, false);
    UpdateCommandEnabled(IDC_SEND_HNS_VPN_FEEDBACK, false);
    UpdateCommandEnabled(IDC_ABOUT_HNS_VPN, false);
    UpdateCommandEnabled(IDC_MANAGE_HNS_VPN_PLAN, false);
    UpdateCommandEnabled(IDC_TOGGLE_HNS_VPN, false);
#if BUILDFLAG(IS_WIN)
    UpdateCommandEnabled(IDC_TOGGLE_HNS_VPN_TRAY_ICON, false);
#endif
    return;
  }
  UpdateCommandEnabled(IDC_SHOW_HNS_VPN_PANEL, true);
  UpdateCommandEnabled(IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON, true);
#if BUILDFLAG(IS_WIN)
  UpdateCommandEnabled(IDC_TOGGLE_HNS_VPN_TRAY_ICON, true);
#endif
  UpdateCommandEnabled(IDC_SEND_HNS_VPN_FEEDBACK, true);
  UpdateCommandEnabled(IDC_ABOUT_HNS_VPN, true);
  UpdateCommandEnabled(IDC_MANAGE_HNS_VPN_PLAN, true);

  if (auto* vpn_service = hns_vpn::HnsVpnServiceFactory::GetForProfile(
          browser_->profile())) {
    // Only show vpn sub menu for purchased user.
    UpdateCommandEnabled(IDC_HNS_VPN_MENU, vpn_service->is_purchased_user());
    UpdateCommandEnabled(IDC_TOGGLE_HNS_VPN,
                         vpn_service->is_purchased_user());
  }
#endif
}

void HnsBrowserCommandController::UpdateCommandForPlaylist() {
#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
  if (base::FeatureList::IsEnabled(playlist::features::kPlaylist)) {
    UpdateCommandEnabled(
        IDC_SHOW_PLAYLIST_BUBBLE,
        browser_->is_type_normal() && !browser_->profile()->IsOffTheRecord());
  }
#endif
}

void HnsBrowserCommandController::UpdateCommandForHnsSync() {
  UpdateCommandEnabled(IDC_SHOW_HNS_SYNC, true);
}

void HnsBrowserCommandController::UpdateCommandForHnsWallet() {
  UpdateCommandEnabled(IDC_SHOW_HNS_WALLET, true);
  UpdateCommandEnabled(IDC_SHOW_HNS_WALLET_PANEL, true);
  UpdateCommandEnabled(IDC_CLOSE_HNS_WALLET_PANEL, true);
}

bool HnsBrowserCommandController::ExecuteHnsCommandWithDisposition(
    int id,
    WindowOpenDisposition disposition,
    base::TimeTicks time_stamp) {
  if (!SupportsCommand(id) || !IsCommandEnabled(id)) {
    return false;
  }

  if (browser_->tab_strip_model()->active_index() == TabStripModel::kNoTab) {
    return true;
  }

  DCHECK(IsCommandEnabled(id)) << "Invalid/disabled command " << id;

  switch (id) {
    case IDC_NEW_WINDOW:
      // Use chromium's action for non-Tor profiles.
      if (!browser_->profile()->IsTor()) {
        return BrowserCommandController::ExecuteCommandWithDisposition(
            id, disposition, time_stamp);
      }
      NewEmptyWindow(browser_->profile()->GetOriginalProfile());
      break;
    case IDC_NEW_INCOGNITO_WINDOW:
      // Use chromium's action for non-Tor profiles.
      if (!browser_->profile()->IsTor()) {
        return BrowserCommandController::ExecuteCommandWithDisposition(
            id, disposition, time_stamp);
      }
      NewIncognitoWindow(browser_->profile()->GetOriginalProfile());
      break;
    case IDC_SHOW_HNS_REWARDS:
      hns::ShowHnsRewards(&*browser_);
      break;
    case IDC_SHOW_HNS_WEBCOMPAT_REPORTER:
      hns::ShowWebcompatReporter(&*browser_);
      break;
    case IDC_NEW_OFFTHERECORD_WINDOW_TOR:
      hns::NewOffTheRecordWindowTor(&*browser_);
      break;
    case IDC_NEW_TOR_CONNECTION_FOR_SITE:
      hns::NewTorConnectionForSite(&*browser_);
      break;
    case IDC_SHOW_HNS_SYNC:
      hns::ShowSync(&*browser_);
      break;
    case IDC_SHOW_HNS_WALLET:
      hns::ShowHnsWallet(&*browser_);
      break;
    case IDC_SPEEDREADER_ICON_ONCLICK:
      hns::MaybeDistillAndShowSpeedreaderBubble(&*browser_);
      break;
    case IDC_SHOW_HNS_WALLET_PANEL:
      hns::ShowWalletBubble(&*browser_);
      break;
    case IDC_CLOSE_HNS_WALLET_PANEL:
      hns::CloseWalletBubble(&*browser_);
      break;
    case IDC_SHOW_HNS_VPN_PANEL:
      hns::ShowHnsVPNBubble(&*browser_);
      break;
    case IDC_TOGGLE_HNS_VPN_TRAY_ICON:
      hns::ToggleHnsVPNTrayIcon();
      break;
    case IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON:
      hns::ToggleHnsVPNButton(&*browser_);
      break;
    case IDC_SEND_HNS_VPN_FEEDBACK:
    case IDC_ABOUT_HNS_VPN:
    case IDC_MANAGE_HNS_VPN_PLAN:
      hns::OpenHnsVPNUrls(&*browser_, id);
      break;
    case IDC_SIDEBAR_TOGGLE_POSITION:
      hns::ToggleSidebarPosition(&*browser_);
      break;
    case IDC_TOGGLE_SIDEBAR:
      hns::ToggleSidebar(&*browser_);
      break;
    case IDC_COPY_CLEAN_LINK:
      hns::CopySanitizedURL(
          &*browser_,
          browser_->tab_strip_model()->GetActiveWebContents()->GetVisibleURL());
      break;
    case IDC_APP_MENU_IPFS_OPEN_FILES:
      hns::OpenIpfsFilesWebUI(&*browser_);
      break;
    case IDC_TOGGLE_TAB_MUTE:
      hns::ToggleActiveTabAudioMute(&*browser_);
      break;
    case IDC_TOGGLE_VERTICAL_TABS:
      hns::ToggleVerticalTabStrip(&*browser_);
      break;
    case IDC_TOGGLE_VERTICAL_TABS_WINDOW_TITLE:
      hns::ToggleWindowTitleVisibilityForVerticalTabs(&*browser_);
      break;
    case IDC_TOGGLE_VERTICAL_TABS_EXPANDED:
      hns::ToggleVerticalTabStripExpanded(&*browser_);
      break;
    case IDC_CONFIGURE_HNS_NEWS:
      hns::ShowHnsNewsConfigure(&*browser_);
      break;
    case IDC_CONFIGURE_SHORTCUTS:
      hns::ShowShortcutsPage(&*browser_);
      break;
    case IDC_SHOW_HNS_TALK:
      hns::ShowHnsTalk(&*browser_);
      break;
    case IDC_TOGGLE_SHIELDS:
      hns::ToggleShieldsEnabled(&*browser_);
      break;
    case IDC_TOGGLE_JAVASCRIPT:
      hns::ToggleJavascriptEnabled(&*browser_);
      break;
    case IDC_SHOW_PLAYLIST_BUBBLE:
#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
      hns::ShowPlaylistBubble(&*browser_);
#else
      NOTREACHED() << " This command shouldn't be enabled";
#endif
      break;
    case IDC_GROUP_TABS_ON_CURRENT_ORIGIN:
      hns::GroupTabsOnCurrentOrigin(&*browser_);
      break;
    case IDC_MOVE_GROUP_TO_NEW_WINDOW:
      hns::MoveGroupToNewWindow(&*browser_);
      break;
    case IDC_CLOSE_DUPLICATE_TABS:
      hns::CloseDuplicateTabs(&*browser_);
      break;
    default:
      LOG(WARNING) << "Received Unimplemented Command: " << id;
      break;
  }

  return true;
}

#if BUILDFLAG(ENABLE_HNS_VPN)
void HnsBrowserCommandController::OnPurchasedStateChanged(
    hns_vpn::mojom::PurchasedState state,
    const absl::optional<std::string>& description) {
  UpdateCommandForHnsVPN();
}
#endif

}  // namespace chrome
