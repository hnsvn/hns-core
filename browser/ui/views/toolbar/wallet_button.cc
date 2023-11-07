// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/views/toolbar/wallet_button.h"

#include <vector>

#include "hns/browser/hns_wallet/hns_wallet_tab_helper.h"
#include "hns/browser/ui/hns_icon_with_badge_image_source.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/components/vector_icons/vector_icons.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/toolbar/toolbar_ink_drop_util.h"
#include "components/grit/hns_components_strings.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/color/color_provider_manager.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/rrect_f.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/views/controls/highlight_path_generator.h"
#include "ui/views/layout/fill_layout.h"

namespace {

constexpr int kHnsWalletLeftMarginExtra = -3;

content::WebContents* GetActiveWebContents() {
  return BrowserList::GetInstance()
      ->GetLastActive()
      ->tab_strip_model()
      ->GetActiveWebContents();
}

class HnsWalletButtonHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  // HighlightPathGenerator:
  SkPath GetHighlightPath(const views::View* view) override {
    DCHECK(view);

    gfx::Rect rect(view->size());
    rect.Inset(GetToolbarInkDropInsets(view));
    rect.Inset(gfx::Insets::TLBR(0, 0, 0, -1 * kHnsWalletLeftMarginExtra));

    auto* layout_provider = ChromeLayoutProvider::Get();
    DCHECK(layout_provider);

    int radius = layout_provider->GetCornerRadiusMetric(
        views::Emphasis::kMaximum, rect.size());

    SkPath path;
    path.addRoundRect(gfx::RectToSkRect(rect), radius, radius);
    return path;
  }
};

class WalletButtonMenuModel : public ui::SimpleMenuModel,
                              public ui::SimpleMenuModel::Delegate {
 public:
  explicit WalletButtonMenuModel(PrefService* prefs)
      : SimpleMenuModel(this), prefs_(prefs) {
    Build();
  }

  ~WalletButtonMenuModel() override = default;
  WalletButtonMenuModel(const WalletButtonMenuModel&) = delete;
  WalletButtonMenuModel& operator=(const WalletButtonMenuModel&) = delete;

 private:
  enum ContextMenuCommand {
    HideHnsWalletIcon,
  };

  // ui::SimpleMenuModel::Delegate override:
  void ExecuteCommand(int command_id, int event_flags) override {
    if (command_id == HideHnsWalletIcon) {
      prefs_->SetBoolean(kShowWalletIconOnToolbar, false);
    }
  }

  void Build() {
    AddItemWithStringId(HideHnsWalletIcon,
                        IDS_HIDE_HNS_WALLET_ICON_ON_TOOLBAR);
  }

  raw_ptr<PrefService> prefs_ = nullptr;
};

const ui::ColorProvider* GetColorProviderForView(
    base::WeakPtr<WalletButton> view) {
  if (view) {
    return view->GetColorProvider();
  }

  return ui::ColorProviderManager::Get().GetColorProviderFor(
      ui::NativeTheme::GetInstanceForNativeUi()->GetColorProviderKey(nullptr));
}

}  // namespace

WalletButton::WalletButton(View* backup_anchor_view, Profile* profile)
    : ToolbarButton(
          base::BindRepeating(&WalletButton::OnWalletPressed,
                              base::Unretained(this)),
          std::make_unique<WalletButtonMenuModel>(profile->GetPrefs()),
          nullptr,
          false),  // Long-pressing is not intended for something that
                   // already shows a panel on click
      prefs_(profile->GetPrefs()),
      backup_anchor_view_(backup_anchor_view) {
  pref_change_registrar_.Init(prefs_);
  pref_change_registrar_.Add(
      kShowWalletIconOnToolbar,
      base::BindRepeating(&WalletButton::OnPreferenceChanged,
                          base::Unretained(this)));

  // The MenuButtonController makes sure the panel closes when clicked if the
  // panel is already open.
  auto menu_button_controller = std::make_unique<views::MenuButtonController>(
      this,
      base::BindRepeating(&WalletButton::OnWalletPressed,
                          base::Unretained(this)),
      std::make_unique<views::Button::DefaultButtonControllerDelegate>(this));
  menu_button_controller_ = menu_button_controller.get();
  SetButtonController(std::move(menu_button_controller));

  UpdateVisibility();

  if (hns_wallet::ShouldShowTxStatusInToolbar()) {
    notification_source_ =
        std::make_unique<hns::WalletButtonNotificationSource>(
            profile, base::BindRepeating(&WalletButton::OnNotificationUpdate,
                                         weak_ptr_factory_.GetWeakPtr()));
  }

  views::HighlightPathGenerator::Install(
      this, std::make_unique<HnsWalletButtonHighlightPathGenerator>());
}

