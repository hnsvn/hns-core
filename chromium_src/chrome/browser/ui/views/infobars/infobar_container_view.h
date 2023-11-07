/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_CONTAINER_VIEW_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_CONTAINER_VIEW_H_

#include "components/infobars/core/infobar_container.h"

#define PlatformSpecificInfoBarStateChanged       \
  PlatformSpecificInfoBarStateChanged_UnUsed() {} \
  friend class HnsInfoBarContainerView;         \
  void PlatformSpecificInfoBarStateChanged

#include "src/chrome/browser/ui/views/infobars/infobar_container_view.h"  // IWYU pragma: export

#undef PlatformSpecificInfoBarStateChanged

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_CONTAINER_VIEW_H_