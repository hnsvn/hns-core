/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/location_bar/hns_location_bar_view.h"

#include <memory>
#include <utility>

#include "base/containers/contains.h"
#include "base/feature_list.h"
#include "hns/app/vector_icons/vector_icons.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/themes/hns_theme_service.h"
#include "hns/browser/ui/color/hns_color_id.h"
#include "hns/browser/ui/views/hns_actions/hns_actions_container.h"
#include "hns/browser/ui/views/location_bar/hns_news_location_view.h"
#include "hns/browser/ui/views/playlist/playlist_action_icon_view.h"
#include "hns/browser/ui/views/toolbar/hns_toolbar_view.h"
#include "hns/components/commander/common/buildflags/buildflags.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_theme_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/omnibox/omnibox_theme.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "chrome/browser/ui/views/omnibox/omnibox_view_views.h"
#include "chrome/browser/ui/views/page_action/page_action_icon_controller.h"
#include "chrome/grit/chromium_strings.h"
#include "components/grit/hns_components_strings.h"
#include "components/omnibox/browser/omnibox_edit_model.h"
#include "components/version_info/channel.h"
#include "content/public/browser/navigation_entry.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/skia_conversions.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/views/animation/ink_drop.h"
#include "ui/views/controls/highlight_path_generator.h"
#include "ui/views/view_utils.h"

#if BUILDFLAG(ENABLE_TOR)
#include "hns/browser/ui/views/location_bar/onion_location_view.h"
#endif
#if BUILDFLAG(ENABLE_IPFS)
#include "hns/browser/ipfs/ipfs_service_factory.h"
#include "hns/browser/ui/views/location_bar/ipfs_location_view.h"
#include "hns/components/ipfs/ipfs_constants.h"
#include "hns/components/ipfs/ipfs_utils.h"
#endif

#if BUILDFLAG(ENABLE_COMMANDER)
#include "hns/browser/ui/commander/commander_service_factory.h"
#include "hns/components/commander/browser/commander_frontend_delegate.h"
#include "hns/components/commander/common/features.h"
#endif

namespace {

class HnsLocationBarViewFocusRingHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  HnsLocationBarViewFocusRingHighlightPathGenerator() = default;
  HnsLocationBarViewFocusRingHighlightPathGenerator(
      const HnsLocationBarViewFocusRingHighlightPathGenerator&) = delete;
  HnsLocationBarViewFocusRingHighlightPathGenerator& operator=(
      const HnsLocationBarViewFocusRingHighlightPathGenerator&) = delete;

  // HighlightPathGenerator
  SkPath GetHighlightPath(const views::View* view) override {
    return static_cast<const HnsLocationBarView*>(view)
        ->GetFocusRingHighlightPath();
  }
};

absl::optional<HnsColorIds> GetFocusRingColor(Profile* profile) {
  if (hns::IsRegularProfile(profile) || profile->IsGuestSession()) {
    // Don't update color.
    return absl::nullopt;
  }
  // Private or Tor window - use color mixer.
  return kColorLocationBarFocusRing;
}

}  // namespace

void HnsLocationBarView::Init() {
  // base method calls Update and Layout
  LocationBarView::Init();
  // Change focus ring highlight path
  views::FocusRing* focus_ring = views::FocusRing::Get(this);
  if (focus_ring) {
    focus_ring->SetPathGenerator(
        std::make_unique<
            HnsLocationBarViewFocusRingHighlightPathGenerator>());
    if (const auto color_id = GetFocusRingColor(profile())) {
      focus_ring->SetColorId(color_id.value());
    }
  }

  if (!browser_->profile()->IsOffTheRecord()) {
    hns_news_location_view_ =
        AddChildView(std::make_unique<HnsNewsLocationView>(
            browser_->profile(), this, this));
    hns_news_location_view_->SetVisible(false);
    views::InkDrop::Get(hns_news_location_view_)
        ->SetVisibleOpacity(GetPageActionInkDropVisibleOpacity());
  }
#if BUILDFLAG(ENABLE_TOR)
  onion_location_view_ =
      AddChildView(std::make_unique<OnionLocationView>(browser_->profile()));
#endif
#if BUILDFLAG(ENABLE_IPFS)
  ipfs_location_view_ =
      AddChildView(std::make_unique<IPFSLocationView>(browser_->profile()));
#endif

  // hns action buttons
  hns_actions_ = AddChildView(
      std::make_unique<HnsActionsContainer>(browser_, profile()));
  hns_actions_->Init();
  // Call Update again to cause a Layout
  Update(nullptr);

  // Stop slide animation for all content settings views icon.
  for (auto* content_setting_view : content_setting_views_) {
    content_setting_view->disable_animation();
  }
}

bool HnsLocationBarView::ShouldShowIPFSLocationView() const {
#if BUILDFLAG(ENABLE_IPFS)
  const GURL& url = GetLocationBarModel()->GetURL();
  if (!ipfs::IpfsServiceFactory::IsIpfsEnabled(profile_) ||
      !ipfs::IsIPFSScheme(url) ||
      !ipfs::IsLocalGatewayConfigured(profile_->GetPrefs())) {
    return false;
  }

  return true;
#else
  return false;
#endif
}

void HnsLocationBarView::ShowPlaylistBubble() {
  if (auto* playlist_action_icon_view = GetPlaylistActionIconView()) {
    playlist_action_icon_view->ShowPlaylistBubble();
  }
}

PlaylistActionIconView* HnsLocationBarView::GetPlaylistActionIconView() {
  auto* playlist_action_icon_view =
      page_action_icon_controller()->GetPlaylistActionIconView();
  if (!playlist_action_icon_view) {
    return nullptr;
  }

  return views::AsViewClass<PlaylistActionIconView>(playlist_action_icon_view);
}

