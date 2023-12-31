/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/translate/core/common/hns_translate_constants.h"

namespace translate {

const char kHnsTranslateOrigin[] = "https://translate.hns.com";
const char kHnsTranslateScriptURL[] =
    "https://translate.hns.com/static/v1/element.js";

// The used version of translate static resources (js/css files).
// Used in hns_translate.js as a replacement to /translate_static/ part in
// original script URLs.
const char kHnsTranslateStaticPath[] = "/static/v1/";

}  // namespace translate
