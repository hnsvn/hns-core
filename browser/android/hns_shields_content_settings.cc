/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/android/hns_shields_content_settings.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"

#include <string>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "hns/build/android/jni_headers/HnsShieldsContentSettings_jni.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/cookie_settings_factory.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/content_settings/core/browser/content_settings_utils.h"
#include "components/content_settings/core/browser/cookie_settings.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "url/gurl.h"

namespace chrome {
namespace android {


// That class is linked to a global toolbar. It's a one instance on Android
HnsShieldsContentSettings* g_hns_shields_content_settings = nullptr;

static void JNI_HnsShieldsContentSettings_Init(JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  g_hns_shields_content_settings =
      new HnsShieldsContentSettings(env, jcaller);
}

HnsShieldsContentSettings::HnsShieldsContentSettings(
    JNIEnv* env,
    const base::android::JavaRef<jobject>& obj)
    : jobj_(base::android::ScopedJavaGlobalRef<jobject>(obj)) {
  Java_HnsShieldsContentSettings_setNativePtr(env, obj,
      reinterpret_cast<intptr_t>(this));
}

HnsShieldsContentSettings::~HnsShieldsContentSettings() {
}

void HnsShieldsContentSettings::Destroy(JNIEnv* env) {
  g_hns_shields_content_settings = nullptr;
  delete this;
}

void HnsShieldsContentSettings::DispatchBlockedEventToJava(int tab_id,
        const std::string& block_type, const std::string& subresource) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsShieldsContentSettings_blockedEvent(
      env, jobj_, tab_id,
      base::android::ConvertUTF8ToJavaString(env, block_type),
      base::android::ConvertUTF8ToJavaString(env, subresource));
}

void HnsShieldsContentSettings::DispatchSavedBandwidthToJava(
  uint64_t savings) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsShieldsContentSettings_savedBandwidth(env, jobj_, savings);
}

void HnsShieldsContentSettings::DispatchSavedBandwidth(uint64_t savings) {
  DCHECK(g_hns_shields_content_settings);
  if (!g_hns_shields_content_settings) {
    return;
  }
  g_hns_shields_content_settings->DispatchSavedBandwidthToJava(savings);
}

// static
void HnsShieldsContentSettings::DispatchBlockedEvent(int tab_id,
  const std::string& block_type, const std::string& subresource) {
  DCHECK(g_hns_shields_content_settings);
  if (!g_hns_shields_content_settings) {
    return;
  }
  g_hns_shields_content_settings->DispatchBlockedEventToJava(tab_id,
      block_type, subresource);
}

void JNI_HnsShieldsContentSettings_SetHnsShieldsEnabled(JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetHnsShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      enabled,
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_HnsShieldsContentSettings_GetHnsShieldsEnabled(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return hns_shields::GetHnsShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

void JNI_HnsShieldsContentSettings_SetAdControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      hns_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_HnsShieldsContentSettings_GetAdControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::ControlType control_type =
      hns_shields::GetAdControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      hns_shields::ControlTypeToString(control_type));
}

void JNI_HnsShieldsContentSettings_SetCookieControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      ProfileAndroid::FromProfileAndroid(j_profile)->GetPrefs(),
      hns_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

void JNI_HnsShieldsContentSettings_SetCosmeticFilteringControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetCosmeticFilteringControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      hns_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state(),
      ProfileAndroid::FromProfileAndroid(j_profile)->GetPrefs());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_HnsShieldsContentSettings_GetCookieControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::ControlType control_type = hns_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      CookieSettingsFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile))
          .get(),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      hns_shields::ControlTypeToString(control_type));
}

void JNI_HnsShieldsContentSettings_SetFingerprintingControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      hns_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state(),
      ProfileAndroid::FromProfileAndroid(j_profile)->GetPrefs());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_HnsShieldsContentSettings_GetFingerprintingControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::ControlType control_type =
      hns_shields::GetFingerprintingControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(
      env, hns_shields::ControlTypeToString(control_type));
}

void JNI_HnsShieldsContentSettings_SetHttpsUpgradeControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetHttpsUpgradeControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      hns_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
JNI_HnsShieldsContentSettings_GetHttpsUpgradeControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::ControlType control_type =
      hns_shields::GetHttpsUpgradeControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      hns_shields::ControlTypeToString(control_type));
}

base::android::ScopedJavaLocalRef<jstring>
JNI_HnsShieldsContentSettings_GetCosmeticFilteringControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::ControlType cosmetic_type =
      hns_shields::GetCosmeticFilteringControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(
      env, hns_shields::ControlTypeToString(cosmetic_type));
}

void JNI_HnsShieldsContentSettings_SetHTTPSEverywhereEnabled(JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      enabled,
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_HnsShieldsContentSettings_GetHTTPSEverywhereEnabled(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return hns_shields::GetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

void JNI_HnsShieldsContentSettings_SetNoScriptControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      hns_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_HnsShieldsContentSettings_GetNoScriptControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::ControlType control_type =
      hns_shields::GetNoScriptControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      hns_shields::ControlTypeToString(control_type));
}

void JNI_HnsShieldsContentSettings_SetForgetFirstPartyStorageEnabled(
    JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  hns_shields::SetForgetFirstPartyStorageEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      enabled, GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_HnsShieldsContentSettings_GetForgetFirstPartyStorageEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return hns_shields::GetForgetFirstPartyStorageEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

}  // namespace android
}  // namespace chrome
