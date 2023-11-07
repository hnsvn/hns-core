/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WAYBACK_MACHINE_HNS_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_
#define HNS_COMPONENTS_HNS_WAYBACK_MACHINE_HNS_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_

#include <memory>

#include "components/infobars/core/infobar_delegate.h"

class HnsWaybackMachineInfoBarDelegate : public infobars::InfoBarDelegate {
 public:
  HnsWaybackMachineInfoBarDelegate();
  ~HnsWaybackMachineInfoBarDelegate() override;

  HnsWaybackMachineInfoBarDelegate(
      const HnsWaybackMachineInfoBarDelegate&) = delete;
  HnsWaybackMachineInfoBarDelegate& operator=(
      const HnsWaybackMachineInfoBarDelegate&) = delete;

 private:
  // infobars::InfoBarDelegate overrides:
  InfoBarIdentifier GetIdentifier() const override;
  bool EqualsDelegate(
      infobars::InfoBarDelegate* delegate) const override;
};

#endif  // HNS_COMPONENTS_HNS_WAYBACK_MACHINE_HNS_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_
