/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/android/jni_android.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/build/android/jni_headers/HnsWalletServiceFactory_jni.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

namespace chrome {
namespace android {
static jlong JNI_HnsWalletServiceFactory_GetInterfaceToHnsWalletService(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& profile_android) {
  auto* profile = ProfileAndroid::FromProfileAndroid(profile_android);
  auto pending =
      hns_wallet::HnsWalletServiceFactory::GetInstance()->GetForContext(
          profile);

  return static_cast<jlong>(pending.PassPipe().release().value());
}

static jlong JNI_HnsWalletServiceFactory_GetInterfaceToHnsWalletP3A(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& profile_android) {
  auto* profile = ProfileAndroid::FromProfileAndroid(profile_android);
  auto pending = hns_wallet::HnsWalletServiceFactory::GetInstance()
                     ->GetServiceForContext(profile)
                     ->GetHnsWalletP3A()
                     ->MakeRemote();

  return static_cast<jlong>(pending.PassPipe().release().value());
}

}  // namespace android
}  // namespace chrome
