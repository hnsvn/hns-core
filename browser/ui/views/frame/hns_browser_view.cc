/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/frame/hns_browser_view.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "base/containers/contains.h"
#include "base/feature_list.h"
#include "base/functional/bind.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/sparkle_buildflags.h"
#include "hns/browser/translate/hns_translate_utils.h"
#include "hns/browser/ui/hns_browser.h"
#include "hns/browser/ui/hns_rewards/rewards_panel_coordinator.h"
#include "hns/browser/ui/hns_rewards/tip_panel_coordinator.h"
#include "hns/browser/ui/commands/accelerator_service.h"
#include "hns/browser/ui/commands/accelerator_service_factory.h"
#include "hns/browser/ui/sidebar/sidebar_utils.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/hns_actions/hns_actions_container.h"
#include "hns/browser/ui/views/hns_actions/hns_shields_action_view.h"
#include "hns/browser/ui/views/hns_rewards/tip_panel_bubble_host.h"
#include "hns/browser/ui/views/hns_shields/cookie_list_opt_in_bubble_host.h"
#include "hns/browser/ui/views/frame/hns_contents_layout_manager.h"
#include "hns/browser/ui/views/frame/vertical_tab_strip_region_view.h"
#include "hns/browser/ui/views/frame/vertical_tab_strip_widget_delegate_view.h"
#include "hns/browser/ui/views/location_bar/hns_location_bar_view.h"
#include "hns/browser/ui/views/omnibox/hns_omnibox_view_views.h"
#include "hns/browser/ui/views/sidebar/sidebar_container_view.h"
#include "hns/browser/ui/views/speedreader/reader_mode_toolbar_view.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"
#include "hns/browser/ui/views/toolbar/bookmark_button.h"
#include "hns/browser/ui/views/toolbar/hns_toolbar_view.h"
#include "hns/browser/ui/views/toolbar/wallet_button.h"
#include "hns/browser/ui/views/window_closing_confirm_dialog_view.h"
#include "hns/components/commands/common/features.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "chrome/browser/app_mode/app_mode_utils.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/browser/ui/frame/window_frame_util.h"
#include "chrome/browser/ui/views/frame/contents_layout_manager.h"
#include "chrome/browser/ui/views/frame/tab_strip_region_view.h"
#include "chrome/browser/ui/views/frame/top_container_view.h"
#include "chrome/browser/ui/views/side_panel/side_panel_coordinator.h"
#include "chrome/browser/ui/views/tabs/tab_search_button.h"
#include "chrome/browser/ui/views/toolbar/browser_app_menu_button.h"
#include "chrome/common/pref_names.h"
#include "extensions/buildflags/buildflags.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/accelerators/accelerator_manager.h"
#include "ui/events/event_observer.h"
#include "ui/views/bubble/bubble_dialog_delegate_view.h"
#include "ui/views/event_monitor.h"
#include "ui/views/layout/fill_layout.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/ui/views/toolbar/hns_vpn_button.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_SPARKLE)
#include "hns/browser/ui/views/update_recommended_message_box_mac.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/browser/speedreader/speedreader_tab_helper.h"
#include "hns/browser/ui/views/speedreader/reader_mode_bubble.h"
#endif

namespace {
absl::optional<bool> g_download_confirm_return_allow_for_testing;

bool IsUnsupported(int command_id, Browser* browser) {
  return chrome::IsRunningInForcedAppMode() &&
         !chrome::IsCommandAllowedInAppMode(command_id,
                                            browser->is_type_popup());
}

}  // namespace

// static
void HnsBrowserView::SetDownloadConfirmReturnForTesting(bool allow) {
  g_download_confirm_return_allow_for_testing = allow;
}

