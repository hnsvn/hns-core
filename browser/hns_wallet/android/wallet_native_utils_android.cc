/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "base/logging.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/build/android/jni_headers/WalletNativeUtils_jni.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/decentralized_dns/core/utils.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"

namespace chrome {
namespace android {

static void JNI_WalletNativeUtils_ResetWallet(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& profile_android) {
  auto* profile = ProfileAndroid::FromProfileAndroid(profile_android);
  DCHECK(profile);

  auto* hns_wallet_service =
      hns_wallet::HnsWalletServiceFactory::GetServiceForContext(profile);
  if (hns_wallet_service) {
    hns_wallet_service->Reset();
  }
}

static jboolean JNI_WalletNativeUtils_IsUnstoppableDomainsTld(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& domain) {
  auto domain_string = base::android::ConvertJavaStringToUTF8(env, domain);

  return decentralized_dns::IsUnstoppableDomainsTLD(domain_string);
}

static jboolean JNI_WalletNativeUtils_IsEnsTld(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& domain) {
  auto domain_string = base::android::ConvertJavaStringToUTF8(env, domain);

  return decentralized_dns::IsENSTLD(domain_string);
}

static jboolean JNI_WalletNativeUtils_IsSnsTld(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& domain) {
  auto domain_string = base::android::ConvertJavaStringToUTF8(env, domain);

  return decentralized_dns::IsSnsTLD(domain_string);
}

}  // namespace android
}  // namespace chrome
