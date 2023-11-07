/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_INFOBARS_HNS_CONFIRM_P3A_INFOBAR_DELEGATE_H_
#define HNS_BROWSER_INFOBARS_HNS_CONFIRM_P3A_INFOBAR_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/memory/raw_ptr.h"
#include "components/infobars/core/confirm_infobar_delegate.h"
#include "url/gurl.h"

class PrefService;

namespace infobars {
class ContentInfoBarManager;
}  // namespace infobars

// An infobar that is run with a string, buttons, and a "Learn More" link.
class HnsConfirmP3AInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  HnsConfirmP3AInfoBarDelegate(const HnsConfirmP3AInfoBarDelegate&) =
      delete;
  HnsConfirmP3AInfoBarDelegate& operator=(
      const HnsConfirmP3AInfoBarDelegate&) = delete;

  static void Create(infobars::ContentInfoBarManager* infobar_manager,
                     PrefService* local_state);

 private:
  explicit HnsConfirmP3AInfoBarDelegate(PrefService* local_state);
  ~HnsConfirmP3AInfoBarDelegate() override;

  infobars::InfoBarDelegate::InfoBarIdentifier GetIdentifier() const override;
  const gfx::VectorIcon& GetVectorIcon() const override;
  bool ShouldExpire(const NavigationDetails& details) const override;
  void InfoBarDismissed() override;
  std::u16string GetMessageText() const override;
  int GetButtons() const override;
  std::u16string GetButtonLabel(InfoBarButton button) const override;
  std::u16string GetLinkText() const override;
  GURL GetLinkURL() const override;
  bool Accept() override;
  bool Cancel() override;

  raw_ptr<PrefService> local_state_ = nullptr;
};

#endif  // HNS_BROWSER_INFOBARS_HNS_CONFIRM_P3A_INFOBAR_DELEGATE_H_
