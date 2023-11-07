/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ADS_INSETS_UTIL_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ADS_INSETS_UTIL_H_

namespace gfx {
class FontList;
class Insets;
}  // namespace gfx

namespace hns_ads {

void AdjustInsetsForFontList(gfx::Insets* insets,
                             const gfx::FontList& font_list);

}  // namespace hns_ads

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ADS_INSETS_UTIL_H_
