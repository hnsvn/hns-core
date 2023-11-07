/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REFERRALS_BROWSER_ANDROID_HNS_REFERRER_H_
#define HNS_COMPONENTS_HNS_REFERRALS_BROWSER_ANDROID_HNS_REFERRER_H_

#include <jni.h>
#include <memory>
#include <string>
#include <vector>

#include "base/android/scoped_java_ref.h"
#include "net/base/completion_once_callback.h"

namespace android_hns_referrer {

using InitReferrerCallback = base::OnceCallback<void()>;

class HnsReferrer {
 public:
  explicit HnsReferrer();
  ~HnsReferrer();

  HnsReferrer(const HnsReferrer&) = delete;
  HnsReferrer& operator=(const HnsReferrer&) = delete;

  void InitReferrer(InitReferrerCallback init_referrer_callback);
  void OnReferrerReady(JNIEnv* env);

 private:
  base::android::ScopedJavaGlobalRef<jobject> java_obj_;
  InitReferrerCallback init_referrer_callback_;
};

}  // namespace android_hns_referrer

#endif  // HNS_COMPONENTS_HNS_REFERRALS_BROWSER_ANDROID_HNS_REFERRER_H_
