/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_INFOBARS_HNS_CONFIRM_INFOBAR_CREATOR_H_
#define HNS_BROWSER_INFOBARS_HNS_CONFIRM_INFOBAR_CREATOR_H_

#include <memory>

class HnsConfirmInfoBarDelegate;

namespace infobars {
class InfoBar;
}  // namespace infobars

std::unique_ptr<infobars::InfoBar> CreateHnsConfirmInfoBar(
    std::unique_ptr<HnsConfirmInfoBarDelegate> delegate);

#endif  // HNS_BROWSER_INFOBARS_HNS_CONFIRM_INFOBAR_CREATOR_H_
