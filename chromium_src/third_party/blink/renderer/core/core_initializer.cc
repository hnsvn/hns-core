/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/farbling/hns_session_cache.h"
#include "third_party/blink/renderer/bindings/core/v8/binding_security.h"

#define BindingSecurity             \
  hns::HnsSessionCache::Init(); \
  BindingSecurity

#include "src/third_party/blink/renderer/core/core_initializer.cc"
