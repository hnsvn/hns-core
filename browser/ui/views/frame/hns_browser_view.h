/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_VIEW_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "hns/browser/ui/commands/accelerator_service.h"
#include "hns/browser/ui/tabs/hns_tab_strip_model.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/commands/browser/accelerator_pref_manager.h"
#include "build/build_config.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/base/accelerators/accelerator.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/ui/views/toolbar/hns_vpn_panel_controller.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/browser/ui/views/speedreader/reader_mode_toolbar_view.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
namespace speedreader {
class SpeedreaderBubbleView;
class SpeedreaderTabHelper;
enum class SpeedreaderBubbleLocation : int;
}  // namespace speedreader
#endif

namespace content {
class WebContents;
}  // namespace content

namespace sidebar {
class SidebarBrowserTest;
}  // namespace sidebar

class HnsBrowser;
class ContentsLayoutManager;
class SidebarContainerView;
class WalletButton;
class VerticalTabStripWidgetDelegateView;

class HnsBrowserView : public BrowserView,
                         public commands::AcceleratorService::Observer {
 public:
  explicit HnsBrowserView(std::unique_ptr<Browser> browser);
  HnsBrowserView(const HnsBrowserView&) = delete;
  HnsBrowserView& operator=(const HnsBrowserView&) = delete;
  ~HnsBrowserView() override;

  void SetStarredState(bool is_starred) override;
  void ShowUpdateChromeDialog() override;
  void CreateWalletBubble();
  void CreateApproveWalletBubble();
  void CloseWalletBubble();
  WalletButton* GetWalletButton();
  views::View* GetWalletButtonAnchorView();

  // BrowserView overrides:
  void StartTabCycling() override;
  views::View* GetAnchorViewForHnsVPNPanel();
  gfx::Rect GetShieldsBubbleRect() override;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderBubbleView* ShowSpeedreaderBubble(
      speedreader::SpeedreaderTabHelper* tab_helper,
      speedreader::SpeedreaderBubbleLocation location) override;
  void ShowReaderModeToolbar() override;
  void HideReaderModeToolbar() override;
#endif
  bool GetTabStripVisible() const override;
#if BUILDFLAG(IS_WIN)
  bool GetSupportsTitle() const override;
#endif
  bool ShouldShowWindowTitle() const override;
  void OnThemeChanged() override;
  TabSearchBubbleHost* GetTabSearchBubbleHost() override;

#if defined(USE_AURA)
  views::View* sidebar_host_view() { return sidebar_host_view_; }
#endif

  bool IsSidebarVisible() const;

  VerticalTabStripWidgetDelegateView*
  vertical_tab_strip_widget_delegate_view() {
    return vertical_tab_strip_widget_delegate_view_;
  }

  // commands::AcceleratorService:
  void OnAcceleratorsChanged(const commands::Accelerators& changed) override;

 private:
  class TabCyclingEventHandler;
  friend class WindowClosingConfirmBrowserTest;
  friend class sidebar::SidebarBrowserTest;
  friend class VerticalTabStripDragAndDropBrowserTest;

  FRIEND_TEST_ALL_PREFIXES(VerticalTabStripBrowserTest, VisualState);
  FRIEND_TEST_ALL_PREFIXES(VerticalTabStripBrowserTest, Fullscreen);
  FRIEND_TEST_ALL_PREFIXES(VerticalTabStripDragAndDropBrowserTest,
                           DragTabToReorder);
  FRIEND_TEST_ALL_PREFIXES(SpeedReaderBrowserTest, Toolbar);

  static void SetDownloadConfirmReturnForTesting(bool allow);

  // BrowserView overrides:
  void AddedToWidget() override;
  void LoadAccelerators() override;
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;
  void ShowHnsVPNBubble() override;
  views::CloseRequestResult OnWindowCloseRequested() override;
  void ConfirmBrowserCloseWithPendingDownloads(
      int download_count,
      Browser::DownloadCloseType dialog_type,
      base::OnceCallback<void(bool)> callback) override;
  void MaybeShowReadingListInSidePanelIPH() override;
  void OnWidgetActivationChanged(views::Widget* widget, bool active) override;

  void StopTabCycling();
  void UpdateSearchTabsButtonState();
  void OnPreferenceChanged(const std::string& pref_name);
  void OnWindowClosingConfirmResponse(bool allowed_to_close);
  HnsBrowser* GetHnsBrowser() const;

  sidebar::Sidebar* InitSidebar() override;
  void ToggleSidebar() override;
  bool HasSelectedURL() const override;
  void CleanAndCopySelectedURL() override;

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
  void ShowPlaylistBubble() override;
#endif

  void UpdateSideBarHorizontalAlignment();

  bool closing_confirm_dialog_activated_ = false;
  raw_ptr<SidebarContainerView> sidebar_container_view_ = nullptr;
  raw_ptr<views::View> vertical_tab_strip_host_view_ = nullptr;
  raw_ptr<VerticalTabStripWidgetDelegateView>
      vertical_tab_strip_widget_delegate_view_ = nullptr;

#if defined(USE_AURA)
  raw_ptr<views::View> sidebar_host_view_ = nullptr;
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
  HnsVPNPanelController vpn_panel_controller_{this};
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
  std::unique_ptr<ReaderModeToolbarView> reader_mode_toolbar_view_;
#endif

  std::unique_ptr<TabCyclingEventHandler> tab_cycling_event_handler_;
  PrefChangeRegistrar pref_change_registrar_;
  base::ScopedObservation<commands::AcceleratorService,
                          commands::AcceleratorService::Observer>
      accelerators_observation_{this};

  base::WeakPtrFactory<HnsBrowserView> weak_ptr_{this};
};

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_VIEW_H_
