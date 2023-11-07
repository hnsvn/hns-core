/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_FRAME_HNS_NON_CLIENT_HIT_TEST_HELPER_H_
#define HNS_BROWSER_UI_VIEWS_FRAME_HNS_NON_CLIENT_HIT_TEST_HELPER_H_

namespace gfx {
class Point;
}  // namespace gfx

class BrowserView;

namespace hns {

// Helper function to set additional draggable area in client view.
// Returns HTNOWHERE if the point is not what we're interested in. In that
// case, caller should depend on the default behavior.
int NonClientHitTest(BrowserView* browser_view,
                     const gfx::Point& point_in_widget);

}  // namespace hns

#endif  // HNS_BROWSER_UI_VIEWS_FRAME_HNS_NON_CLIENT_HIT_TEST_HELPER_H_
