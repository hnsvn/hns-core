// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_UI_UTILS_H_
#define HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_UI_UTILS_H_

#include <string>

// Simply append `https://` scheme to |url| if |url| is not valid.
// Retruns true if |url| is valid or fixed |url| is valid.
// Fixed url is passed via |url|.
bool GetValidURLStringForTopSite(std::string* url);

#endif  // HNS_BROWSER_UI_WEBUI_NEW_TAB_PAGE_HNS_NEW_TAB_UI_UTILS_H_
