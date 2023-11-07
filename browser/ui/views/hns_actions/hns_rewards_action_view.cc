// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/views/hns_actions/hns_rewards_action_view.h"

#include <memory>
#include <string>
#include <utility>

#include "base/strings/string_number_conversions.h"
#include "hns/app/vector_icons/vector_icons.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/ui/hns_icon_with_badge_image_source.h"
#include "hns/browser/ui/views/bubble/hns_webui_bubble_manager.h"
#include "hns/browser/ui/webui/hns_rewards/rewards_panel_ui.h"
#include "hns/components/hns_rewards/browser/rewards_p3a.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/components/vector_icons/vector_icons.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "components/grit/hns_components_strings.h"
#include "components/prefs/pref_service.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/color/color_provider_manager.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/animation/ink_drop_impl.h"
#include "ui/views/controls/button/label_button_border.h"
#include "ui/views/controls/button/menu_button_controller.h"
#include "ui/views/controls/highlight_path_generator.h"
#include "ui/views/view_class_properties.h"

namespace {

using hns_rewards::RewardsNotificationService;
using hns_rewards::RewardsPanelCoordinator;
using hns_rewards::RewardsPanelUI;
using hns_rewards::RewardsServiceFactory;
using hns_rewards::RewardsTabHelper;

constexpr SkColor kIconColor = SK_ColorBLACK;
constexpr SkColor kBadgeVerifiedBG = SkColorSetRGB(0x42, 0x3E, 0xEE);

class ButtonHighlightPathGenerator : public views::HighlightPathGenerator {
 public:
  // views::HighlightPathGenerator:
  SkPath GetHighlightPath(const views::View* view) override {
    DCHECK(view);
    gfx::Rect rect(view->GetPreferredSize());
    rect.Inset(gfx::Insets::TLBR(0, 0, 0, -1 * kHnsActionLeftMarginExtra));

    auto* layout_provider = ChromeLayoutProvider::Get();
    DCHECK(layout_provider);

    int radius = layout_provider->GetCornerRadiusMetric(
        views::Emphasis::kMaximum, rect.size());

    SkPath path;
    path.addRoundRect(gfx::RectToSkRect(rect), radius, radius);
    return path;
  }
};

const ui::ColorProvider* GetColorProviderForWebContents(
    base::WeakPtr<content::WebContents> web_contents) {
  if (web_contents) {
    return &web_contents->GetColorProvider();
  }

  return ui::ColorProviderManager::Get().GetColorProviderFor(
      ui::NativeTheme::GetInstanceForNativeUi()->GetColorProviderKey(nullptr));
}

// Draws a custom badge for the "verified" checkmark.
class RewardsBadgeImageSource : public hns::HnsIconWithBadgeImageSource {
 public:
  RewardsBadgeImageSource(const gfx::Size& size,
                          GetColorProviderCallback get_color_provider_callback)
      : HnsIconWithBadgeImageSource(size,
                                      std::move(get_color_provider_callback),
                                      kHnsActionGraphicSize,
                                      kHnsActionLeftMarginExtra) {}

  void UseVerifiedIcon(bool verified_icon) {
    verified_icon_ = verified_icon;
    SetAllowEmptyText(verified_icon);
  }

 private:
  // hns::HnsIconWithBadgeImageSource:
  void PaintBadgeWithoutText(const gfx::Rect& badge_rect,
                             gfx::Canvas* canvas) override {
    if (!verified_icon_) {
      HnsIconWithBadgeImageSource::PaintBadgeWithoutText(badge_rect, canvas);
      return;
    }

    // The verified icon must be drawn slightly larger than the default badge
    // area. Expand the badge rectangle accordingly.
    gfx::Rect image_rect(badge_rect);
    gfx::Outsets outsets;
    outsets.set_top(3);
    outsets.set_left(2);
    outsets.set_right(1);
    image_rect.Outset(outsets);

    gfx::RectF check_rect(image_rect);
    check_rect.Inset(4);
    cc::PaintFlags check_flags;
    check_flags.setStyle(cc::PaintFlags::kFill_Style);
    check_flags.setColor(SK_ColorWHITE);
    check_flags.setAntiAlias(true);
    canvas->DrawRoundRect(check_rect, 2, check_flags);

    auto image = gfx::CreateVectorIcon(kLeoVerificationFilledIcon,
                                       image_rect.width(), kBadgeVerifiedBG);

    cc::PaintFlags image_flags;
    image_flags.setStyle(cc::PaintFlags::kFill_Style);
    image_flags.setAntiAlias(true);
    canvas->DrawImageInt(image, image_rect.x(), image_rect.y(), image_flags);
  }

