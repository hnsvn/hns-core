/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/infobars/hns_sync_account_deleted_infobar_delegate.h"

#include <memory>
#include <utility>

#include "base/memory/ptr_util.h"
#include "hns/browser/ui/views/infobars/hns_sync_account_deleted_infobar.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/infobars/confirm_infobar_creator.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/infobar.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/views/vector_icons.h"

// static
void HnsSyncAccountDeletedInfoBarDelegate::Create(
    content::WebContents* active_web_contents,
    Profile* profile,
    Browser* browser) {
  hns_sync::Prefs hns_sync_prefs(profile->GetPrefs());
  const bool notification_pending =
      hns_sync_prefs.IsSyncAccountDeletedNoticePending();
  if (!notification_pending) {
    return;
  }

  // If we already are on hns://settings/hnsSync/setup page, don't show
  // informer
  if (!active_web_contents || active_web_contents->GetURL() ==
                                  chrome::GetSettingsUrl(kHnsSyncSetupPath)) {
    return;
  }

  infobars::ContentInfoBarManager* infobar_manager =
      infobars::ContentInfoBarManager::FromWebContents(active_web_contents);

  if (!infobar_manager) {
    return;
  }

  // Create custom confirm infobar
  std::unique_ptr<infobars::InfoBar> infobar(
      std::make_unique<HnsSyncAccountDeletedInfoBar>(
          base::WrapUnique<ConfirmInfoBarDelegate>(
              new HnsSyncAccountDeletedInfoBarDelegate(browser, profile))));

  // Show infobar
  infobar_manager->AddInfoBar(std::move(infobar));
}

// Start class impl
HnsSyncAccountDeletedInfoBarDelegate::HnsSyncAccountDeletedInfoBarDelegate(
    Browser* browser,
    Profile* profile)
    : ConfirmInfoBarDelegate(), profile_(profile), browser_(browser) {}

HnsSyncAccountDeletedInfoBarDelegate::
    ~HnsSyncAccountDeletedInfoBarDelegate() {}

infobars::InfoBarDelegate::InfoBarIdentifier
HnsSyncAccountDeletedInfoBarDelegate::GetIdentifier() const {
  return HNS_SYNC_ACCOUNT_DELETED_INFOBAR;
}

const gfx::VectorIcon& HnsSyncAccountDeletedInfoBarDelegate::GetVectorIcon()
    const {
  return views::kInfoIcon;
}

bool HnsSyncAccountDeletedInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  return false;
}

void HnsSyncAccountDeletedInfoBarDelegate::InfoBarDismissed() {
  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  hns_sync_prefs.SetSyncAccountDeletedNoticePending(false);
}

std::u16string HnsSyncAccountDeletedInfoBarDelegate::GetMessageText() const {
  // The replacement with empty string here is required to eat placeholder $1
  // in grit string resource. And it's impossible to have empty placeholder
  // <ph name="NAME"></ph>, grit compiler gives error. Placeholder is required
  // to explane translation team that message string and link text are part of
  // the same sentense.
  return l10n_util::GetStringFUTF16(
      IDS_HNS_SYNC_ACCOUNT_DELETED_INFOBAR_MESSAGE, u"");
}

int HnsSyncAccountDeletedInfoBarDelegate::GetButtons() const {
  return BUTTON_OK;
}

std::u16string HnsSyncAccountDeletedInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  return hns_l10n::GetLocalizedResourceUTF16String(
      IDS_HNS_SYNC_ACCOUNT_DELETED_INFOBAR_BUTTON);
}

std::u16string HnsSyncAccountDeletedInfoBarDelegate::GetLinkText() const {
  // See comment at |HnsSyncAccountDeletedInfoBarDelegate::GetMessageText|
  // above for empty substitution
  return l10n_util::GetStringFUTF16(
      IDS_HNS_SYNC_ACCOUNT_DELETED_INFOBAR_LINK_TEXT, u"");
}

GURL HnsSyncAccountDeletedInfoBarDelegate::GetLinkURL() const {
  return chrome::GetSettingsUrl(kHnsSyncSetupPath);
}

bool HnsSyncAccountDeletedInfoBarDelegate::Accept() {
  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  hns_sync_prefs.SetSyncAccountDeletedNoticePending(false);
  return true;
}

bool HnsSyncAccountDeletedInfoBarDelegate::LinkClicked(
    WindowOpenDisposition disposition) {
  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  hns_sync_prefs.SetSyncAccountDeletedNoticePending(false);
  InfoBarDelegate::LinkClicked(disposition);
  return true;
}

bool HnsSyncAccountDeletedInfoBarDelegate::IsCloseable() const {
  return false;
}