void HnsLocationBarView::Update(content::WebContents* contents) {
  // base Init calls update before our Init is run, so our children
  // may not be initialized yet
  if (hns_actions_) {
    hns_actions_->Update();
  }

  auto show_page_actions = !ShouldHidePageActionIcons();
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_) {
    onion_location_view_->Update(contents, show_page_actions);
  }
#endif
#if BUILDFLAG(ENABLE_IPFS)
  if (ipfs_location_view_) {
    ipfs_location_view_->Update(contents, show_page_actions);
  }
#endif

  if (hns_news_location_view_) {
    hns_news_location_view_->Update();
  }

  LocationBarView::Update(contents);

  if (!ShouldShowIPFSLocationView()) {
    return;
  }
  // Secure display text for a page was set by chromium.
  // We do not want to override this.
  if (!GetLocationBarModel()->GetSecureDisplayText().empty()) {
    return;
  }
  auto badge_text =
      hns_l10n::GetLocalizedResourceUTF16String(IDS_IPFS_BADGE_TITLE);
  location_icon_view()->SetLabel(badge_text);
}

ui::ImageModel HnsLocationBarView::GetLocationIcon(
    LocationIconView::Delegate::IconFetchedCallback on_icon_fetched) const {
  if (!ShouldShowIPFSLocationView() ||
      !omnibox_view_->model()->ShouldShowCurrentPageIcon()) {
    return LocationBarView::GetLocationIcon(std::move(on_icon_fetched));
  }

  auto& bundle = ui::ResourceBundle::GetSharedInstance();
  const auto& ipfs_logo = *bundle.GetImageSkiaNamed(IDR_HNS_IPFS_LOGO);
  return ui::ImageModel::FromImageSkia(ipfs_logo);
}

void HnsLocationBarView::OnOmniboxBlurred() {
#if BUILDFLAG(ENABLE_COMMANDER)
  if (base::FeatureList::IsEnabled(features::kHnsCommander)) {
    if (auto* commander_service =
            commander::CommanderServiceFactory::GetForBrowserContext(
                profile_)) {
      commander_service->Hide();
    }
  }
#endif
  LocationBarView::OnOmniboxBlurred();
}

void HnsLocationBarView::OnChanged() {
  auto hide_page_actions = ShouldHidePageActionIcons();
  if (hns_actions_) {
    hns_actions_->SetShouldHide(hide_page_actions);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_) {
    onion_location_view_->Update(
        browser_->tab_strip_model()->GetActiveWebContents(),
        !hide_page_actions);
  }
#endif
#if BUILDFLAG(ENABLE_IPFS)
  if (ipfs_location_view_) {
    ipfs_location_view_->Update(
        browser_->tab_strip_model()->GetActiveWebContents(),
        !hide_page_actions);
  }
#endif

  if (hns_news_location_view_) {
    hns_news_location_view_->Update();
  }

  // OnChanged calls Layout
  LocationBarView::OnChanged();
}

std::vector<views::View*> HnsLocationBarView::GetTrailingViews() {
  std::vector<views::View*> views;
  if (hns_news_location_view_) {
    views.push_back(hns_news_location_view_);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_) {
    views.push_back(onion_location_view_);
  }
#endif
#if BUILDFLAG(ENABLE_IPFS)
  if (ipfs_location_view_) {
    views.push_back(ipfs_location_view_);
  }
#endif

  if (hns_actions_) {
    views.push_back(hns_actions_);
  }

  return views;
}

gfx::Size HnsLocationBarView::CalculatePreferredSize() const {
  gfx::Size min_size = LocationBarView::CalculatePreferredSize();
  if (hns_actions_ && hns_actions_->GetVisible()) {
    const int hns_actions_min = hns_actions_->GetMinimumSize().width();
    const int extra_width =
        hns_actions_min + GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING);
    min_size.Enlarge(extra_width, 0);
  }
  if (hns_news_location_view_ && hns_news_location_view_->GetVisible()) {
    const int extra_width = GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
                            hns_news_location_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_ && onion_location_view_->GetVisible()) {
    const int extra_width = GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
                            onion_location_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#endif
#if BUILDFLAG(ENABLE_IPFS)
  if (ipfs_location_view_ && ipfs_location_view_->GetVisible()) {
    const int extra_width = GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
                            ipfs_location_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#endif

  return min_size;
}

void HnsLocationBarView::OnThemeChanged() {
  LocationBarView::OnThemeChanged();

  if (!IsInitialized()) {
    return;
  }

  Update(nullptr);
  RefreshBackground();
}

void HnsLocationBarView::ChildVisibilityChanged(views::View* child) {
  LocationBarView::ChildVisibilityChanged(child);
  // Normally, PageActionIcons are in a container which is always visible, only
  // the size changes when an icon is shown or hidden. The LocationBarView
  // does not listen to ChildVisibilityChanged events so we must make we Layout
  // and re-caculate trailing decorator positions when a child changes.
  if (base::Contains(GetTrailingViews(), child)) {
    Layout();
    SchedulePaint();
  }
}

int HnsLocationBarView::GetBorderRadius() const {
  return ChromeLayoutProvider::Get()->GetCornerRadiusMetric(
      views::Emphasis::kHigh, size());
}

SkPath HnsLocationBarView::GetFocusRingHighlightPath() const {
  const SkScalar radius = GetBorderRadius();
  return SkPath().addRoundRect(gfx::RectToSkRect(GetLocalBounds()), radius,
                               radius);
}

ContentSettingImageView*
HnsLocationBarView::GetContentSettingsImageViewForTesting(size_t idx) {
  DCHECK(idx < content_setting_views_.size());
  return content_setting_views_[idx];
}