  bool verified_icon_ = false;
};

// Provides the context menu for the Rewards button.
class RewardsActionMenuModel : public ui::SimpleMenuModel,
                               public ui::SimpleMenuModel::Delegate {
 public:
  explicit RewardsActionMenuModel(PrefService* prefs)
      : SimpleMenuModel(this), prefs_(prefs) {
    Build();
  }

  ~RewardsActionMenuModel() override = default;
  RewardsActionMenuModel(const RewardsActionMenuModel&) = delete;
  RewardsActionMenuModel& operator=(const RewardsActionMenuModel&) = delete;

 private:
  enum ContextMenuCommand { kHideHnsRewardsIcon };

  // ui::SimpleMenuModel::Delegate override:
  void ExecuteCommand(int command_id, int event_flags) override {
    if (command_id == kHideHnsRewardsIcon) {
      prefs_->SetBoolean(hns_rewards::prefs::kShowLocationBarButton, false);
    }
  }

  void Build() {
    AddItemWithStringId(kHideHnsRewardsIcon,
                        IDS_HIDE_HNS_REWARDS_ACTION_ICON);
  }

  raw_ptr<PrefService> prefs_ = nullptr;
};

}  // namespace

HnsRewardsActionView::HnsRewardsActionView(Browser* browser)
    : ToolbarButton(
          base::BindRepeating(&HnsRewardsActionView::OnButtonPressed,
                              base::Unretained(this)),
          std::make_unique<RewardsActionMenuModel>(
              browser->profile()->GetPrefs()),
          nullptr,
          false),
      browser_(browser),
      bubble_manager_(std::make_unique<HnsWebUIBubbleManager<RewardsPanelUI>>(
          this,
          browser_->profile(),
          GURL(kHnsRewardsPanelURL),
          IDS_HNS_UI_HNS_REWARDS)) {
  DCHECK(browser_);

  SetButtonController(std::make_unique<views::MenuButtonController>(
      this,
      base::BindRepeating(&HnsRewardsActionView::OnButtonPressed,
                          base::Unretained(this)),
      std::make_unique<views::Button::DefaultButtonControllerDelegate>(this)));

  views::HighlightPathGenerator::Install(
      this, std::make_unique<ButtonHighlightPathGenerator>());

  // The highlight opacity set by |ToolbarButton| is different that the default
  // highlight opacity used by the other buttons in the actions container. Unset
  // the highlight opacity to match.
  views::InkDrop::Get(this)->SetHighlightOpacity({});

  SetHorizontalAlignment(gfx::ALIGN_CENTER);
  SetLayoutInsets(gfx::Insets(0));
  SetAccessibleName(
      hns_l10n::GetLocalizedResourceUTF16String(IDS_HNS_UI_HNS_REWARDS));

  auto* profile = browser_->profile();

  pref_change_registrar_.Init(profile->GetPrefs());
  pref_change_registrar_.Add(
      hns_rewards::prefs::kBadgeText,
      base::BindRepeating(&HnsRewardsActionView::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      hns_rewards::prefs::kDeclaredGeo,
      base::BindRepeating(&HnsRewardsActionView::OnPreferencesChanged,
                          base::Unretained(this)));

  browser_->tab_strip_model()->AddObserver(this);

  if (auto* rewards_service = GetRewardsService()) {
    rewards_service_observation_.Observe(rewards_service);
  }

  if (auto* notification_service = GetNotificationService()) {
    notification_service_observation_.Observe(notification_service);
  }

  panel_coordinator_ = RewardsPanelCoordinator::FromBrowser(browser_);
  if (panel_coordinator_) {
    panel_observation_.Observe(panel_coordinator_);
  }

  UpdateTabHelper(GetActiveWebContents());
}

HnsRewardsActionView::~HnsRewardsActionView() = default;

void HnsRewardsActionView::Update() {
  gfx::Size preferred_size = GetPreferredSize();
  auto* web_contents = GetActiveWebContents();
  auto weak_contents = web_contents ? web_contents->GetWeakPtr()
                                    : base::WeakPtr<content::WebContents>();

  auto image_source = std::make_unique<RewardsBadgeImageSource>(
      preferred_size,
      base::BindRepeating(GetColorProviderForWebContents, weak_contents));
  image_source->SetIcon(gfx::Image(GetRewardsIcon()));

  auto [text, background_color] = GetBadgeTextAndBackground();
  image_source->SetBadge(std::make_unique<IconWithBadgeImageSource::Badge>(
      text, hns::kBadgeTextColor, background_color));
  image_source->UseVerifiedIcon(background_color == kBadgeVerifiedBG);

  SetImage(views::Button::STATE_NORMAL,
           gfx::ImageSkia(std::move(image_source), preferred_size));
}

void HnsRewardsActionView::ClosePanelForTesting() {
  if (IsPanelOpen()) {
    ToggleRewardsPanel();
  }
}

gfx::Rect HnsRewardsActionView::GetAnchorBoundsInScreen() const {
  if (!GetVisible()) {
    // If the button is currently hidden, then anchor the bubble to the
    // location bar instead.
    auto* browser_view = BrowserView::GetBrowserViewForBrowser(browser_);
    DCHECK(browser_view);
    return browser_view->GetLocationBarView()->GetAnchorBoundsInScreen();
  }
  return ToolbarButton::GetAnchorBoundsInScreen();
}

std::unique_ptr<views::LabelButtonBorder>
HnsRewardsActionView::CreateDefaultBorder() const {
  auto border = ToolbarButton::CreateDefaultBorder();
  border->set_insets(gfx::Insets::TLBR(0, 0, 0, 0));
  return border;
}

void HnsRewardsActionView::OnWidgetDestroying(views::Widget* widget) {
  DCHECK(bubble_observation_.IsObservingSource(widget));
  bubble_observation_.Reset();
}

void HnsRewardsActionView::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  if (selection.active_tab_changed()) {
    UpdateTabHelper(selection.new_contents);
  }
}

