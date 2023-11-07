/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_API_HNS_TALK_API_H_
#define HNS_BROWSER_EXTENSIONS_API_HNS_TALK_API_H_

#include <map>
#include <string>

#include "extensions/browser/extension_function.h"

class Profile;

namespace extensions {
namespace api {

class HnsTalkIsSupportedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsTalk.isSupported", UNKNOWN)

 protected:
  ~HnsTalkIsSupportedFunction() override {}
  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_API_HNS_TALK_API_H_
