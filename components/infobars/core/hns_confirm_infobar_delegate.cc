/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/infobars/core/hns_confirm_infobar_delegate.h"

HnsConfirmInfoBarDelegate::HnsConfirmInfoBarDelegate() = default;
HnsConfirmInfoBarDelegate::~HnsConfirmInfoBarDelegate() = default;

int HnsConfirmInfoBarDelegate::GetButtons() const {
  return BUTTON_OK | BUTTON_CANCEL | BUTTON_EXTRA;
}

std::vector<int> HnsConfirmInfoBarDelegate::GetButtonsOrder() const {
  return {BUTTON_OK | BUTTON_EXTRA | BUTTON_CANCEL};
}

bool HnsConfirmInfoBarDelegate::IsProminent(int id) const {
  return id == BUTTON_OK;
}

bool HnsConfirmInfoBarDelegate::HasCheckbox() const {
  return false;
}

std::u16string HnsConfirmInfoBarDelegate::GetCheckboxText() const {
  return std::u16string();
}

void HnsConfirmInfoBarDelegate::SetCheckboxChecked(bool checked) {}

bool HnsConfirmInfoBarDelegate::InterceptClosing() {
  return false;
}
