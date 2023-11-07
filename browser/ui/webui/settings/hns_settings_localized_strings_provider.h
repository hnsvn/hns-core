/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SETTINGS_LOCALIZED_STRINGS_PROVIDER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SETTINGS_LOCALIZED_STRINGS_PROVIDER_H_

class Profile;

namespace content {
class WebUIDataSource;
}

namespace settings {

void HnsAddLocalizedStrings(content::WebUIDataSource*, Profile*);

}  // namespace settings

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SETTINGS_LOCALIZED_STRINGS_PROVIDER_H_