void HnsRewardsActionView::OnPublisherForTabUpdated(
    const std::string& publisher_id) {
  publisher_registered_ = {publisher_id, false};
  bool status_updating = UpdatePublisherStatus();
  if (!status_updating) {
    Update();
  }
}

void HnsRewardsActionView::OnRewardsPanelRequested(
    const hns_rewards::mojom::RewardsPanelArgs& args) {
  if (!IsPanelOpen()) {
    ToggleRewardsPanel();
  }
}

void HnsRewardsActionView::OnPublisherRegistryUpdated() {
  UpdatePublisherStatus();
}

void HnsRewardsActionView::OnPublisherUpdated(
    const std::string& publisher_id) {
  if (publisher_id == std::get<std::string>(publisher_registered_)) {
    UpdatePublisherStatus();
  }
}

void HnsRewardsActionView::OnNotificationAdded(
    RewardsNotificationService* service,
    const RewardsNotificationService::RewardsNotification& notification) {
  Update();
}

void HnsRewardsActionView::OnNotificationDeleted(
    RewardsNotificationService* service,
    const RewardsNotificationService::RewardsNotification& notification) {
  Update();
}

void HnsRewardsActionView::OnButtonPressed() {
  hns_rewards::RewardsService* rewards_service = GetRewardsService();
  if (rewards_service != nullptr) {
    auto* prefs = browser_->profile()->GetPrefs();
    if (!prefs->GetBoolean(hns_rewards::prefs::kEnabled)) {
      rewards_service->GetP3AConversionMonitor()->RecordPanelTrigger(
          ::hns_rewards::p3a::PanelTrigger::kToolbarButton);
    }
  }
  // If we are opening the Rewards panel, use `RewardsPanelCoordinator` to open
  // it so that the panel arguments will be correctly set.
  if (!IsPanelOpen() && panel_coordinator_) {
    panel_coordinator_->OpenRewardsPanel();
    return;
  }

  ToggleRewardsPanel();
}

void HnsRewardsActionView::OnPreferencesChanged(const std::string& key) {
  Update();
}

content::WebContents* HnsRewardsActionView::GetActiveWebContents() {
  return browser_->tab_strip_model()->GetActiveWebContents();
}