class HnsBrowserView::TabCyclingEventHandler : public ui::EventObserver,
                                                 public views::WidgetObserver {
 public:
  explicit TabCyclingEventHandler(HnsBrowserView* browser_view)
      : browser_view_(browser_view) {
    Start();
  }

  ~TabCyclingEventHandler() override { Stop(); }

  TabCyclingEventHandler(const TabCyclingEventHandler&) = delete;
  TabCyclingEventHandler& operator=(const TabCyclingEventHandler&) = delete;

 private:
  // ui::EventObserver overrides:
  void OnEvent(const ui::Event& event) override {
    if (event.type() == ui::ET_KEY_RELEASED &&
        event.AsKeyEvent()->key_code() == ui::VKEY_CONTROL) {
      // Ctrl key was released, stop the tab cycling
      Stop();
      return;
    }

    if (event.type() == ui::ET_MOUSE_PRESSED) {
      Stop();
    }
  }

  // views::WidgetObserver overrides:
  void OnWidgetActivationChanged(views::Widget* widget, bool active) override {
    // We should stop cycling if other application gets active state.
    if (!active) {
      Stop();
    }
  }

  // Handle Browser widget closing while tab Cycling is in-progress.
  void OnWidgetClosing(views::Widget* widget) override { Stop(); }

  void Start() {
    // Add the event handler
    auto* widget = browser_view_->GetWidget();
    if (widget->GetNativeWindow()) {
      monitor_ = views::EventMonitor::CreateWindowMonitor(
          this, widget->GetNativeWindow(),
          {ui::ET_MOUSE_PRESSED, ui::ET_KEY_RELEASED});
    }

    widget->AddObserver(this);
  }

  void Stop() {
    if (!monitor_.get()) {
      // We already stopped
      return;
    }

    // Remove event handler
    auto* widget = browser_view_->GetWidget();
    monitor_.reset();
    widget->RemoveObserver(this);
    browser_view_->StopTabCycling();
  }

  raw_ptr<HnsBrowserView> browser_view_ = nullptr;
  std::unique_ptr<views::EventMonitor> monitor_;
};

HnsBrowserView::HnsBrowserView(std::unique_ptr<Browser> browser)
    : BrowserView(std::move(browser)) {
  pref_change_registrar_.Init(GetProfile()->GetPrefs());
  if (!WindowFrameUtil::IsWindowsTabSearchCaptionButtonEnabled(
          browser_.get())) {
    pref_change_registrar_.Add(
        kTabsSearchShow,
        base::BindRepeating(&HnsBrowserView::OnPreferenceChanged,
                            base::Unretained(this)));
    // Show the correct value in settings on initial start
    UpdateSearchTabsButtonState();
  }

  auto* rewards_service =
      hns_rewards::RewardsServiceFactory::GetForProfile(browser_->profile());
  if (rewards_service) {
    hns_rewards::RewardsPanelCoordinator::CreateForBrowser(browser_.get());
    hns_rewards::TipPanelCoordinator::CreateForBrowser(browser_.get(),
                                                         rewards_service);
  }

  hns_rewards::TipPanelBubbleHost::MaybeCreateForBrowser(browser_.get());

  hns_shields::CookieListOptInBubbleHost::MaybeCreateForBrowser(
      browser_.get());

#if BUILDFLAG(ENABLE_HNS_VPN)
  pref_change_registrar_.Add(
      hns_vpn::prefs::kHnsVPNShowButton,
      base::BindRepeating(&HnsBrowserView::OnPreferenceChanged,
                          base::Unretained(this)));
#endif

  const bool supports_vertical_tabs =
      base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs) &&
      tabs::utils::SupportsVerticalTabs(browser_.get());
  if (supports_vertical_tabs) {
    vertical_tab_strip_host_view_ =
        AddChildView(std::make_unique<views::View>());
  }

  // Only normal window (tabbed) should have sidebar.
  const bool can_have_sidebar = sidebar::CanUseSidebar(browser_.get());
  if (can_have_sidebar) {
    // Wrap chromium side panel with our sidebar container
    auto original_side_panel = RemoveChildViewT(unified_side_panel_.get());
    sidebar_container_view_ = contents_container_->AddChildView(
        std::make_unique<SidebarContainerView>(
            GetHnsBrowser(),
            SidePanelUtil::GetSidePanelCoordinatorForBrowser(browser_.get()),
            std::move(original_side_panel)));
    unified_side_panel_ = sidebar_container_view_->side_panel();
    contents_container_->SetLayoutManager(
        std::make_unique<HnsContentsLayoutManager>(
            devtools_web_view_, contents_web_view_, sidebar_container_view_));

#if defined(USE_AURA)
    sidebar_host_view_ = AddChildView(std::make_unique<views::View>());
#endif

    pref_change_registrar_.Add(
        prefs::kSidePanelHorizontalAlignment,
        base::BindRepeating(&HnsBrowserView::OnPreferenceChanged,
                            base::Unretained(this)));
  }

  if (!supports_vertical_tabs && !can_have_sidebar) {
    return;
  }

  // Make sure |find_bar_host_view_| is the last child of BrowserView by
  // re-ordering. FindBarHost widgets uses this view as a  kHostViewKey.
  // See the comments of BrowserView::find_bar_host_view().
  ReorderChildView(find_bar_host_view_, -1);
}

