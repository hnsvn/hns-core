/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_INFOBARS_CORE_HNS_CONFIRM_INFOBAR_DELEGATE_H_
#define HNS_COMPONENTS_INFOBARS_CORE_HNS_CONFIRM_INFOBAR_DELEGATE_H_

#include <string>
#include <vector>

#include "components/infobars/core/confirm_infobar_delegate.h"

class HnsConfirmInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  HnsConfirmInfoBarDelegate(const HnsConfirmInfoBarDelegate&) = delete;
  HnsConfirmInfoBarDelegate& operator=(const HnsConfirmInfoBarDelegate&) =
      delete;
  ~HnsConfirmInfoBarDelegate() override;

  virtual bool HasCheckbox() const;
  virtual std::u16string GetCheckboxText() const;
  virtual void SetCheckboxChecked(bool checked);
  // Returns true when delegate wants to intercept closing.
  // Then closing will be cancelled and delegate should remove infobar
  // after doing something.
  virtual bool InterceptClosing();
  virtual std::vector<int> GetButtonsOrder() const;
  virtual bool IsProminent(int id) const;

  int GetButtons() const override;

 protected:
  HnsConfirmInfoBarDelegate();
};

#endif  // HNS_COMPONENTS_INFOBARS_CORE_HNS_CONFIRM_INFOBAR_DELEGATE_H_
