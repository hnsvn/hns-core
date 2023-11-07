/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/profiles/hns_avatar_toolbar_button.h"

#include <memory>
#include <string>

#include "base/strings/string_number_conversions.h"
#include "hns/app/vector_icons/vector_icons.h"
#include "hns/browser/ui/views/profiles/hns_avatar_toolbar_button_delegate.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/app/vector_icons/vector_icons.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_properties.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/profiles/avatar_toolbar_button.h"
#include "chrome/browser/ui/views/toolbar/toolbar_ink_drop_util.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/pointer/touch_ui_controller.h"
#include "ui/base/theme_provider.h"
#include "ui/gfx/geometry/rrect_f.h"
#include "ui/gfx/geometry/skia_conversions.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/highlight_path_generator.h"
#include "ui/views/view_class_properties.h"

namespace {

constexpr int kHighlightRadius = 36;
constexpr int kHnsAvatarButtonHorizontalSpacing = 10;

class HnsAvatarButtonHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  explicit HnsAvatarButtonHighlightPathGenerator(
      const base::WeakPtr<HnsAvatarToolbarButton>& avatar_button)
      : avatar_button_(avatar_button) {}
  HnsAvatarButtonHighlightPathGenerator(
      const HnsAvatarButtonHighlightPathGenerator&) = delete;
  HnsAvatarButtonHighlightPathGenerator& operator=(
      const HnsAvatarButtonHighlightPathGenerator&) = delete;
  ~HnsAvatarButtonHighlightPathGenerator() override = default;

  absl::optional<gfx::RRectF> GetRoundRect(const gfx::RectF& bounds) override {
    gfx::Rect rect(avatar_button_->size());
    rect.Inset(GetToolbarInkDropInsets(avatar_button_.get()));
    DCHECK(avatar_button_);
    if (avatar_button_->GetAvatarButtonState() ==
        AvatarToolbarButton::State::kAnimatedUserIdentity) {
      // In this case, our radius wouldn't be used. We should keep using
      // upstream's radius for the highlight too.
      return gfx::RRectF(gfx::RectF(rect),
                         ChromeLayoutProvider::Get()->GetCornerRadiusMetric(
                             views::Emphasis::kMaximum, {}));
    }

    // This make the highlight drawn as circle.
    return gfx::RRectF(gfx::RectF(rect), kHighlightRadius);
  }

 private:
  base::WeakPtr<HnsAvatarToolbarButton> avatar_button_;
};

}  // namespace

HnsAvatarToolbarButton::HnsAvatarToolbarButton(BrowserView* browser_view)
    : AvatarToolbarButton(browser_view) {
  // Replace ToolbarButton's highlight path generator.
  views::HighlightPathGenerator::Install(
      this, std::make_unique<HnsAvatarButtonHighlightPathGenerator>(
                weak_ptr_factory_.GetWeakPtr()));
}

HnsAvatarToolbarButton::~HnsAvatarToolbarButton() = default;

AvatarToolbarButton::State HnsAvatarToolbarButton::GetAvatarButtonState()
    const {
  return delegate_->GetState();
}

void HnsAvatarToolbarButton::SetHighlight(
    const std::u16string& highlight_text,
    absl::optional<SkColor> highlight_color) {
  std::u16string revised_highlight_text;
  if (browser_->profile()->IsTor()) {
    revised_highlight_text = hns_l10n::GetLocalizedResourceUTF16String(
        IDS_TOR_AVATAR_BUTTON_LABEL);

    if (GetWindowCount() > 1) {
      revised_highlight_text =
          l10n_util::GetStringFUTF16(IDS_TOR_AVATAR_BUTTON_LABEL_COUNT,
                                     base::NumberToString16(GetWindowCount()));
    }
  } else if (browser_->profile()->IsIncognitoProfile()) {
    // We only want the icon and count for Incognito profiles.
    revised_highlight_text = std::u16string();
    if (GetWindowCount() > 1) {
      revised_highlight_text = base::NumberToString16(GetWindowCount());
    }
  } else if (browser_->profile()->IsGuestSession()) {
    // We only want the icon for Guest profiles.
    revised_highlight_text = std::u16string();
  } else {
    revised_highlight_text = highlight_text;
  }

  AvatarToolbarButton::SetHighlight(revised_highlight_text, highlight_color);
}

int HnsAvatarToolbarButton::GetWindowCount() const {
  return delegate_->GetWindowCount();
}

void HnsAvatarToolbarButton::UpdateColorsAndInsets() {
  // Use custom bg/border for private/tor window.
  const bool is_tor = browser_->profile()->IsTor();
  if (delegate_->GetState() == State::kIncognitoProfile || is_tor) {
    constexpr SkColor kPrivateAvatarBGColor = SkColorSetRGB(0x1D, 0x19, 0x35);
    constexpr SkColor kPrivateTorAvatarBGColor =
        SkColorSetRGB(0x1A, 0x0F, 0x2A);

    const gfx::Insets paint_insets =
        gfx::Insets((height() - GetLayoutConstant(LOCATION_BAR_HEIGHT)) / 2) +
        *GetProperty(views::kInternalPaddingKey);
    SetEnabledTextColors(SK_ColorWHITE);
    SetBackground(views::CreateBackgroundFromPainter(
        views::Painter::CreateSolidRoundRectPainter(
            is_tor ? kPrivateTorAvatarBGColor : kPrivateAvatarBGColor,
            kHighlightRadius, paint_insets)));

    // We give more margins to horizontally.
    gfx::Insets target_insets =
        ::GetLayoutInsets(TOOLBAR_BUTTON) +
        *GetProperty(views::kInternalPaddingKey) +
        gfx::Insets::VH(0, kHnsAvatarButtonHorizontalSpacing);
    SetBorder(views::CreateEmptyBorder(target_insets));

    constexpr int kHnsAvatarImageLabelSpacing = 8;
    SetImageLabelSpacing(kHnsAvatarImageLabelSpacing);
    return;
  }

  AvatarToolbarButton::UpdateColorsAndInsets();
}

ui::ImageModel HnsAvatarToolbarButton::GetAvatarIcon(
    ButtonState state,
    const gfx::Image& gaia_account_image) const {
  // We don't use icon for tor avatar button.
  if (browser_->profile()->IsTor())
    return ui::ImageModel();

  if (browser_->profile()->IsGuestSession()) {
    return ui::ImageModel::FromVectorIcon(
        kUserMenuGuestIcon, GetForegroundColor(state),
        ui::TouchUiController::Get()->touch_ui() ? 24 : 20);
  }

  return AvatarToolbarButton::GetAvatarIcon(state, gaia_account_image);
}

std::u16string HnsAvatarToolbarButton::GetAvatarTooltipText() const {
  if (browser_->profile()->IsTor())
    return hns_l10n::GetLocalizedResourceUTF16String(
        IDS_TOR_AVATAR_BUTTON_TOOLTIP_TEXT);

  return AvatarToolbarButton::GetAvatarTooltipText();
}