void HnsBrowserView::OnPreferenceChanged(const std::string& pref_name) {
  if (pref_name == kTabsSearchShow) {
    UpdateSearchTabsButtonState();
    return;
  }

  if (pref_name == prefs::kSidePanelHorizontalAlignment) {
    UpdateSideBarHorizontalAlignment();
    return;
  }

#if BUILDFLAG(ENABLE_HNS_VPN)
  if (pref_name == hns_vpn::prefs::kHnsVPNShowButton) {
    vpn_panel_controller_.ResetBubbleManager();
    return;
  }
#endif
}

void HnsBrowserView::UpdateSideBarHorizontalAlignment() {
  DCHECK(sidebar_container_view_);

  const bool on_left = !GetProfile()->GetPrefs()->GetBoolean(
      prefs::kSidePanelHorizontalAlignment);

  sidebar_container_view_->SetSidebarOnLeft(on_left);
  static_cast<HnsContentsLayoutManager*>(GetContentsLayoutManager())
      ->set_sidebar_on_left(on_left);

  contents_container_->Layout();
}

void HnsBrowserView::UpdateSearchTabsButtonState() {
  if (auto* button = tab_strip_region_view()->tab_search_button()) {
    if (button) {
      auto is_tab_search_visible =
          GetProfile()->GetPrefs()->GetBoolean(kTabsSearchShow);
      button->SetVisible(is_tab_search_visible);
    }
  }
}

HnsBrowserView::~HnsBrowserView() {
  tab_cycling_event_handler_.reset();
  DCHECK(!tab_cycling_event_handler_);
}

sidebar::Sidebar* HnsBrowserView::InitSidebar() {
  // Start Sidebar UI initialization.
  DCHECK(sidebar_container_view_);
  sidebar_container_view_->Init();
  UpdateSideBarHorizontalAlignment();
  return sidebar_container_view_;
}

void HnsBrowserView::ToggleSidebar() {
  SidePanelUI::GetSidePanelUIForBrowser(browser_.get())->Toggle();
}

void HnsBrowserView::ShowHnsVPNBubble() {
#if BUILDFLAG(ENABLE_HNS_VPN)
  vpn_panel_controller_.ShowHnsVPNPanel();
#endif
}

views::View* HnsBrowserView::GetAnchorViewForHnsVPNPanel() {
#if BUILDFLAG(ENABLE_HNS_VPN)
  auto* vpn_button =
      static_cast<HnsToolbarView*>(toolbar())->hns_vpn_button();
  if (vpn_button->GetVisible()) {
    return vpn_button;
  }
  return toolbar()->app_menu_button();
#else
  return nullptr;
#endif
}

gfx::Rect HnsBrowserView::GetShieldsBubbleRect() {
  auto* hns_location_bar_view =
      static_cast<HnsLocationBarView*>(GetLocationBarView());
  if (!hns_location_bar_view) {
    return gfx::Rect();
  }

  auto* shields_action_view =
      hns_location_bar_view->hns_actions_contatiner_view()
          ->GetShieldsActionView();
  if (!shields_action_view) {
    return gfx::Rect();
  }

  auto* bubble_widget = shields_action_view->GetBubbleWidget();
  if (!bubble_widget) {
    return gfx::Rect();
  }

  return bubble_widget->GetClientAreaBoundsInScreen();
}

