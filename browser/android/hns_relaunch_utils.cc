/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/build/android/jni_headers/HnsRelaunchUtils_jni.h"

#include "chrome/browser/lifetime/application_lifetime.h"

namespace chrome {
namespace android {

void JNI_HnsRelaunchUtils_Restart(JNIEnv* env) {
  chrome::AttemptRestart();
}

}  // namespace android
}  // namespace chrome
