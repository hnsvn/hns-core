/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/infobars/sync_cannot_run_infobar_delegate.h"

#include <memory>
#include <string>
#include <utility>

#include "base/memory/raw_ptr.h"
#include "hns/browser/ui/hns_pages.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/infobars/confirm_infobar_creator.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/infobar.h"
#include "ui/views/vector_icons.h"

namespace {

bool SeedDecryptionFailed(raw_ptr<hns_sync::Prefs> hns_sync_prefs) {
  CHECK_NE(hns_sync_prefs, nullptr);
  bool failed_to_decrypt = false;
  std::string seed = hns_sync_prefs->GetSeed(&failed_to_decrypt);
  return failed_to_decrypt;
}

}  // namespace

// static
void SyncCannotRunInfoBarDelegate::Create(
    infobars::ContentInfoBarManager* infobar_manager,
    Profile* profile,
    Browser* browser) {
  hns_sync::Prefs hns_sync_prefs(profile->GetPrefs());
  if (hns_sync_prefs.IsFailedDecryptSeedNoticeDismissed()) {
    return;
  }

  if (!SeedDecryptionFailed(&hns_sync_prefs)) {
    return;
  }

  infobar_manager->AddInfoBar(
      CreateConfirmInfoBar(std::unique_ptr<ConfirmInfoBarDelegate>(
          new SyncCannotRunInfoBarDelegate(browser, profile))));
}

// Start class impl
SyncCannotRunInfoBarDelegate::SyncCannotRunInfoBarDelegate(Browser* browser,
                                                           Profile* profile)
    : profile_(profile), browser_(browser) {}

SyncCannotRunInfoBarDelegate::~SyncCannotRunInfoBarDelegate() = default;

infobars::InfoBarDelegate::InfoBarIdentifier
SyncCannotRunInfoBarDelegate::GetIdentifier() const {
  return SYNC_CANNOT_RUN_INFOBAR;
}

const gfx::VectorIcon& SyncCannotRunInfoBarDelegate::GetVectorIcon() const {
  return views::kInfoIcon;
}

bool SyncCannotRunInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  return false;
}

void SyncCannotRunInfoBarDelegate::InfoBarDismissed() {
  // Small cross on right top was pressed
}

std::u16string SyncCannotRunInfoBarDelegate::GetMessageText() const {
  return hns_l10n::GetLocalizedResourceUTF16String(
      IDS_HNS_SYNC_CANNOT_RUN_INFOBAR_MESSAGE);
}

int SyncCannotRunInfoBarDelegate::GetButtons() const {
  return BUTTON_OK | BUTTON_CANCEL;
}

std::u16string SyncCannotRunInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  if (button == BUTTON_CANCEL) {
    return hns_l10n::GetLocalizedResourceUTF16String(
        IDS_HNS_SYNC_CANNOT_RUN_INFOBAR_DONT_SHOW_BUTTON);
  }

  DCHECK(button == BUTTON_OK);

  return hns_l10n::GetLocalizedResourceUTF16String(
      IDS_HNS_SYNC_CANNOT_RUN_INFOBAR_CHECK_DETAILS_BUTTON);
}

bool SyncCannotRunInfoBarDelegate::Accept() {
  // "Check details" button
  hns::ShowSync(browser_);
  return true;
}

bool SyncCannotRunInfoBarDelegate::Cancel() {
  // "Don't show again" button
  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  hns_sync_prefs.DismissFailedDecryptSeedNotice();
  return true;
}
