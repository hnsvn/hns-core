/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ADS_SPACER_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ADS_SPACER_VIEW_H_

namespace views {
class View;
}  // namespace views

namespace hns_ads {

views::View* CreateFlexibleSpacerView(const int spacing);
views::View* CreateFixedSizeSpacerView(const int spacing);

}  // namespace hns_ads

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ADS_SPACER_VIEW_H_
