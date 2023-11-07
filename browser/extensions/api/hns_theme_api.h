/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_API_HNS_THEME_API_H_
#define HNS_BROWSER_EXTENSIONS_API_HNS_THEME_API_H_

#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class HnsThemeGetHnsThemeListFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsTheme.getHnsThemeList", UNKNOWN)

 protected:
  ~HnsThemeGetHnsThemeListFunction() override {}

  ResponseAction Run() override;
};

class HnsThemeGetHnsThemeTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsTheme.getHnsThemeType", UNKNOWN)

 protected:
  ~HnsThemeGetHnsThemeTypeFunction() override {}

  ResponseAction Run() override;
};

class HnsThemeSetHnsThemeTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsTheme.setHnsThemeType", UNKNOWN)

 protected:
  ~HnsThemeSetHnsThemeTypeFunction() override {}

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_API_HNS_THEME_API_H_
