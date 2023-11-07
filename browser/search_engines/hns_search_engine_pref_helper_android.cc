/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/android/java/org/chromium/chrome/browser/search_engines/jni_headers/HnsSearchEnginePrefHelper_jni.h"

#include "hns/components/hns_search/browser/prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/prefs/pref_service.h"

namespace {
Profile* GetOriginalProfile() {
  return ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
}
}  // namespace

void JNI_HnsSearchEnginePrefHelper_SetFetchSEFromNative(JNIEnv* env,
                                                          jboolean value) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      hns_search::prefs::kFetchFromNative, value);
}

jboolean JNI_HnsSearchEnginePrefHelper_GetFetchSEFromNative(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      hns_search::prefs::kFetchFromNative);
}
