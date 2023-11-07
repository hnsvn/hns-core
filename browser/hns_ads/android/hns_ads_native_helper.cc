/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_ads/android/hns_ads_native_helper.h"

#include <string>

#include "base/android/jni_string.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/hns_ads/android/jni_headers/HnsAdsNativeHelper_jni.h"
#include "hns/components/hns_ads/core/public/ads_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "chrome/browser/profiles/profile_android.h"
#include "components/prefs/pref_service.h"

namespace hns_ads {

// static
jboolean JNI_HnsAdsNativeHelper_IsOptedInToNotificationAds(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  return profile->GetPrefs()->GetBoolean(
      hns_ads::prefs::kOptedInToNotificationAds);
}

// static
void JNI_HnsAdsNativeHelper_SetOptedInToNotificationAds(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    jboolean should_enable_ads) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  profile->GetPrefs()->SetBoolean(hns_ads::prefs::kOptedInToNotificationAds,
                                  should_enable_ads);
}

// static
jboolean JNI_HnsAdsNativeHelper_IsSupportedRegion(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return false;
  }

  return hns_ads::IsSupportedRegion();
}

// static
void JNI_HnsAdsNativeHelper_OnNotificationAdShown(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return;
  }

  const std::string notification_id =
      base::android::ConvertJavaStringToUTF8(env, j_notification_id);
  ads_service->OnNotificationAdShown(notification_id);
}

// static
void JNI_HnsAdsNativeHelper_OnNotificationAdClosed(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id,
    jboolean j_by_user) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return;
  }

  const std::string notification_id =
      base::android::ConvertJavaStringToUTF8(env, j_notification_id);
  ads_service->OnNotificationAdClosed(notification_id, j_by_user);
}

// static
void JNI_HnsAdsNativeHelper_OnNotificationAdClicked(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return;
  }

  const std::string notification_id =
      base::android::ConvertJavaStringToUTF8(env, j_notification_id);
  ads_service->OnNotificationAdClicked(notification_id);
}

}  // namespace hns_ads
