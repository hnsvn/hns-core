/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_SYNC_ACCOUNT_DELETED_INFOBAR_H_
#define HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_SYNC_ACCOUNT_DELETED_INFOBAR_H_

#include <memory>

#include "chrome/browser/ui/views/infobars/confirm_infobar.h"

// The customized ConfirmInfoBar:
// "Text _link_                     [ok_button]"
// cancel_button is not supported

class HnsSyncAccountDeletedInfoBar : public ConfirmInfoBar {
 public:
  explicit HnsSyncAccountDeletedInfoBar(
      std::unique_ptr<ConfirmInfoBarDelegate> delegate);

  HnsSyncAccountDeletedInfoBar(const HnsSyncAccountDeletedInfoBar&) =
      delete;
  HnsSyncAccountDeletedInfoBar& operator=(
      const HnsSyncAccountDeletedInfoBar&) = delete;

  ~HnsSyncAccountDeletedInfoBar() override;

  // InfoBarView:
  void Layout() override;
};

#endif  // HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_SYNC_ACCOUNT_DELETED_INFOBAR_H_
