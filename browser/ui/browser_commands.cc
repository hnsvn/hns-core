/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/browser_commands.h"

#include <string>
#include <vector>

#include "base/strings/utf_string_conversions.h"
#include "hns/app/hns_command_ids.h"
#include "hns/browser/debounce/debounce_service_factory.h"
#include "hns/browser/ui/hns_shields_data_controller.h"
#include "hns/browser/ui/sidebar/sidebar_service_factory.h"
#include "hns/browser/ui/tabs/hns_tab_prefs.h"
#include "hns/browser/url_sanitizer/url_sanitizer_service_factory.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/debounce/browser/debounce_service.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/query_filter/utils.h"
#include "hns/components/sidebar/sidebar_service.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "hns/components/url_sanitizer/browser/url_sanitizer_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_metrics.h"
#include "chrome/browser/profiles/profile_window.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_tabstrip.h"
#include "chrome/browser/ui/profiles/profile_picker.h"
#include "chrome/browser/ui/tabs/tab_enums.h"
#include "chrome/browser/ui/tabs/tab_group.h"
#include "chrome/browser/ui/tabs/tab_group_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_utils.h"
#include "chrome/common/pref_names.h"
#include "components/tab_groups/tab_group_visual_data.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/web_contents.h"
#include "ui/base/clipboard/clipboard_buffer.h"
#include "ui/base/clipboard/scoped_clipboard_writer.h"
#include "url/origin.h"

#if defined(TOOLKIT_VIEWS)
#include "hns/browser/ui/views/frame/hns_browser_view.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/browser/speedreader/speedreader_service_factory.h"
#include "hns/browser/speedreader/speedreader_tab_helper.h"
#include "hns/components/speedreader/speedreader_service.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/browser/tor/tor_profile_manager.h"
#include "hns/browser/tor/tor_profile_service_factory.h"
#include "hns/components/tor/tor_profile_service.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/hns_vpn/common/hns_vpn_constants.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/pref_names.h"

#if BUILDFLAG(IS_WIN)
#include "hns/components/hns_vpn/common/wireguard/win/storage_utils.h"
#include "hns/components/hns_vpn/common/wireguard/win/wireguard_utils_win.h"
#endif  // BUILDFLAG(ENABLE_HNS_VPN)

#endif  // BUILDFLAG(ENABLE_HNS_VPN)

#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
#include "hns/components/ipfs/ipfs_utils.h"
#include "chrome/common/channel_info.h"
#endif

using content::WebContents;

namespace hns {
void NewOffTheRecordWindowTor(Browser* browser) {
  CHECK(browser);
  if (browser->profile()->IsTor()) {
    chrome::OpenEmptyWindow(browser->profile());
    return;
  }

  TorProfileManager::SwitchToTorProfile(browser->profile(), base::DoNothing());
}

void NewTorConnectionForSite(Browser* browser) {
#if BUILDFLAG(ENABLE_TOR)
  Profile* profile = browser->profile();
  DCHECK(profile);
  tor::TorProfileService* service =
      TorProfileServiceFactory::GetForContext(profile);
  DCHECK(service);
  WebContents* current_tab = browser->tab_strip_model()->GetActiveWebContents();
  if (!current_tab) {
    return;
  }
  service->SetNewTorCircuit(current_tab);
#endif
}

void MaybeDistillAndShowSpeedreaderBubble(Browser* browser) {
#if BUILDFLAG(ENABLE_SPEEDREADER)
  WebContents* contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!contents) {
    return;
  }
  if (auto* tab_helper =
          speedreader::SpeedreaderTabHelper::FromWebContents(contents)) {
    tab_helper->ProcessIconClick();
  }
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)
}

void ShowHnsVPNBubble(Browser* browser) {
  // Ask to browser view.
  static_cast<HnsBrowserWindow*>(browser->window())->ShowHnsVPNBubble();
}

void ToggleHnsVPNTrayIcon() {
#if BUILDFLAG(ENABLE_HNS_VPN) && BUILDFLAG(IS_WIN)
  hns_vpn::EnableVPNTrayIcon(!hns_vpn::IsVPNTrayIconEnabled());
  if (hns_vpn::IsVPNTrayIconEnabled()) {
    hns_vpn::wireguard::ShowHnsVpnStatusTrayIcon();
  }
#endif
}

void ToggleHnsVPNButton(Browser* browser) {
#if BUILDFLAG(ENABLE_HNS_VPN)
  auto* prefs = browser->profile()->GetPrefs();
  const bool show = prefs->GetBoolean(hns_vpn::prefs::kHnsVPNShowButton);
  prefs->SetBoolean(hns_vpn::prefs::kHnsVPNShowButton, !show);
#endif
}

