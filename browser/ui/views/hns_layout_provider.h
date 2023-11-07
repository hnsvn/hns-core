/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_LAYOUT_PROVIDER_H_
#define HNS_BROWSER_UI_VIEWS_HNS_LAYOUT_PROVIDER_H_

#include "chrome/browser/ui/views/chrome_layout_provider.h"

class HnsLayoutProvider : public ChromeLayoutProvider {
 public:
  HnsLayoutProvider() = default;
  HnsLayoutProvider(const HnsLayoutProvider&) = delete;
  HnsLayoutProvider& operator=(const HnsLayoutProvider&) = delete;
  ~HnsLayoutProvider() override = default;

  int GetCornerRadiusMetric(views::Emphasis emphasis,
                            const gfx::Size& size = gfx::Size()) const override;
};

#endif  // HNS_BROWSER_UI_VIEWS_HNS_LAYOUT_PROVIDER_H_
