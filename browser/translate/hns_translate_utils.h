/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_TRANSLATE_HNS_TRANSLATE_UTILS_H_
#define HNS_BROWSER_TRANSLATE_HNS_TRANSLATE_UTILS_H_

namespace content {
class BrowserContext;
}
namespace translate {

bool IsTranslateExtensionEnabled(content::BrowserContext* context);

}  // namespace translate

#endif  // HNS_BROWSER_TRANSLATE_HNS_TRANSLATE_UTILS_H_
