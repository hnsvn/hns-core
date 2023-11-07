/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_CANVAS_CANVAS2D_CANVAS_RENDERING_CONTEXT_2D_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_CANVAS_CANVAS2D_CANVAS_RENDERING_CONTEXT_2D_H_

#include "third_party/blink/renderer/modules/canvas/canvas2d/base_rendering_context_2d.h"

#define getImageDataInternal                                         \
  getImageDataInternal(ScriptState*, int sx, int sy, int sw, int sh, \
                       ImageDataSettings*, ExceptionState&) final;   \
  ImageData* getImageDataInternal_Unused

#include "src/third_party/blink/renderer/modules/canvas/canvas2d/canvas_rendering_context_2d.h"  // IWYU pragma: export
#undef getImageDataInternal

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_CANVAS_CANVAS2D_CANVAS_RENDERING_CONTEXT_2D_H_