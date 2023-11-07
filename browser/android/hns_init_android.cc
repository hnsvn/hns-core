// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <string>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "hns/browser/android/safe_browsing/buildflags.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/hns_stats/hns_stats_updater.h"
#include "hns/build/android/jni_headers/HnsActivity_jni.h"

namespace chrome {
namespace android {

static void JNI_HnsActivity_RestartStatsUpdater(JNIEnv* env) {
  g_hns_browser_process->hns_stats_updater()->Stop();
  g_hns_browser_process->hns_stats_updater()->Start();
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_HnsActivity_GetSafeBrowsingApiKey(JNIEnv* env) {
  return base::android::ConvertUTF8ToJavaString(
      env, BUILDFLAG(SAFEBROWSING_API_KEY));
}

}  // namespace android
}  // namespace chrome