bool HnsBrowserView::GetTabStripVisible() const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    return BrowserView::GetTabStripVisible();
  }

  if (tabs::utils::ShouldShowVerticalTabs(browser())) {
    return false;
  }

  return BrowserView::GetTabStripVisible();
}

#if BUILDFLAG(IS_WIN)
bool HnsBrowserView::GetSupportsTitle() const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    return BrowserView::GetSupportsTitle();
  }

  if (tabs::utils::SupportsVerticalTabs(browser())) {
    return true;
  }

  return BrowserView::GetSupportsTitle();
}
#endif

void HnsBrowserView::SetStarredState(bool is_starred) {
  BookmarkButton* button =
      static_cast<HnsToolbarView*>(toolbar())->bookmark_button();
  if (button) {
    button->SetToggled(is_starred);
  }
}

#if BUILDFLAG(ENABLE_SPEEDREADER)

speedreader::SpeedreaderBubbleView* HnsBrowserView::ShowSpeedreaderBubble(
    speedreader::SpeedreaderTabHelper* tab_helper,
    speedreader::SpeedreaderBubbleLocation location) {
  views::View* anchor = nullptr;
  views::BubbleBorder::Arrow arrow = views::BubbleBorder::NONE;
  switch (location) {
    case speedreader::SpeedreaderBubbleLocation::kLocationBar:
      anchor = GetLocationBarView();
      arrow = views::BubbleBorder::TOP_RIGHT;
      break;
    case speedreader::SpeedreaderBubbleLocation::kToolbar:
      anchor = reader_mode_toolbar_view_->toolbar();
      arrow = views::BubbleBorder::TOP_LEFT;
      break;
  }

  auto* reader_mode_bubble =
      new speedreader::ReaderModeBubble(anchor, tab_helper);
  views::BubbleDialogDelegateView::CreateBubble(reader_mode_bubble);
  reader_mode_bubble->SetArrow(arrow);
  reader_mode_bubble->Show();
  return reader_mode_bubble;
}

void HnsBrowserView::ShowReaderModeToolbar() {
  if (!reader_mode_toolbar_view_) {
    reader_mode_toolbar_view_ =
        std::make_unique<ReaderModeToolbarView>(GetProfile());
    contents_web_view()->parent()->AddChildView(
        reader_mode_toolbar_view_.get());
    static_cast<HnsContentsLayoutManager*>(GetContentsLayoutManager())
        ->set_reader_mode_toolbar(reader_mode_toolbar_view_.get());
  } else {
    reader_mode_toolbar_view_->SetVisible(true);
  }

  Layout();
}

void HnsBrowserView::HideReaderModeToolbar() {
  if (reader_mode_toolbar_view_ && reader_mode_toolbar_view_->GetVisible()) {
    reader_mode_toolbar_view_->SetVisible(false);
    Layout();
  }
}
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)

void HnsBrowserView::ShowUpdateChromeDialog() {
#if BUILDFLAG(ENABLE_SPARKLE)
  // On mac, sparkle frameworks's relaunch api is used.
  UpdateRecommendedMessageBoxMac::Show(GetNativeWindow());
#else
  BrowserView::ShowUpdateChromeDialog();
#endif
}

bool HnsBrowserView::HasSelectedURL() const {
  if (!GetLocationBarView() || !GetLocationBarView()->HasFocus()) {
    return false;
  }
  auto* hns_omnibox_view =
      static_cast<HnsOmniboxViewViews*>(GetLocationBarView()->omnibox_view());
  return hns_omnibox_view && hns_omnibox_view->SelectedTextIsURL();
}

void HnsBrowserView::CleanAndCopySelectedURL() {
  if (!GetLocationBarView()) {
    return;
  }
  auto* hns_omnibox_view =
      static_cast<HnsOmniboxViewViews*>(GetLocationBarView()->omnibox_view());
  if (!hns_omnibox_view) {
    return;
  }
  hns_omnibox_view->CleanAndCopySelectedURL();
}

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
void HnsBrowserView::ShowPlaylistBubble() {
  static_cast<HnsLocationBarView*>(GetLocationBarView())
      ->ShowPlaylistBubble();
}
#endif

