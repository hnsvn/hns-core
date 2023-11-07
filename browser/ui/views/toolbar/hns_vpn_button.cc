/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/toolbar/hns_vpn_button.h"

#include <utility>

#include "base/memory/raw_ptr.h"
#include "base/notreached.h"
#include "hns/app/hns_command_ids.h"
#include "hns/app/vector_icons/vector_icons.h"
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/browser/ui/color/hns_color_id.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/views/toolbar/toolbar_ink_drop_util.h"
#include "components/grit/hns_components_strings.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/compositor/layer.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/rrect_f.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/animation/ink_drop.h"
#include "ui/views/animation/ink_drop_host.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/highlight_path_generator.h"

using ConnectionState = hns_vpn::mojom::ConnectionState;
using PurchasedState = hns_vpn::mojom::PurchasedState;

namespace {

constexpr int kButtonRadius = 100;

class HnsVPNButtonHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  explicit HnsVPNButtonHighlightPathGenerator(const gfx::Insets& insets)
      : HighlightPathGenerator(insets) {}

  HnsVPNButtonHighlightPathGenerator(
      const HnsVPNButtonHighlightPathGenerator&) = delete;
  HnsVPNButtonHighlightPathGenerator& operator=(
      const HnsVPNButtonHighlightPathGenerator&) = delete;

  // views::HighlightPathGenerator overrides:
  absl::optional<gfx::RRectF> GetRoundRect(const gfx::RectF& rect) override {
    return gfx::RRectF(rect, kButtonRadius);
  }
};

// For error icon's inner color.
class ConnectErrorIconBackground : public views::Background {
 public:
  explicit ConnectErrorIconBackground(SkColor color) {
    SetNativeControlColor(color);
  }

  ConnectErrorIconBackground(const ConnectErrorIconBackground&) = delete;
  ConnectErrorIconBackground& operator=(const ConnectErrorIconBackground&) =
      delete;

  void Paint(gfx::Canvas* canvas, views::View* view) const override {
    auto bounds = view->GetLocalBounds();
    bounds.Inset(gfx::Insets::TLBR(2, 4, 2, 4));
    canvas->FillRect(bounds, get_color());
  }
};

class VPNButtonMenuModel : public ui::SimpleMenuModel,
                           public ui::SimpleMenuModel::Delegate,
                           public hns_vpn::HnsVPNServiceObserver {
 public:
  explicit VPNButtonMenuModel(Browser* browser)
      : SimpleMenuModel(this),
        browser_(browser),
        service_(hns_vpn::HnsVpnServiceFactory::GetForProfile(
            browser_->profile())) {
    DCHECK(service_);
    Observe(service_);
    Build(service_->is_purchased_user());
  }

  ~VPNButtonMenuModel() override = default;
  VPNButtonMenuModel(const VPNButtonMenuModel&) = delete;
  VPNButtonMenuModel& operator=(const VPNButtonMenuModel&) = delete;

 private:
  // ui::SimpleMenuModel::Delegate override:
  void ExecuteCommand(int command_id, int event_flags) override {
    chrome::ExecuteCommand(browser_, command_id);
  }

  // HnsVPNServiceObserver overrides:
  void OnPurchasedStateChanged(
      hns_vpn::mojom::PurchasedState state,
      const absl::optional<std::string>& description) override {
    // Rebuild menu items based on purchased state change.
    Build(service_->is_purchased_user());
  }

  void Build(bool purchased) {
    // Clear all menu items and re-build as purchased state can be updated
    // during the runtime.
    Clear();
    AddItemWithStringId(IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON,
                        IDS_HNS_VPN_HIDE_VPN_BUTTON_MENU_ITEM);
    if (purchased) {
      AddItemWithStringId(IDC_SEND_HNS_VPN_FEEDBACK,
                          IDS_HNS_VPN_SHOW_FEEDBACK_MENU_ITEM);
      AddItemWithStringId(IDC_ABOUT_HNS_VPN,
                          IDS_HNS_VPN_ABOUT_VPN_MENU_ITEM);
      AddItemWithStringId(IDC_MANAGE_HNS_VPN_PLAN,
                          IDS_HNS_VPN_MANAGE_MY_PLAN_MENU_ITEM);
    }
  }

  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<hns_vpn::HnsVpnService> service_ = nullptr;
};

}  // namespace

