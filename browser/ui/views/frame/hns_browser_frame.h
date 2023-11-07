/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "build/build_config.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "ui/color/color_provider_key.h"

class CustomThemeSupplier;

class HnsBrowserFrame : public BrowserFrame {
 public:
  explicit HnsBrowserFrame(BrowserView* browser_view);
  HnsBrowserFrame(const HnsBrowserFrame&) = delete;
  HnsBrowserFrame& operator=(const HnsBrowserFrame&) = delete;
  ~HnsBrowserFrame() override;

  const BrowserView* browser_view() const { return view_; }

  // BrowserFrame overrides:
#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC)
  const ui::NativeTheme* GetNativeTheme() const override;
#endif
  ui::ColorProviderKey::ThemeInitializerSupplier* GetCustomTheme()
      const override;
  views::internal::RootView* CreateRootView() override;

 private:
  raw_ptr<BrowserView> view_ = nullptr;
  scoped_refptr<CustomThemeSupplier> theme_supplier_;
};

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_BROWSER_FRAME_H_
