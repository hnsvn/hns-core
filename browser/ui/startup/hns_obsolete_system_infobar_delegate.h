/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_STARTUP_HNS_OBSOLETE_SYSTEM_INFOBAR_DELEGATE_H_
#define HNS_BROWSER_UI_STARTUP_HNS_OBSOLETE_SYSTEM_INFOBAR_DELEGATE_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "hns/components/infobars/core/hns_confirm_infobar_delegate.h"

namespace infobars {
class ContentInfoBarManager;
}  // namespace infobars

class HnsObsoleteSystemInfoBarDelegate : public HnsConfirmInfoBarDelegate {
 public:
  static void Create(infobars::ContentInfoBarManager* infobar_manager);

  HnsObsoleteSystemInfoBarDelegate(
      const HnsObsoleteSystemInfoBarDelegate&) = delete;
  HnsObsoleteSystemInfoBarDelegate& operator=(
      const HnsObsoleteSystemInfoBarDelegate&) = delete;

 private:
  HnsObsoleteSystemInfoBarDelegate();
  ~HnsObsoleteSystemInfoBarDelegate() override;

  // HnsConfirmInfoBarDelegate overrides:
  bool HasCheckbox() const override;
  std::u16string GetCheckboxText() const override;
  void SetCheckboxChecked(bool checked) override;
  bool InterceptClosing() override;
  infobars::InfoBarDelegate::InfoBarIdentifier GetIdentifier() const override;
  std::u16string GetLinkText() const override;
  GURL GetLinkURL() const override;
  std::u16string GetMessageText() const override;
  int GetButtons() const override;
  std::vector<int> GetButtonsOrder() const override;
  bool ShouldExpire(const NavigationDetails& details) const override;

  void OnConfirmDialogClosing(bool suppress);

  bool launch_confirmation_dialog_ = false;

  base::WeakPtrFactory<HnsObsoleteSystemInfoBarDelegate> weak_factory_{this};
};

#endif  // HNS_BROWSER_UI_STARTUP_HNS_OBSOLETE_SYSTEM_INFOBAR_DELEGATE_H_
