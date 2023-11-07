/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/api/greaselion_api.h"

#include <memory>

#include "base/values.h"
#include "hns/browser/greaselion/greaselion_service_factory.h"
#include "hns/common/extensions/api/greaselion.h"
#include "hns/components/greaselion/browser/greaselion_service.h"
#include "chrome/browser/profiles/profile.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction
GreaselionIsGreaselionExtensionFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::greaselion::GreaselionService* greaselion_service =
      ::greaselion::GreaselionServiceFactory::GetForBrowserContext(profile);
  if (!greaselion_service) {
    return RespondNow(WithArguments(false));
  }

  absl::optional<greaselion::IsGreaselionExtension::Params> params =
      greaselion::IsGreaselionExtension::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  bool result = greaselion_service->IsGreaselionExtension(params->id);
  return RespondNow(WithArguments(result));
}

}  // namespace api
}  // namespace extensions
