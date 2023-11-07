/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "android_hns_referrer.h"

#include "base/android/jni_android.h"
#include "hns/components/hns_referrals/browser/jni_headers/HnsReferrer_jni.h"

namespace android_hns_referrer {

HnsReferrer::HnsReferrer() {
  JNIEnv* env = base::android::AttachCurrentThread();
  java_obj_.Reset(
      env,
      Java_HnsReferrer_create(env, reinterpret_cast<intptr_t>(this)).obj());
}

HnsReferrer::~HnsReferrer() {
  Java_HnsReferrer_destroy(base::android::AttachCurrentThread(), java_obj_);
}

void HnsReferrer::InitReferrer(InitReferrerCallback init_referrer_callback) {
  init_referrer_callback_ = std::move(init_referrer_callback);
  JNIEnv* env = base::android::AttachCurrentThread();
  return Java_HnsReferrer_initReferrer(env, java_obj_);
}

void HnsReferrer::OnReferrerReady(JNIEnv* env) {
  std::move(init_referrer_callback_).Run();
}

}  // namespace android_hns_referrer
