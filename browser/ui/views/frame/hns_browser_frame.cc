/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/frame/hns_browser_frame.h"

#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/themes/hns_private_window_theme_supplier.h"
#include "hns/browser/ui/views/frame/hns_browser_root_view.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/color/color_provider_key.h"

HnsBrowserFrame::HnsBrowserFrame(BrowserView* browser_view)
    : BrowserFrame(browser_view), view_(browser_view) {
  if (view_->browser()->profile()->IsIncognitoProfile() ||
      view_->browser()->profile()->IsTor() ||
      view_->browser()->profile()->IsGuestSession()) {
    theme_supplier_ = base::MakeRefCounted<HnsPrivateWindowThemeSupplier>(
        !view_->browser()->profile()->IsTor());
  }
}

HnsBrowserFrame::~HnsBrowserFrame() = default;

#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC)
// Tor/Guest profile should use DarkAura. If not, their native ui is affected by
// normal windows theme change.
const ui::NativeTheme* HnsBrowserFrame::GetNativeTheme() const {
  if ((view_->browser()->profile()->IsIncognitoProfile() ||
       view_->browser()->profile()->IsTor() ||
       hns::IsGuestProfile(view_->browser()->profile())) &&
      ThemeServiceFactory::GetForProfile(view_->browser()->profile())
          ->UsingDefaultTheme()) {
    return ui::NativeTheme::GetInstanceForDarkUI();
  }
  return views::Widget::GetNativeTheme();
}
#endif

ui::ColorProviderKey::ThemeInitializerSupplier*
HnsBrowserFrame::GetCustomTheme() const {
  // To provider private(tor) windows's theme color via color provider,
  // we use |theme_supplier_| for both as upstream doesn't use separated
  // mix for private window.
  if (theme_supplier_) {
    return theme_supplier_.get();
  }

  return BrowserFrame::GetCustomTheme();
}

views::internal::RootView* HnsBrowserFrame::CreateRootView() {
  root_view_ = new HnsBrowserRootView(browser_view_, this);
  return root_view_;
}
