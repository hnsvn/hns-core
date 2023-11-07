/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/android/hns_vpn_native_worker.h"

#include "base/android/jni_android.h"
#include "base/android/jni_array.h"
#include "base/android/jni_string.h"
#include "base/functional/bind.h"
#include "base/json/json_writer.h"
#include "base/values.h"
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/build/android/jni_headers/HnsVpnNativeWorker_jni.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"

using hns_vpn::HnsVpnService;

namespace {

HnsVpnService* GetHnsVpnService() {
  return hns_vpn::HnsVpnServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
}

}  // namespace

namespace chrome {
namespace android {

HnsVpnNativeWorker::HnsVpnNativeWorker(
    JNIEnv* env,
    const base::android::JavaRef<jobject>& obj)
    : weak_java_hns_vpn_native_worker_(env, obj), weak_factory_(this) {
  Java_HnsVpnNativeWorker_setNativePtr(env, obj,
                                         reinterpret_cast<intptr_t>(this));
}

HnsVpnNativeWorker::~HnsVpnNativeWorker() {}

void HnsVpnNativeWorker::Destroy(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  delete this;
}

void HnsVpnNativeWorker::GetAllServerRegions(JNIEnv* env) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->GetAllServerRegions(
        base::BindOnce(&HnsVpnNativeWorker::OnGetAllServerRegions,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsVpnNativeWorker::OnGetAllServerRegions(
    const std::string& server_regions_json,
    bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsVpnNativeWorker_onGetAllServerRegions(
      env, weak_java_hns_vpn_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, server_regions_json),
      success);
}

void HnsVpnNativeWorker::GetTimezonesForRegions(JNIEnv* env) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->GetTimezonesForRegions(
        base::BindOnce(&HnsVpnNativeWorker::OnGetTimezonesForRegions,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsVpnNativeWorker::OnGetTimezonesForRegions(
    const std::string& timezones_json,
    bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsVpnNativeWorker_onGetTimezonesForRegions(
      env, weak_java_hns_vpn_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, timezones_json), success);
}

void HnsVpnNativeWorker::GetHostnamesForRegion(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& region) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->GetHostnamesForRegion(
        base::BindOnce(&HnsVpnNativeWorker::OnGetHostnamesForRegion,
                       weak_factory_.GetWeakPtr()),
        base::android::ConvertJavaStringToUTF8(env, region));
  }
}

void HnsVpnNativeWorker::OnGetHostnamesForRegion(
    const std::string& hostnames_json,
    bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsVpnNativeWorker_onGetHostnamesForRegion(
      env, weak_java_hns_vpn_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, hostnames_json), success);
}

void HnsVpnNativeWorker::GetWireguardProfileCredentials(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& subscriber_credential,
    const base::android::JavaParamRef<jstring>& public_key,
    const base::android::JavaParamRef<jstring>& hostname) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->GetWireguardProfileCredentials(
        base::BindOnce(&HnsVpnNativeWorker::OnGetWireguardProfileCredentials,
                       weak_factory_.GetWeakPtr()),
        base::android::ConvertJavaStringToUTF8(env, subscriber_credential),
        base::android::ConvertJavaStringToUTF8(env, public_key),
        base::android::ConvertJavaStringToUTF8(env, hostname));
  }
}

void HnsVpnNativeWorker::OnGetWireguardProfileCredentials(
    const std::string& wireguard_profile_credentials_json,
    bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsVpnNativeWorker_onGetWireguardProfileCredentials(
      env, weak_java_hns_vpn_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(
          env, wireguard_profile_credentials_json),
      success);
}

void HnsVpnNativeWorker::VerifyCredentials(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& hostname,
    const base::android::JavaParamRef<jstring>& client_id,
    const base::android::JavaParamRef<jstring>& subscriber_credential,
    const base::android::JavaParamRef<jstring>& api_auth_token) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->VerifyCredentials(
        base::BindOnce(&HnsVpnNativeWorker::OnVerifyCredentials,
                       weak_factory_.GetWeakPtr()),
        base::android::ConvertJavaStringToUTF8(env, hostname),
        base::android::ConvertJavaStringToUTF8(env, client_id),
        base::android::ConvertJavaStringToUTF8(env, subscriber_credential),
        base::android::ConvertJavaStringToUTF8(env, api_auth_token));
  }
}

void HnsVpnNativeWorker::OnVerifyCredentials(
    const std::string& verify_credentials_json,
    bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsVpnNativeWorker_onVerifyCredentials(
      env, weak_java_hns_vpn_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, verify_credentials_json),
      success);
}

