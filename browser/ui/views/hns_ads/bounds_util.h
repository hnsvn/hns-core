/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ADS_BOUNDS_UTIL_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ADS_BOUNDS_UTIL_H_

#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Rect;
}  // namespace gfx

namespace display {
class Display;
}  // namespace display

namespace views {
class Widget;
}  // namespace views

namespace hns_ads {

gfx::Rect GetDefaultDisplayScreenWorkArea(gfx::NativeView browser_native_view);

void AdjustBoundsAndSnapToFitWorkAreaForNativeView(views::Widget* widget,
                                                   gfx::Rect* bounds);

// Exposed here to be available in tests.
void SnapBoundsToEdgeOfWorkArea(const gfx::Rect& work_area, gfx::Rect* bounds);

}  // namespace hns_ads

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ADS_BOUNDS_UTIL_H_
