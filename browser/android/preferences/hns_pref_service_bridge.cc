/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/build/android/jni_headers/HnsPrefServiceBridge_jni.h"

#include "base/android/jni_string.h"
#include "hns/components/hns_adaptive_captcha/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_perf_predictor/common/pref_names.h"
#include "hns/components/hns_referrals/common/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "hns/components/hns_shields/common/pref_names.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/de_amp/common/pref_names.h"
#include "hns/components/decentralized_dns/core/pref_names.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/cookie_settings_factory.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/pref_names.h"
#include "components/content_settings/core/browser/cookie_settings.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/prefs/pref_service.h"
#include "third_party/blink/public/common/peerconnection/webrtc_ip_handling_policy.h"
#include "url/gurl.h"

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/ipfs_constants.h"
#include "hns/components/ipfs/pref_names.h"
#endif

using base::android::ConvertUTF8ToJavaString;
using base::android::JavaParamRef;
using base::android::ScopedJavaLocalRef;
using hns_shields::ControlType;

namespace {

Profile* GetOriginalProfile() {
  return ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
}

enum WebRTCIPHandlingPolicy {
  DEFAULT,
  DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES,
  DEFAULT_PUBLIC_INTERFACE_ONLY,
  DISABLE_NON_PROXIED_UDP,
};

WebRTCIPHandlingPolicy GetWebRTCIPHandlingPolicy(
    const std::string& preference) {
  if (preference == blink::kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces)
    return DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES;
  if (preference == blink::kWebRTCIPHandlingDefaultPublicInterfaceOnly)
    return DEFAULT_PUBLIC_INTERFACE_ONLY;
  if (preference == blink::kWebRTCIPHandlingDisableNonProxiedUdp)
    return DISABLE_NON_PROXIED_UDP;
  return DEFAULT;
}

std::string GetWebRTCIPHandlingPreference(WebRTCIPHandlingPolicy policy) {
  if (policy == DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES)
    return blink::kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces;
  if (policy == DEFAULT_PUBLIC_INTERFACE_ONLY)
    return blink::kWebRTCIPHandlingDefaultPublicInterfaceOnly;
  if (policy == DISABLE_NON_PROXIED_UDP)
    return blink::kWebRTCIPHandlingDisableNonProxiedUdp;
  return blink::kWebRTCIPHandlingDefault;
}

}  // namespace

namespace chrome {
namespace android {

void JNI_HnsPrefServiceBridge_SetCookiesBlockType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type) {
  hns_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      GetOriginalProfile()->GetPrefs(),
      hns_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(), g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
JNI_HnsPrefServiceBridge_GetCookiesBlockType(JNIEnv* env) {
  hns_shields::ControlType control_type = hns_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      CookieSettingsFactory::GetForProfile(GetOriginalProfile()).get(), GURL());
  return base::android::ConvertUTF8ToJavaString(
      env, hns_shields::ControlTypeToString(control_type));
}

void JNI_HnsPrefServiceBridge_SetPlayYTVideoInBrowserEnabled(
    JNIEnv* env,
    jboolean enabled) {
  return GetOriginalProfile()->GetPrefs()->SetBoolean(
      kPlayYTVideoInBrowserEnabled, enabled);
}

jboolean JNI_HnsPrefServiceBridge_GetPlayYTVideoInBrowserEnabled(
    JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      kPlayYTVideoInBrowserEnabled);
}

void JNI_HnsPrefServiceBridge_SetBackgroundVideoPlaybackEnabled(
    JNIEnv* env,
    jboolean enabled) {
  return GetOriginalProfile()->GetPrefs()->SetBoolean(
      kBackgroundVideoPlaybackEnabled, enabled);
}

jboolean JNI_HnsPrefServiceBridge_GetBackgroundVideoPlaybackEnabled(
    JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      kBackgroundVideoPlaybackEnabled);
}

