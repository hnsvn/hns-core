/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/android/jni_string.h"
#include "hns/browser/download/android/jni_headers/HnsMimeUtils_jni.h"
#include "chrome/browser/download/android/jni_headers/MimeUtils_jni.h"

namespace {

// We need this just to avoid unused function
// 'Java_MimeUtils_canAutoOpenMimeType' error message.
bool DummyMimeUtilUsage() {
  JNIEnv* env = nullptr;
  if (Java_MimeUtils_canAutoOpenMimeType(
          env, base::android::ConvertUTF8ToJavaString(env, "")))
    return true;

  return DummyMimeUtilUsage();
}

}  // namespace

#define Java_MimeUtils_canAutoOpenMimeType \
  Java_HnsMimeUtils_canAutoOpenMimeType
#include "src/chrome/browser/download/android/download_utils.cc"
#undef Java_MimeUtils_canAutoOpenMimeType