WalletButton::~WalletButton() = default;

void WalletButton::AddedToWidget() {
  if (notification_source_) {
    notification_source_->Init();
  }
}

void WalletButton::OnWalletPressed(const ui::Event& event) {
  if (IsShowingBubble()) {
    CloseWalletBubble();
    return;
  }

  ShowWalletBubble();
  notification_source_->MarkWalletButtonWasClicked();
}

void WalletButton::OnNotificationUpdate(bool show_suggest_badge,
                                        size_t counter) {
  show_suggest_badge_ = show_suggest_badge;
  counter_ = counter;
  UpdateImageAndText();
}

std::string WalletButton::GetBadgeText() {
  if (counter_ > 0) {
    std::string text = counter_ > 99 ? "99+" : base::NumberToString(counter_);
    return text;
  }
  return "";
}

void WalletButton::UpdateImageAndText() {
  const ui::ColorProvider* color_provider = GetColorProvider();
  SkColor icon_color = color_provider->GetColor(kColorToolbarButtonIcon);
  auto icon = gfx::CreateVectorIcon(kLeoProductHnsWalletIcon, 16, icon_color);

  size_t icon_size = std::max(icon.width(), icon.height());
  auto badge_size = hns::HnsIconWithBadgeImageSource::GetMaxBadgeSize();
  gfx::Size preferred_size(icon_size + badge_size.width(),
                           icon_size + badge_size.height() / 2);

  auto image_source = std::make_unique<hns::HnsIconWithBadgeImageSource>(
      preferred_size,
      base::BindRepeating(&GetColorProviderForView,
                          weak_ptr_factory_.GetWeakPtr()),
      icon_size, kHnsWalletLeftMarginExtra);
  image_source->SetAllowEmptyText(show_suggest_badge_);
  image_source->SetIcon(gfx::Image(icon));

  auto text = GetBadgeText();
  image_source->SetBadge(std::make_unique<IconWithBadgeImageSource::Badge>(
      text, hns::kBadgeTextColor, hns::kBadgeNotificationBG));
  SetImage(views::Button::STATE_NORMAL,
           gfx::ImageSkia(std::move(image_source), preferred_size));
  SetTooltipText(
      hns_l10n::GetLocalizedResourceUTF16String(IDS_TOOLTIP_WALLET));
}

void WalletButton::UpdateVisibility() {
  SetVisible(prefs_->GetBoolean(kShowWalletIconOnToolbar));
}

void WalletButton ::OnPreferenceChanged() {
  UpdateVisibility();
}

void WalletButton::ShowWalletBubble() {
  hns_wallet::HnsWalletTabHelper::FromWebContents(GetActiveWebContents())
      ->ShowBubble();
}

void WalletButton::ShowApproveWalletBubble() {
  hns_wallet::HnsWalletTabHelper::FromWebContents(GetActiveWebContents())
      ->ShowApproveWalletBubble();
}

void WalletButton::CloseWalletBubble() {
  hns_wallet::HnsWalletTabHelper::FromWebContents(GetActiveWebContents())
      ->CloseBubble();
}

bool WalletButton::IsShowingBubble() {
  return hns_wallet::HnsWalletTabHelper::FromWebContents(
             GetActiveWebContents())
      ->IsShowingBubble();
}

bool WalletButton::IsBubbleClosedForTesting() {
  return hns_wallet::HnsWalletTabHelper::FromWebContents(
             GetActiveWebContents())
      ->IsBubbleClosedForTesting();
}

views::View* WalletButton::GetAsAnchorView() {
  View* anchor_view = this;
  if (!prefs_->GetBoolean(kShowWalletIconOnToolbar)) {
    anchor_view = backup_anchor_view_;
  }
  return anchor_view;
}

BEGIN_METADATA(WalletButton, ToolbarButton)
END_METADATA
