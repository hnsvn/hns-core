/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/api/hns_theme_api.h"

#include <memory>
#include <string>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/common/extensions/api/hns_theme.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction HnsThemeGetHnsThemeListFunction::Run() {
  std::string json_string;
  base::JSONWriter::Write(dark_mode::GetHnsDarkModeTypeList(), &json_string);
  return RespondNow(WithArguments(json_string));
}

ExtensionFunction::ResponseAction HnsThemeGetHnsThemeTypeFunction::Run() {
  const std::string theme_type =
      dark_mode::GetStringFromHnsDarkModeType(
          dark_mode::GetActiveHnsDarkModeType());
  return RespondNow(WithArguments(theme_type));
}

ExtensionFunction::ResponseAction HnsThemeSetHnsThemeTypeFunction::Run() {
  absl::optional<hns_theme::SetHnsThemeType::Params> params =
      hns_theme::SetHnsThemeType::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  dark_mode::SetHnsDarkModeType(params->type);

  return RespondNow(NoArguments());
}

}  // namespace api
}  // namespace extensions