HnsVPNButton::HnsVPNButton(Browser* browser)
    : ToolbarButton(base::BindRepeating(&HnsVPNButton::OnButtonPressed,
                                        base::Unretained(this)),
                    std::make_unique<VPNButtonMenuModel>(browser),
                    nullptr,
                    false),  // Long-pressing is not intended for something that
                             // already shows a panel on click
      browser_(browser),
      service_(hns_vpn::HnsVpnServiceFactory::GetForProfile(
          browser_->profile())) {
  DCHECK(service_);
  Observe(service_);

  // Replace ToolbarButton's highlight path generator.
  views::HighlightPathGenerator::Install(
      this, std::make_unique<HnsVPNButtonHighlightPathGenerator>(
                GetToolbarInkDropInsets(this)));

  // Set 0.0f to use same color for activated state.
  views::InkDrop::Get(this)->SetVisibleOpacity(0.00f);

  // Different base color is set per themes and it has alpha.
  views::InkDrop::Get(this)->SetHighlightOpacity(1.0f);

  // The MenuButtonController makes sure the panel closes when clicked if the
  // panel is already open.
  auto menu_button_controller = std::make_unique<views::MenuButtonController>(
      this,
      base::BindRepeating(&HnsVPNButton::OnButtonPressed,
                          base::Unretained(this)),
      std::make_unique<views::Button::DefaultButtonControllerDelegate>(this));
  menu_button_controller_ = menu_button_controller.get();
  SetButtonController(std::move(menu_button_controller));

  SetTextSubpixelRenderingEnabled(false);
  label()->SetText(hns_l10n::GetLocalizedResourceUTF16String(
      IDS_HNS_VPN_TOOLBAR_BUTTON_TEXT));
  gfx::FontList font_list = views::Label::GetDefaultFontList();
  constexpr int kFontSize = 12;
  label()->SetFontList(
      font_list.DeriveWithSizeDelta(kFontSize - font_list.GetFontSize()));

  // W/o layer, ink drop affects text color.
  label()->SetPaintToLayer();

  // To clear previous pixels.
  label()->layer()->SetFillsBoundsOpaquely(false);

  // Set image positions first. then label.
  SetHorizontalAlignment(gfx::ALIGN_LEFT);

  // Views resulting in focusable nodes later on in the accessibility tree need
  // to have an accessible name for screen readers to see what they are about.
  // TODO(simonhong): Re-visit this name.
  SetAccessibleName(hns_l10n::GetLocalizedResourceUTF16String(
      IDS_HNS_VPN_TOOLBAR_BUTTON_TEXT));

  constexpr int kHnsAvatarImageLabelSpacing = 4;
  SetImageLabelSpacing(kHnsAvatarImageLabelSpacing);
}

HnsVPNButton::~HnsVPNButton() = default;

void HnsVPNButton::OnConnectionStateChanged(ConnectionState state) {
  UpdateColorsAndInsets();
}

void HnsVPNButton::OnPurchasedStateChanged(
    hns_vpn::mojom::PurchasedState state,
    const absl::optional<std::string>& description) {
  if (IsPurchased()) {
    UpdateColorsAndInsets();
  }
}

std::unique_ptr<views::Border> HnsVPNButton::GetBorder(
    SkColor border_color) const {
  constexpr auto kTargetInsets = gfx::Insets::VH(5, 11);
  constexpr auto kBorderThickness = 1;
  std::unique_ptr<views::Border> border = views::CreateRoundedRectBorder(
      kBorderThickness, kButtonRadius, gfx::Insets(), border_color);
  const gfx::Insets extra_insets = kTargetInsets - border->GetInsets();
  return views::CreatePaddedBorder(std::move(border), extra_insets);
}