void OpenIpfsFilesWebUI(Browser* browser) {
#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
  auto* prefs = browser->profile()->GetPrefs();
  DCHECK(ipfs::IsLocalGatewayConfigured(prefs));
  GURL gateway = ipfs::GetAPIServer(chrome::GetChannel());
  GURL::Replacements replacements;
  replacements.SetPathStr("/webui/");
  replacements.SetRefStr("/files");
  auto target_url = gateway.ReplaceComponents(replacements);
  chrome::AddTabAt(browser, GURL(target_url), -1, true);
#endif
}

void OpenHnsVPNUrls(Browser* browser, int command_id) {
#if BUILDFLAG(ENABLE_HNS_VPN)
  hns_vpn::HnsVpnService* vpn_service =
      hns_vpn::HnsVpnServiceFactory::GetForProfile(browser->profile());
  std::string target_url;
  switch (command_id) {
    case IDC_SEND_HNS_VPN_FEEDBACK:
      target_url = hns_vpn::kFeedbackUrl;
      break;
    case IDC_ABOUT_HNS_VPN:
      target_url = hns_vpn::kAboutUrl;
      break;
    case IDC_MANAGE_HNS_VPN_PLAN:
      target_url =
          hns_vpn::GetManageUrl(vpn_service->GetCurrentEnvironment());
      break;
    default:
      NOTREACHED();
  }

  chrome::AddTabAt(browser, GURL(target_url), -1, true);
#endif
}

void ShowWalletBubble(Browser* browser) {
#if defined(TOOLKIT_VIEWS)
  static_cast<HnsBrowserView*>(browser->window())->CreateWalletBubble();
#endif
}

void ShowApproveWalletBubble(Browser* browser) {
#if defined(TOOLKIT_VIEWS)
  static_cast<HnsBrowserView*>(browser->window())
      ->CreateApproveWalletBubble();
#endif
}

void CloseWalletBubble(Browser* browser) {
#if defined(TOOLKIT_VIEWS)
  static_cast<HnsBrowserView*>(browser->window())->CloseWalletBubble();
#endif
}

void CopySanitizedURL(Browser* browser, const GURL& url) {
  if (!browser || !browser->profile()) {
    return;
  }
  GURL sanitized_url = hns::URLSanitizerServiceFactory::GetForBrowserContext(
                           browser->profile())
                           ->SanitizeURL(url);

  ui::ScopedClipboardWriter scw(ui::ClipboardBuffer::kCopyPaste);
  scw.WriteText(base::UTF8ToUTF16(sanitized_url.spec()));
}

// Copies an url cleared through:
// - Debouncer (potentially debouncing many levels)
// - Query filter
// - URLSanitizerService
void CopyLinkWithStrictCleaning(Browser* browser, const GURL& url) {
  if (!browser || !browser->profile()) {
    return;
  }
  DCHECK(url.SchemeIsHTTPOrHTTPS());
  GURL final_url;
  // Apply debounce rules.
  auto* debounce_service =
      debounce::DebounceServiceFactory::GetForBrowserContext(
          browser->profile());
  if (debounce_service && !debounce_service->Debounce(url, &final_url)) {
    VLOG(1) << "Unable to apply debounce rules";
    final_url = url;
  }
  // Apply query filters.
  auto filtered_url = query_filter::ApplyQueryFilter(final_url);
  if (filtered_url.has_value()) {
    final_url = filtered_url.value();
  }
  // Sanitize url.
  final_url = hns::URLSanitizerServiceFactory::GetForBrowserContext(
                  browser->profile())
                  ->SanitizeURL(final_url);

  ui::ScopedClipboardWriter scw(ui::ClipboardBuffer::kCopyPaste);
  scw.WriteText(base::UTF8ToUTF16(final_url.spec()));
}

void ToggleWindowTitleVisibilityForVerticalTabs(Browser* browser) {
  auto* prefs = browser->profile()->GetOriginalProfile()->GetPrefs();
  prefs->SetBoolean(
      hns_tabs::kVerticalTabsShowTitleOnWindow,
      !prefs->GetBoolean(hns_tabs::kVerticalTabsShowTitleOnWindow));
}

void ToggleVerticalTabStrip(Browser* browser) {
  auto* profile = browser->profile()->GetOriginalProfile();
  auto* prefs = profile->GetPrefs();
  const bool was_using_vertical_tab_strip =
      prefs->GetBoolean(hns_tabs::kVerticalTabsEnabled);
  prefs->SetBoolean(hns_tabs::kVerticalTabsEnabled,
                    !was_using_vertical_tab_strip);
}

