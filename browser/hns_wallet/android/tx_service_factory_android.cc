/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/android/jni_android.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"
#include "hns/build/android/jni_headers/TxServiceFactory_jni.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

namespace chrome {
namespace android {

static jlong JNI_TxServiceFactory_GetInterfaceToTxService(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& profile_android) {
  auto* profile = ProfileAndroid::FromProfileAndroid(profile_android);
  auto pending =
      hns_wallet::TxServiceFactory::GetInstance()->GetForContext(profile);

  return static_cast<jlong>(pending.PassPipe().release().value());
}

static jlong JNI_TxServiceFactory_GetInterfaceToEthTxManagerProxy(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& profile_android) {
  auto* profile = ProfileAndroid::FromProfileAndroid(profile_android);
  auto pending = hns_wallet::TxServiceFactory::GetInstance()
                     ->GetEthTxManagerProxyForContext(profile);

  return static_cast<jlong>(pending.PassPipe().release().value());
}

static jlong JNI_TxServiceFactory_GetInterfaceToSolanaTxManagerProxy(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& profile_android) {
  auto* profile = ProfileAndroid::FromProfileAndroid(profile_android);
  auto pending = hns_wallet::TxServiceFactory::GetInstance()
                     ->GetSolanaTxManagerProxyForContext(profile);

  return static_cast<jlong>(pending.PassPipe().release().value());
}

}  // namespace android
}  // namespace chrome
