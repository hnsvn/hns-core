/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/farbling/hns_session_cache.h"
#include "third_party/blink/public/platform/web_content_settings_client.h"

#define HNS_CANVAS_ASYNC_BLOB_CREATOR                    \
  hns::HnsSessionCache::From(*context_).PerturbPixels( \
      static_cast<const unsigned char*>(src_data_.addr()), \
      src_data_.computeByteSize());

#include "src/third_party/blink/renderer/core/html/canvas/canvas_async_blob_creator.cc"

#undef HNS_CANVAS_ASYNC_BLOB_CREATOR
