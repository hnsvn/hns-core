/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/infobars/hns_wayback_machine_infobar_button_container.h"

#include <memory>
#include <utility>

#include "hns/browser/ui/views/infobars/hns_wayback_machine_infobar_throbber.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "ui/views/controls/button/md_text_button.h"
#include "ui/views/view_class_properties.h"

namespace {
constexpr int kThrobberDiameter = 16;
constexpr int kInsetOffsetsForThrobber = kThrobberDiameter;
}  // namespace

HnsWaybackMachineInfoBarButtonContainer::
    HnsWaybackMachineInfoBarButtonContainer(
        views::Button::PressedCallback callback) {
  auto button = std::make_unique<views::MdTextButton>(
      std::move(callback), hns_l10n::GetLocalizedResourceUTF16String(
                               IDS_HNS_WAYBACK_MACHINE_CHECK_BUTTON_TEXT));
  button_ = button.get();
  button->SetProminent(true);
  button->SizeToPreferredSize();
  AddChildView(button.release());

  throbber_ = new HnsWaybackMachineInfoBarThrobber;
  throbber_->SetSize(gfx::Size(kThrobberDiameter, kThrobberDiameter));
  throbber_->SetVisible(false);
  button_->AddChildView(throbber_.get());
}

HnsWaybackMachineInfoBarButtonContainer::
    ~HnsWaybackMachineInfoBarButtonContainer() = default;

void HnsWaybackMachineInfoBarButtonContainer::Layout() {
  if (throbber_->GetVisible()) {
    int x = button_->width() - throbber_->width() - kThrobberDiameter / 2;
    int y = (button_->height() - throbber_->height()) / 2;
    throbber_->SetPosition(gfx::Point(x, y));
  }
}

gfx::Size
HnsWaybackMachineInfoBarButtonContainer::CalculatePreferredSize() const {
  // This container doesn't need more space than button because throbber is
  // drawn over the button.
  return button_->GetPreferredSize();
}

void HnsWaybackMachineInfoBarButtonContainer::StartThrobber() {
  AdjustButtonInsets(true);
  throbber_->SetVisible(true);
  throbber_->Start();
  Layout();
}

void HnsWaybackMachineInfoBarButtonContainer::StopThrobber() {
  AdjustButtonInsets(false);
  throbber_->SetVisible(false);
  throbber_->Stop();
  Layout();
}

void HnsWaybackMachineInfoBarButtonContainer::AdjustButtonInsets(
    bool add_insets) {
  const gfx::Insets insets_offset =
      gfx::Insets::TLBR(0, 0, 0, kInsetOffsetsForThrobber);
  const gfx::Insets button_insets =
      add_insets ? button_->GetInsets() + insets_offset
                 : button_->GetInsets() - insets_offset;
  button_->SetBorder(views::CreateEmptyBorder(button_insets));
  button_->SizeToPreferredSize();
}