void HnsVpnNativeWorker::InvalidateCredentials(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& hostname,
    const base::android::JavaParamRef<jstring>& client_id,
    const base::android::JavaParamRef<jstring>& subscriber_credential,
    const base::android::JavaParamRef<jstring>& api_auth_token) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->InvalidateCredentials(
        base::BindOnce(&HnsVpnNativeWorker::OnInvalidateCredentials,
                       weak_factory_.GetWeakPtr()),
        base::android::ConvertJavaStringToUTF8(env, hostname),
        base::android::ConvertJavaStringToUTF8(env, client_id),
        base::android::ConvertJavaStringToUTF8(env, subscriber_credential),
        base::android::ConvertJavaStringToUTF8(env, api_auth_token));
  }
}

void HnsVpnNativeWorker::OnInvalidateCredentials(
    const std::string& invalidate_credentials_json,
    bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsVpnNativeWorker_onInvalidateCredentials(
      env, weak_java_hns_vpn_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, invalidate_credentials_json),
      success);
}

void HnsVpnNativeWorker::GetSubscriberCredential(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& product_type,
    const base::android::JavaParamRef<jstring>& product_id,
    const base::android::JavaParamRef<jstring>& validation_method,
    const base::android::JavaParamRef<jstring>& purchase_token,
    const base::android::JavaParamRef<jstring>& bundle_id) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->GetSubscriberCredential(
        base::BindOnce(&HnsVpnNativeWorker::OnGetSubscriberCredential,
                       weak_factory_.GetWeakPtr()),
        base::android::ConvertJavaStringToUTF8(env, product_type),
        base::android::ConvertJavaStringToUTF8(env, product_id),
        base::android::ConvertJavaStringToUTF8(env, validation_method),
        base::android::ConvertJavaStringToUTF8(env, purchase_token),
        base::android::ConvertJavaStringToUTF8(env, bundle_id));
  }
}

void HnsVpnNativeWorker::GetSubscriberCredentialV12(JNIEnv* env) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->GetSubscriberCredentialV12(
        base::BindOnce(&HnsVpnNativeWorker::OnGetSubscriberCredential,
                       weak_factory_.GetWeakPtr()));
  }
}

void HnsVpnNativeWorker::OnGetSubscriberCredential(
    const std::string& subscriber_credential,
    bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsVpnNativeWorker_onGetSubscriberCredential(
      env, weak_java_hns_vpn_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, subscriber_credential),
      success);
}

void HnsVpnNativeWorker::VerifyPurchaseToken(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& purchase_token,
    const base::android::JavaParamRef<jstring>& product_id,
    const base::android::JavaParamRef<jstring>& product_type,
    const base::android::JavaParamRef<jstring>& bundle_id) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->VerifyPurchaseToken(
        base::BindOnce(&HnsVpnNativeWorker::OnVerifyPurchaseToken,
                       weak_factory_.GetWeakPtr()),
        base::android::ConvertJavaStringToUTF8(env, purchase_token),
        base::android::ConvertJavaStringToUTF8(env, product_id),
        base::android::ConvertJavaStringToUTF8(env, product_type),
        base::android::ConvertJavaStringToUTF8(env, bundle_id));
  }
}

void HnsVpnNativeWorker::OnVerifyPurchaseToken(
    const std::string& json_response,
    bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsVpnNativeWorker_onVerifyPurchaseToken(
      env, weak_java_hns_vpn_native_worker_.get(env),
      base::android::ConvertUTF8ToJavaString(env, json_response), success);
}

jboolean HnsVpnNativeWorker::IsPurchasedUser(JNIEnv* env) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    return hns_vpn_service->is_purchased_user();
  }

  return false;
}

void HnsVpnNativeWorker::ReloadPurchasedState(JNIEnv* env) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->ReloadPurchasedState();
  }
}

void HnsVpnNativeWorker::ReportForegroundP3A(JNIEnv* env) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    // Reporting a new session to P3A functions.
    hns_vpn_service->RecordP3A(true);
  }
}

void HnsVpnNativeWorker::ReportBackgroundP3A(JNIEnv* env,
                                               jlong session_start_time_ms,
                                               jlong session_end_time_ms) {
  HnsVpnService* hns_vpn_service = GetHnsVpnService();
  if (hns_vpn_service) {
    hns_vpn_service->RecordAndroidBackgroundP3A(session_start_time_ms,
                                                  session_end_time_ms);
  }
}

static void JNI_HnsVpnNativeWorker_Init(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  new HnsVpnNativeWorker(env, jcaller);
}

}  // namespace android
}  // namespace chrome
