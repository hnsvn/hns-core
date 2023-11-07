/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import <Foundation/Foundation.h>

#ifndef HNS_IOS_BROWSER_SVG_SVG_IMAGE_H_
#define HNS_IOS_BROWSER_SVG_SVG_IMAGE_H_

class SkBitmap;

namespace SVGImage {

/// Creates a Scaled `SkBitmap` from SVG `data`
/// Width and Height are the desired dimensions when scaling
SkBitmap MakeFromData(const NSData* data, size_t width, size_t height);
}  // namespace SVGImage

#endif  // HNS_IOS_BROWSER_SVG_SVG_IMAGE_H_