void JNI_HnsPrefServiceBridge_SetDesktopModeEnabled(JNIEnv* env,
                                                      jboolean enabled) {
  return GetOriginalProfile()->GetPrefs()->SetBoolean(kDesktopModeEnabled,
                                                      enabled);
}

jboolean JNI_HnsPrefServiceBridge_GetDesktopModeEnabled(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(kDesktopModeEnabled);
}

jlong JNI_HnsPrefServiceBridge_GetTrackersBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  return profile->GetPrefs()->GetUint64(kTrackersBlocked);
}

jlong JNI_HnsPrefServiceBridge_GetAdsBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  return profile->GetPrefs()->GetUint64(kAdsBlocked);
}

jlong JNI_HnsPrefServiceBridge_GetDataSaved(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  return profile->GetPrefs()->GetUint64(
      hns_perf_predictor::prefs::kBandwidthSavedBytes);
}

void JNI_HnsPrefServiceBridge_SetOldTrackersBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  profile->GetPrefs()->SetUint64(
      kTrackersBlocked,
      count + profile->GetPrefs()->GetUint64(kTrackersBlocked));
}

void JNI_HnsPrefServiceBridge_SetOldAdsBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  profile->GetPrefs()->SetUint64(
      kAdsBlocked, count + profile->GetPrefs()->GetUint64(kAdsBlocked));
}

void JNI_HnsPrefServiceBridge_SetOldHttpsUpgradesCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  profile->GetPrefs()->SetUint64(
      kHttpsUpgrades, count + profile->GetPrefs()->GetUint64(kHttpsUpgrades));
}

void JNI_HnsPrefServiceBridge_SetSafetynetCheckFailed(JNIEnv* env,
                                                        jboolean value) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(kSafetynetCheckFailed, value);
}

jboolean JNI_HnsPrefServiceBridge_GetSafetynetCheckFailed(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(kSafetynetCheckFailed);
}

void JNI_HnsPrefServiceBridge_ResetPromotionLastFetchStamp(JNIEnv* env) {
  GetOriginalProfile()->GetPrefs()->SetUint64(
      hns_rewards::prefs::kPromotionLastFetchStamp, 0);
}

jboolean JNI_HnsPrefServiceBridge_GetBooleanForContentSetting(JNIEnv* env,
                                                                jint type) {
  HostContentSettingsMap* content_settings =
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile());
  switch (content_settings->GetDefaultContentSetting((ContentSettingsType)type,
                                                     nullptr)) {
    case CONTENT_SETTING_ALLOW:
      return true;
    case CONTENT_SETTING_BLOCK:
    default:
      return false;
  }
}

jint JNI_HnsPrefServiceBridge_GetWebrtcPolicy(JNIEnv* env) {
  return static_cast<int>(
      GetWebRTCIPHandlingPolicy(GetOriginalProfile()->GetPrefs()->GetString(
          prefs::kWebRTCIPHandlingPolicy)));
}

void JNI_HnsPrefServiceBridge_SetWebrtcPolicy(JNIEnv* env, jint policy) {
  GetOriginalProfile()->GetPrefs()->SetString(
      prefs::kWebRTCIPHandlingPolicy,
      GetWebRTCIPHandlingPreference((WebRTCIPHandlingPolicy)policy));
}

void JNI_HnsPrefServiceBridge_SetNewsOptIn(JNIEnv* env, jboolean value) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      hns_news::prefs::kHnsNewsOptedIn, value);
}

jboolean JNI_HnsPrefServiceBridge_GetNewsOptIn(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      hns_news::prefs::kHnsNewsOptedIn);
}

void JNI_HnsPrefServiceBridge_SetShowNews(JNIEnv* env, jboolean value) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      hns_news::prefs::kNewTabPageShowToday, value);
}

jboolean JNI_HnsPrefServiceBridge_GetShowNews(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      hns_news::prefs::kNewTabPageShowToday);
}

}  // namespace android
}  // namespace chrome