void HnsVPNButton::UpdateColorsAndInsets() {
  ui::ColorProvider* cp = GetColorProvider();
  if (!cp) {
    return;
  }
  const bool is_connect_error = IsConnectError();
  const bool is_connected = IsConnected();
  const auto bg_color =
      cp->GetColor(is_connect_error ? kColorHnsVpnButtonErrorBackgroundNormal
                                    : kColorHnsVpnButtonBackgroundNormal);
  SetBackground(views::CreateRoundedRectBackground(bg_color, kButtonRadius));

  SetEnabledTextColors(cp->GetColor(is_connect_error
                                        ? kColorHnsVpnButtonTextError
                                        : kColorHnsVpnButtonText));

  if (is_connect_error) {
    SetImage(
        views::Button::STATE_NORMAL,
        gfx::CreateVectorIcon(kVpnIndicatorErrorIcon,
                              cp->GetColor(kColorHnsVpnButtonIconError)));

    // Use background for inner color of error button image.
    image()->SetBackground(std::make_unique<ConnectErrorIconBackground>(
        cp->GetColor(kColorHnsVpnButtonIconErrorInner)));
  } else {
    SetImage(
        views::Button::STATE_NORMAL,
        gfx::CreateVectorIcon(
            is_connected ? kVpnIndicatorOnIcon : kVpnIndicatorOffIcon,
            cp->GetColor(is_connected ? kColorHnsVpnButtonIconConnected
                                      : kColorHnsVpnButtonIconDisconnected)));

    // Use background for inner color of button image.
    // Adjusted border thickness to make invisible to the outside of the icon.
    image()->SetBackground(views::CreateRoundedRectBackground(
        cp->GetColor(kColorHnsVpnButtonIconInner), 5 /*radi*/, 2 /*thick*/));
  }

  // Compute highlight color and border in advance. If not, highlight color and
  // border color are mixed as both have alpha value.
  // Draw border only for error state.
  SetBorder(GetBorder(color_utils::GetResultingPaintColor(
      cp->GetColor(is_connect_error ? kColorHnsVpnButtonErrorBorder
                                    : kColorHnsVpnButtonBorder),
      bg_color)));

  auto* ink_drop_host = views::InkDrop::Get(this);

  // Use different ink drop hover color for each themes.
  auto target_base_color = color_utils::GetResultingPaintColor(
      cp->GetColor(is_connect_error ? kColorHnsVpnButtonErrorBackgroundHover
                                    : kColorHnsVpnButtonBorder),
      bg_color);
  bool need_ink_drop_color_update =
      target_base_color != ink_drop_host->GetBaseColor();

  // Update ink drop color if needed because we toggle ink drop mode below after
  // set base color. Toggling could cause subtle flicking.
  if (!need_ink_drop_color_update) {
    return;
  }

  views::InkDrop::Get(this)->SetBaseColor(target_base_color);

  // Hack to update inkdrop color immediately.
  // W/o this, background color and image are changed but inkdrop color is still
  // using previous one till button state is changed after changing base color.
  const auto previous_ink_drop_state =
      views::InkDrop::Get(this)->GetInkDrop()->GetTargetInkDropState();
  views::InkDrop::Get(this)->SetMode(views::InkDropHost::InkDropMode::OFF);
  views::InkDrop::Get(this)->SetMode(views::InkDropHost::InkDropMode::ON);
  // After toggling, ink drop state is reset. So need to re-apply previous
  // state.
  if (previous_ink_drop_state == views::InkDropState::ACTIVATED) {
    views::InkDrop::Get(this)->GetInkDrop()->SnapToActivated();
  }
}

std::u16string HnsVPNButton::GetTooltipText(const gfx::Point& p) const {
  if (!IsPurchased())
    return l10n_util::GetStringUTF16(IDS_HNS_VPN);

  return l10n_util::GetStringUTF16(IsConnected()
                                       ? IDS_HNS_VPN_CONNECTED_TOOLTIP
                                       : IDS_HNS_VPN_DISCONNECTED_TOOLTIP);
}

bool HnsVPNButton::IsConnected() const {
  return service_->IsConnected();
}

bool HnsVPNButton::IsConnectError() const {
  const auto state = service_->GetConnectionState();
  return (state == ConnectionState::CONNECT_NOT_ALLOWED ||
          state == ConnectionState::CONNECT_FAILED);
}

bool HnsVPNButton::IsPurchased() const {
  return service_->is_purchased_user();
}
void HnsVPNButton::OnButtonPressed(const ui::Event& event) {
  chrome::ExecuteCommand(browser_, IDC_SHOW_HNS_VPN_PANEL);
}

BEGIN_METADATA(HnsVPNButton, LabelButton)
END_METADATA