WalletButton* HnsBrowserView::GetWalletButton() {
  return static_cast<HnsToolbarView*>(toolbar())->wallet_button();
}

views::View* HnsBrowserView::GetWalletButtonAnchorView() {
  return static_cast<HnsToolbarView*>(toolbar())
      ->wallet_button()
      ->GetAsAnchorView();
}

void HnsBrowserView::OnAcceleratorsChanged(
    const commands::Accelerators& changed) {
  DCHECK(base::FeatureList::IsEnabled(commands::features::kHnsCommands));

  auto* focus_manager = GetFocusManager();
  DCHECK(focus_manager);

  for (const auto& [command_id, accelerators] : changed) {
    if (IsUnsupported(command_id, browser())) {
      continue;
    }

    std::vector<ui::Accelerator> old_accelerators;
    for (const auto& [accelerator, accelerator_command] : accelerator_table_) {
      if (accelerator_command != command_id) {
        continue;
      }
      old_accelerators.push_back(accelerator);
    }

    // Register current accelerators
    for (const auto& accelerator : accelerators) {
      if (focus_manager->IsAcceleratorRegistered(accelerator)) {
        focus_manager->UnregisterAccelerator(accelerator, this);
      }

      focus_manager->RegisterAccelerator(
          accelerator, ui::AcceleratorManager::kNormalPriority, this);
      accelerator_table_[accelerator] = command_id;
    }

    // Unregister removed accelerators
    for (const auto& old_accelerator : old_accelerators) {
      if (base::Contains(accelerators, old_accelerator)) {
        continue;
      }
      focus_manager->UnregisterAccelerator(old_accelerator, this);
      accelerator_table_.erase(old_accelerator);
    }
  }
}

void HnsBrowserView::CreateWalletBubble() {
  DCHECK(GetWalletButton());
  GetWalletButton()->ShowWalletBubble();
}

void HnsBrowserView::CreateApproveWalletBubble() {
  DCHECK(GetWalletButton());
  GetWalletButton()->ShowApproveWalletBubble();
}

void HnsBrowserView::CloseWalletBubble() {
  if (GetWalletButton()) {
    GetWalletButton()->CloseWalletBubble();
  }
}

void HnsBrowserView::AddedToWidget() {
  BrowserView::AddedToWidget();

  if (vertical_tab_strip_host_view_) {
    vertical_tab_strip_widget_delegate_view_ =
        VerticalTabStripWidgetDelegateView::Create(
            this, vertical_tab_strip_host_view_);

    // By setting this property to the widget for vertical tabs,
    // BrowserView::GetBrowserViewForNativeWindow() will return browser view
    // properly even when we pass the native window for vertical tab strip.
    // As a result, we don't have to call GetTopLevelWidget() in order to
    // get browser view from the vertical tab strip's widget.
    SetNativeWindowPropertyForWidget(
        vertical_tab_strip_widget_delegate_view_->GetWidget());

    GetBrowserViewLayout()->set_vertical_tab_strip_host(
        vertical_tab_strip_host_view_.get());
  }
}

void HnsBrowserView::LoadAccelerators() {
  if (base::FeatureList::IsEnabled(commands::features::kHnsCommands)) {
    auto* accelerator_service =
        commands::AcceleratorServiceFactory::GetForContext(
            browser()->profile());
    accelerators_observation_.Observe(accelerator_service);
  } else {
    BrowserView::LoadAccelerators();
  }
}

void HnsBrowserView::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  BrowserView::OnTabStripModelChanged(tab_strip_model, change, selection);

  if (change.type() != TabStripModelChange::kSelectionOnly) {
    // Stop tab cycling if tab is closed dusing the cycle.
    // This can happen when tab is closed by shortcut (ex, ctrl + F4).
    // After stopping, current tab cycling, new tab cycling will be started.
    StopTabCycling();
  }
}

views::CloseRequestResult HnsBrowserView::OnWindowCloseRequested() {
  if (GetHnsBrowser()->ShouldAskForBrowserClosingBeforeHandlers()) {
    if (!closing_confirm_dialog_activated_) {
      WindowClosingConfirmDialogView::Show(
          browser(),
          base::BindOnce(&HnsBrowserView::OnWindowClosingConfirmResponse,
                         weak_ptr_.GetWeakPtr()));
      closing_confirm_dialog_activated_ = true;
    }
    return views::CloseRequestResult::kCannotClose;
  }

  return BrowserView::OnWindowCloseRequested();
}

