// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_UI_BASE_RESOURCE_RESOURCE_BUNDLE_ANDROID_H_
#define HNS_CHROMIUM_SRC_UI_BASE_RESOURCE_RESOURCE_BUNDLE_ANDROID_H_

#include "src/ui/base/resource/resource_bundle_android.h"  // IWYU pragma: export

namespace ui {

COMPONENT_EXPORT(UI_BASE)
void HnsLoadMainAndroidPackFile(const char* path_within_apk,
                                  const base::FilePath& disk_file_path);
}

#endif  // HNS_CHROMIUM_SRC_UI_BASE_RESOURCE_RESOURCE_BUNDLE_ANDROID_H_
