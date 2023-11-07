/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_WEBUI_SOURCE_H_
#define HNS_BROWSER_UI_WEBUI_HNS_WEBUI_SOURCE_H_

#include <string>

#include "build/build_config.h"

namespace content {
class WebContents;
class WebUI;
class WebUIDataSource;
}  // namespace content

namespace webui {
struct ResourcePath;
}  // namespace webui

// Add hns resource path mapping and localized strings to new data source.
content::WebUIDataSource* CreateAndAddWebUIDataSource(
    content::WebUI* web_ui,
    const std::string& name,
    const webui::ResourcePath* resource_map,
    size_t resouece_map_size,
    int html_resource_id,
    bool disable_trusted_types_csp = false);

#if !BUILDFLAG(IS_ANDROID)

// Provide html with background color so we can avoid flash of
// different colors as the page loads, especially for New Tab Pages.
void AddBackgroundColorToSource(content::WebUIDataSource* source,
                                content::WebContents* contents);

#endif  // !BUILDFLAG(IS_ANDROID)

#endif  // HNS_BROWSER_UI_WEBUI_HNS_WEBUI_SOURCE_H_