void HnsBrowserView::OnWindowClosingConfirmResponse(bool allowed_to_close) {
  DCHECK(closing_confirm_dialog_activated_);
  closing_confirm_dialog_activated_ = false;

  auto* browser = GetHnsBrowser();
  // Set to Browser instance because Browser instance knows about the result
  // of any warning handlers or beforeunload handlers.
  browser->set_confirmed_to_close(allowed_to_close);
  if (allowed_to_close) {
    // Start close window again as user allowed to close it.
    // Confirm dialog will not be launched for this closing request
    // as we set HnsBrowser::confirmed_to_closed_window_ to true.
    // If user cancels this window closing via additional warnings
    // or beforeunload handler, this dialog will be shown again.
    chrome::CloseWindow(browser);
  }
}

void HnsBrowserView::ConfirmBrowserCloseWithPendingDownloads(
    int download_count,
    Browser::DownloadCloseType dialog_type,
    base::OnceCallback<void(bool)> callback) {
  // Simulate user response.
  if (g_download_confirm_return_allow_for_testing) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE,
        base::BindOnce(std::move(callback),
                       *g_download_confirm_return_allow_for_testing));
    return;
  }
  BrowserView::ConfirmBrowserCloseWithPendingDownloads(
      download_count, dialog_type, std::move(callback));
}

void HnsBrowserView::MaybeShowReadingListInSidePanelIPH() {
  // Do nothing.
}

void HnsBrowserView::OnWidgetActivationChanged(views::Widget* widget,
                                                 bool active) {
  BrowserView::OnWidgetActivationChanged(widget, active);

  // For updating sidebar's item state.
  // As we can activate other window's Talk tab with current window's sidebar
  // Talk item, sidebar Talk item should have activated state if other windows
  // have Talk tab. It would be complex to get updated when Talk tab is opened
  // from other windows. So, simply trying to update when window activation
  // state is changed. With this, active window could have correct sidebar item
  // state.
  if (sidebar_container_view_) {
    sidebar_container_view_->UpdateSidebarItemsState();
  }
}

bool HnsBrowserView::ShouldShowWindowTitle() const {
  if (BrowserView::ShouldShowWindowTitle()) {
    return true;
  }

  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    return false;
  }

  if (tabs::utils::ShouldShowWindowTitleForVerticalTabs(browser())) {
    return true;
  }

  return false;
}

void HnsBrowserView::OnThemeChanged() {
  BrowserView::OnThemeChanged();
  if (vertical_tab_strip_host_view_) {
    const auto background_color = GetColorProvider()->GetColor(kColorToolbar);
    vertical_tab_strip_host_view_->SetBackground(
        views::CreateSolidBackground(background_color));
  }
}

TabSearchBubbleHost* HnsBrowserView::GetTabSearchBubbleHost() {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs) ||
      !tabs::utils::ShouldShowVerticalTabs(browser()) ||
      WindowFrameUtil::IsWindowsTabSearchCaptionButtonEnabled(browser())) {
    return BrowserView::GetTabSearchBubbleHost();
  }

  return vertical_tab_strip_widget_delegate_view_
      ->vertical_tab_strip_region_view()
      ->GetTabSearchBubbleHost();
}

bool HnsBrowserView::IsSidebarVisible() const {
  return sidebar_container_view_ && sidebar_container_view_->IsSidebarVisible();
}

HnsBrowser* HnsBrowserView::GetHnsBrowser() const {
  return static_cast<HnsBrowser*>(browser_.get());
}

void HnsBrowserView::StartTabCycling() {
  tab_cycling_event_handler_ = std::make_unique<TabCyclingEventHandler>(this);
}

void HnsBrowserView::StopTabCycling() {
  tab_cycling_event_handler_.reset();
  static_cast<HnsTabStripModel*>(browser()->tab_strip_model())
      ->StopMRUCycling();
}
