/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_API_HNS_SHIELDS_API_H_
#define HNS_BROWSER_EXTENSIONS_API_HNS_SHIELDS_API_H_

#include <memory>
#include <string>

#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class HnsShieldsAddSiteCosmeticFilterFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsShields.addSiteCosmeticFilter", UNKNOWN)

 protected:
  ~HnsShieldsAddSiteCosmeticFilterFunction() override {}

  ResponseAction Run() override;
};

class HnsShieldsOpenFilterManagementPageFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsShields.openFilterManagementPage", UNKNOWN)

 protected:
  ~HnsShieldsOpenFilterManagementPageFunction() override {}

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_API_HNS_SHIELDS_API_H_
