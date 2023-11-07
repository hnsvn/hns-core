/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/translate/core/common/hns_translate_switches.h"

namespace translate {
namespace switches {

// A test switch to disable the redirection for the translation requests to
// translate.hns.com.
const char kHnsTranslateUseGoogleEndpoint[] = "use-google-translate-endpoint";

}  // namespace switches
}  // namespace translate
