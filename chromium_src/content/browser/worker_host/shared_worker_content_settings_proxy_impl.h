/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CONTENT_BROWSER_WORKER_HOST_SHARED_WORKER_CONTENT_SETTINGS_PROXY_IMPL_H_
#define HNS_CHROMIUM_SRC_CONTENT_BROWSER_WORKER_HOST_SHARED_WORKER_CONTENT_SETTINGS_PROXY_IMPL_H_

// We explicitly include ancestor class here so our function redeclaration only
// affects this class.
#include "third_party/blink/public/mojom/worker/worker_content_settings_proxy.mojom.h"

#define RequestFileSystemAccessSync                                       \
  GetHnsFarblingLevel(GetHnsFarblingLevelCallback callback) override; \
  void RequestFileSystemAccessSync

#include "src/content/browser/worker_host/shared_worker_content_settings_proxy_impl.h"  // IWYU pragma: export

#undef RequestFileSystemAccessSync

#endif  // HNS_CHROMIUM_SRC_CONTENT_BROWSER_WORKER_HOST_SHARED_WORKER_CONTENT_SETTINGS_PROXY_IMPL_H_