hns_rewards::RewardsService* HnsRewardsActionView::GetRewardsService() {
  return RewardsServiceFactory::GetForProfile(browser_->profile());
}

hns_rewards::RewardsNotificationService*
HnsRewardsActionView::GetNotificationService() {
  if (auto* rewards_service = GetRewardsService()) {
    return rewards_service->GetNotificationService();
  }
  return nullptr;
}

bool HnsRewardsActionView::IsPanelOpen() {
  return bubble_observation_.IsObserving();
}

void HnsRewardsActionView::ToggleRewardsPanel() {
  if (IsPanelOpen()) {
    DCHECK(bubble_manager_);
    bubble_manager_->CloseBubble();
    return;
  }

  // Clear the default-on-start badge text when the user opens the panel.
  auto* prefs = browser_->profile()->GetPrefs();
  prefs->SetString(hns_rewards::prefs::kBadgeText, "");

  bubble_manager_->ShowBubble();

  DCHECK(!bubble_observation_.IsObserving());
  bubble_observation_.Observe(bubble_manager_->GetBubbleWidget());
}

gfx::ImageSkia HnsRewardsActionView::GetRewardsIcon() {
  // Since the BAT icon has color the actual color value here is not relevant,
  // but |CreateVectorIcon| requires one.
  return gfx::CreateVectorIcon(kBatIcon, kHnsActionGraphicSize, kIconColor);
}

std::pair<std::string, SkColor>
HnsRewardsActionView::GetBadgeTextAndBackground() {
  // 1. Display the default-on-start Rewards badge text, if specified.
  std::string text_pref = browser_->profile()->GetPrefs()->GetString(
      hns_rewards::prefs::kBadgeText);
  if (!text_pref.empty()) {
    return {text_pref, hns::kBadgeNotificationBG};
  }

  // 2. Display the number of current notifications, if non-zero.
  size_t notifications = GetRewardsNotificationCount();
  if (notifications > 0) {
    std::string text =
        notifications > 99 ? "99+" : base::NumberToString(notifications);

    return {text, hns::kBadgeNotificationBG};
  }

  // 3. Display a verified checkmark for verified publishers.
  if (std::get<bool>(publisher_registered_)) {
    return {"", kBadgeVerifiedBG};
  }

  return {"", hns::kBadgeNotificationBG};
}

size_t HnsRewardsActionView::GetRewardsNotificationCount() {
  size_t count = 0;

  if (auto* service = GetNotificationService()) {
    count += service->GetAllNotifications().size();
  }

  // Increment the notification count if the user has enabled Rewards but has
  // not declared a country.
  auto* prefs = browser_->profile()->GetPrefs();
  if (prefs->GetBoolean(hns_rewards::prefs::kEnabled) &&
      prefs->GetString(hns_rewards::prefs::kDeclaredGeo).empty()) {
    ++count;
  }

  return count;
}

bool HnsRewardsActionView::UpdatePublisherStatus() {
  std::string& publisher_id = std::get<std::string>(publisher_registered_);
  if (publisher_id.empty()) {
    return false;
  }

  auto* rewards_service = GetRewardsService();
  if (!rewards_service) {
    return false;
  }

  rewards_service->IsPublisherRegistered(
      publisher_id,
      base::BindOnce(&HnsRewardsActionView::IsPublisherRegisteredCallback,
                     weak_factory_.GetWeakPtr(), publisher_id));

  return true;
}

void HnsRewardsActionView::IsPublisherRegisteredCallback(
    const std::string& publisher_id,
    bool is_registered) {
  if (publisher_id == std::get<std::string>(publisher_registered_)) {
    publisher_registered_.second = is_registered;
    Update();
  }
}

void HnsRewardsActionView::UpdateTabHelper(
    content::WebContents* web_contents) {
  tab_helper_ = nullptr;
  if (tab_helper_observation_.IsObserving()) {
    tab_helper_observation_.Reset();
  }

  if (web_contents) {
    if (auto* helper = RewardsTabHelper::FromWebContents(web_contents)) {
      tab_helper_ = helper;
      tab_helper_observation_.Observe(helper);
    }
  }

  OnPublisherForTabUpdated(tab_helper_ ? tab_helper_->GetPublisherIdForTab()
                                       : "");
}
