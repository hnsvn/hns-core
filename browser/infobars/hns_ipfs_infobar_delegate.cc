/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/infobars/hns_ipfs_infobar_delegate.h"

#include <algorithm>
#include <utility>

#include "hns/browser/ui/views/infobars/hns_confirm_infobar.h"
#include "hns/components/ipfs/ipfs_constants.h"
#include "hns/components/ipfs/pref_names.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "components/infobars/core/infobar.h"
#include "components/prefs/pref_service.h"
#include "ui/views/vector_icons.h"

// HnsIPFSInfoBarDelegateObserver
HnsIPFSInfoBarDelegateObserver::HnsIPFSInfoBarDelegateObserver() = default;

HnsIPFSInfoBarDelegateObserver::~HnsIPFSInfoBarDelegateObserver() = default;

// HnsIPFSInfoBarDelegate
// static
void HnsIPFSInfoBarDelegate::Create(
    infobars::ContentInfoBarManager* infobar_manager,
    std::unique_ptr<HnsIPFSInfoBarDelegateObserver> observer,
    PrefService* local_state) {
  if (!local_state->GetBoolean(kShowIPFSPromoInfobar)) {
    return;
  }
  infobar_manager->AddInfoBar(std::make_unique<HnsConfirmInfoBar>(
                                  std::make_unique<HnsIPFSInfoBarDelegate>(
                                      std::move(observer), local_state)),
                              true);
}

HnsIPFSInfoBarDelegate::HnsIPFSInfoBarDelegate(
    std::unique_ptr<HnsIPFSInfoBarDelegateObserver> observer,
    PrefService* local_state)
    : observer_(std::move(observer)), local_state_(local_state) {}

HnsIPFSInfoBarDelegate::~HnsIPFSInfoBarDelegate() {}

// HnsConfirmInfoBarDelegate
bool HnsIPFSInfoBarDelegate::HasCheckbox() const {
  return false;
}

std::u16string HnsIPFSInfoBarDelegate::GetCheckboxText() const {
  NOTREACHED_NORETURN();
}

void HnsIPFSInfoBarDelegate::SetCheckboxChecked(bool checked) {
  NOTREACHED();
}

bool HnsIPFSInfoBarDelegate::InterceptClosing() {
  return false;
}

// ConfirmInfoBarDelegate
infobars::InfoBarDelegate::InfoBarIdentifier
HnsIPFSInfoBarDelegate::GetIdentifier() const {
  return HNS_IPFS_INFOBAR_DELEGATE;
}

const gfx::VectorIcon& HnsIPFSInfoBarDelegate::GetVectorIcon() const {
  return views::kInfoIcon;
}

bool HnsIPFSInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  return details.is_navigation_to_different_page;
}

void HnsIPFSInfoBarDelegate::InfoBarDismissed() {}

std::u16string HnsIPFSInfoBarDelegate::GetMessageText() const {
  return hns_l10n::GetLocalizedResourceUTF16String(
      IDS_HNS_IPFS_INFOBAR_TEXT);
}

int HnsIPFSInfoBarDelegate::GetButtons() const {
  return BUTTON_OK | BUTTON_CANCEL | BUTTON_EXTRA;
}

bool HnsIPFSInfoBarDelegate::IsProminent(int id) const {
  return id == BUTTON_OK || id == BUTTON_EXTRA;
}

std::u16string HnsIPFSInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  switch (button) {
    case InfoBarButton::BUTTON_OK:
      return hns_l10n::GetLocalizedResourceUTF16String(
          IDS_HNS_IPFS_INFOBAR_APPROVE);
    case InfoBarButton::BUTTON_EXTRA:
      return hns_l10n::GetLocalizedResourceUTF16String(
          IDS_HNS_IPFS_INFOBAR_APPROVE_ONCE);
    case InfoBarButton::BUTTON_CANCEL:
      return hns_l10n::GetLocalizedResourceUTF16String(
          IDS_HNS_IPFS_INFOBAR_NEVER);
    default:
      NOTREACHED_NORETURN();
  }
}

std::vector<int> HnsIPFSInfoBarDelegate::GetButtonsOrder() const {
  return {InfoBarButton::BUTTON_OK, InfoBarButton::BUTTON_EXTRA,
          InfoBarButton::BUTTON_CANCEL};
}

std::u16string HnsIPFSInfoBarDelegate::GetLinkText() const {
  return hns_l10n::GetLocalizedResourceUTF16String(
      IDS_HNS_IPFS_INFOBAR_LINK);
}

GURL HnsIPFSInfoBarDelegate::GetLinkURL() const {
  return GURL(ipfs::kIPFSLearnMorePrivacyURL);
}

bool HnsIPFSInfoBarDelegate::Accept() {
  if (observer_) {
    local_state_->SetBoolean(kShowIPFSPromoInfobar, false);
    observer_->OnRedirectToIPFS(true);
  }
  return true;
}

bool HnsIPFSInfoBarDelegate::ExtraButtonPressed() {
  if (observer_) {
    observer_->OnRedirectToIPFS(false);
  }
  return true;
}

bool HnsIPFSInfoBarDelegate::Cancel() {
  local_state_->SetBoolean(kShowIPFSPromoInfobar, false);
  return true;
}
