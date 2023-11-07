/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/api/hns_talk_api.h"

#include <memory>
#include <string>

#include "base/environment.h"
#include "chrome/browser/profiles/profile.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction HnsTalkIsSupportedFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());

  if (profile->IsTor()) {
    return RespondNow(Error("Not available in Tor profile"));
  }

  return RespondNow(WithArguments(true));
}

}  // namespace api
}  // namespace extensions