void ToggleVerticalTabStripFloatingMode(Browser* browser) {
  auto* prefs = browser->profile()->GetOriginalProfile()->GetPrefs();
  prefs->SetBoolean(
      hns_tabs::kVerticalTabsFloatingEnabled,
      !prefs->GetBoolean(hns_tabs::kVerticalTabsFloatingEnabled));
}

void ToggleVerticalTabStripExpanded(Browser* browser) {
  auto* prefs = browser->profile()->GetPrefs();
  prefs->SetBoolean(hns_tabs::kVerticalTabsCollapsed,
                    !prefs->GetBoolean(hns_tabs::kVerticalTabsCollapsed));
}

void ToggleActiveTabAudioMute(Browser* browser) {
  WebContents* contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!contents || !contents->IsCurrentlyAudible()) {
    return;
  }

  bool mute_tab = !contents->IsAudioMuted();
  chrome::SetTabAudioMuted(contents, mute_tab, TabMutedReason::AUDIO_INDICATOR,
                           std::string());
}

void ToggleSidebarPosition(Browser* browser) {
  auto* prefs = browser->profile()->GetPrefs();
  prefs->SetBoolean(prefs::kSidePanelHorizontalAlignment,
                    !prefs->GetBoolean(prefs::kSidePanelHorizontalAlignment));
}

void ToggleSidebar(Browser* browser) {
  if (!browser) {
    return;
  }

  if (auto* hns_browser_window =
          HnsBrowserWindow::From(browser->window())) {
    hns_browser_window->ToggleSidebar();
  }
}

bool HasSelectedURL(Browser* browser) {
  if (!browser) {
    return false;
  }
  auto* hns_browser_window = HnsBrowserWindow::From(browser->window());
  return hns_browser_window && hns_browser_window->HasSelectedURL();
}

void CleanAndCopySelectedURL(Browser* browser) {
  if (!browser) {
    return;
  }
  auto* hns_browser_window = HnsBrowserWindow::From(browser->window());
  if (hns_browser_window) {
    hns_browser_window->CleanAndCopySelectedURL();
  }
}

void ToggleShieldsEnabled(Browser* browser) {
  if (!browser) {
    return;
  }

  auto* contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!contents) {
    return;
  }
  auto* shields =
      hns_shields::HnsShieldsDataController::FromWebContents(contents);
  if (!shields) {
    return;
  }

  shields->SetHnsShieldsEnabled(!shields->GetHnsShieldsEnabled());
}

void ToggleJavascriptEnabled(Browser* browser) {
  if (!browser) {
    return;
  }

  auto* contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!contents) {
    return;
  }
  auto* shields =
      hns_shields::HnsShieldsDataController::FromWebContents(contents);
  if (!shields) {
    return;
  }

  shields->SetIsNoScriptEnabled(!shields->GetNoScriptEnabled());
}

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
void ShowPlaylistBubble(Browser* browser) {
  HnsBrowserWindow::From(browser->window())->ShowPlaylistBubble();
}
#endif

void GroupTabsOnCurrentOrigin(Browser* browser) {
  auto url =
      browser->tab_strip_model()->GetActiveWebContents()->GetVisibleURL();
  auto origin = url::Origin::Create(url);

  std::vector<int> group_indices;
  for (int index = 0; index < browser->tab_strip_model()->count(); ++index) {
    auto* tab = browser->tab_strip_model()->GetWebContentsAt(index);
    auto tab_origin = url::Origin::Create(tab->GetVisibleURL());
    if (origin.IsSameOriginWith(tab_origin)) {
      group_indices.push_back(index);
    }
  }
  auto group_id = browser->tab_strip_model()->AddToNewGroup(group_indices);
  auto* group =
      browser->tab_strip_model()->group_model()->GetTabGroup(group_id);

  auto data = *group->visual_data();
  data.SetTitle(base::UTF8ToUTF16(origin.host()));
  group->SetVisualData(data);
}

void MoveGroupToNewWindow(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  auto current_group_id = tsm->GetTabGroupForTab(tsm->active_index());
  if (!current_group_id.has_value()) {
    return;
  }

  tsm->delegate()->MoveGroupToNewWindow(current_group_id.value());
}

void CloseDuplicateTabs(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  auto url = tsm->GetActiveWebContents()->GetVisibleURL();

  for (int i = tsm->GetTabCount() - 1; i >= 0; --i) {
    // Don't close the active tab.
    if (tsm->active_index() == i) {
      continue;
    }

    auto* tab = tsm->GetWebContentsAt(i);
    if (tab->GetVisibleURL() == url) {
      tab->Close();
    }
  }
}

}  // namespace hns
