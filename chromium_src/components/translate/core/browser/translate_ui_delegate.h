/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_TRANSLATE_CORE_BROWSER_TRANSLATE_UI_DELEGATE_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_TRANSLATE_CORE_BROWSER_TRANSLATE_UI_DELEGATE_H_

#define TranslateUIDelegate TranslateUIDelegate_ChromiumImpl
#define ShouldShowAlwaysTranslateShortcut \
  virtual ShouldShowAlwaysTranslateShortcut
#define ShouldAutoAlwaysTranslate virtual ShouldAutoAlwaysTranslate
#include "src/components/translate/core/browser/translate_ui_delegate.h"  // IWYU pragma: export
#undef ShouldAutoAlwaysTranslate
#undef ShouldShowAlwaysTranslateShortcut
#undef TranslateUIDelegate

namespace translate {

class TranslateUIDelegate final : public TranslateUIDelegate_ChromiumImpl {
 public:
  using TranslateUIDelegate_ChromiumImpl::TranslateUIDelegate_ChromiumImpl;
  bool ShouldShowAlwaysTranslateShortcut() const override;

#if BUILDFLAG(IS_ANDROID) || BUILDFLAG(IS_IOS)
  bool ShouldAutoAlwaysTranslate() override;
#endif  // BUILDFLAG(IS_ANDROID) || BUILDFLAG(IS_IOS)
};

}  // namespace translate

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_TRANSLATE_CORE_BROWSER_TRANSLATE_UI_DELEGATE_H_
