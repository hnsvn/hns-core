/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_THEMES_HNS_PRIVATE_WINDOW_THEME_SUPPLIER_H_
#define HNS_BROWSER_THEMES_HNS_PRIVATE_WINDOW_THEME_SUPPLIER_H_

#include "chrome/browser/themes/custom_theme_supplier.h"

// A theme supplier for private (or tor) window.
class HnsPrivateWindowThemeSupplier : public CustomThemeSupplier {
 public:
  explicit HnsPrivateWindowThemeSupplier(bool private_window);

  HnsPrivateWindowThemeSupplier(const HnsPrivateWindowThemeSupplier&) =
      delete;
  HnsPrivateWindowThemeSupplier& operator=(
      const HnsPrivateWindowThemeSupplier&) = delete;

 protected:
  ~HnsPrivateWindowThemeSupplier() override;

  void AddColorMixers(ui::ColorProvider* provider,
                      const ui::ColorProviderKey& key) const override;

  // false if this is for tor window.
  bool for_private_window_ = true;
};

#endif  // HNS_BROWSER_THEMES_HNS_PRIVATE_WINDOW_THEME_SUPPLIER_H_
