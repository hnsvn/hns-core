/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_FONTS_FONT_FALLBACK_LIST_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_FONTS_FONT_FALLBACK_LIST_H_

#include "base/functional/callback.h"
#include "src/third_party/blink/renderer/platform/fonts/font_fallback_list.h"  // IWYU pragma: export
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {
class ExecutionContext;
}

namespace hns {

typedef base::RepeatingCallback<bool(blink::ExecutionContext*,
                                     const WTF::AtomicString&)>
    AllowFontFamilyCallback;

PLATFORM_EXPORT void RegisterAllowFontFamilyCallback(
    AllowFontFamilyCallback callback);

}  // namespace hns

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_FONTS_FONT_FALLBACK_LIST_H_
