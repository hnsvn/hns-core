/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_ads/notification_helper/notification_helper_impl_android.h"

#include "base/android/jni_string.h"
#include "base/system/sys_info.h"
#include "hns/browser/hns_ads/android/jni_headers/HnsAdsSignupDialog_jni.h"
#include "hns/browser/hns_ads/android/jni_headers/HnsAds_jni.h"
#include "hns/browser/hns_ads/background_helper/background_helper.h"
#include "hns/build/android/jni_headers/HnsNotificationSettingsBridge_jni.h"
#include "hns/components/hns_ads/browser/feature/custom_notification_ad_feature.h"
#include "chrome/browser/notifications/jni_headers/NotificationSystemStatusUtil_jni.h"
#include "chrome/browser/notifications/notification_channels_provider_android.h"

namespace hns_ads {

namespace {

constexpr int kMinimumMajorOperatingSystemVersionForNotificationChannels = 8;

constexpr int kAppNotificationStatusUndeterminable = 0;
constexpr int kAppNotificationsStatusEnabled = 2;

int GetOperatingSystemMajorVersion() {
  int32_t major_version = 0;
  int32_t minor_version = 0;
  int32_t bugfix_version = 0;

  base::SysInfo::OperatingSystemVersionNumbers(&major_version, &minor_version,
                                               &bugfix_version);

  return major_version;
}

bool IsHnsAdsNotificationChannelEnabled(const bool is_foreground) {
  if (GetOperatingSystemMajorVersion() <
      kMinimumMajorOperatingSystemVersionForNotificationChannels) {
    return true;
  }

  JNIEnv* env = base::android::AttachCurrentThread();

  const auto j_channel_id =
      (is_foreground) ? Java_HnsAds_getHnsAdsChannelId(env)
                      : Java_HnsAds_getHnsAdsBackgroundChannelId(env);

  const auto status = static_cast<NotificationChannelStatus>(
      Java_HnsNotificationSettingsBridge_getChannelStatus(env, j_channel_id));

  return (status == NotificationChannelStatus::ENABLED ||
          status == NotificationChannelStatus::UNAVAILABLE);
}

}  // namespace

NotificationHelperImplAndroid::NotificationHelperImplAndroid() = default;

NotificationHelperImplAndroid::~NotificationHelperImplAndroid() = default;

bool NotificationHelperImplAndroid::CanShowNotifications() {
  JNIEnv* env = base::android::AttachCurrentThread();
  const int status =
      Java_NotificationSystemStatusUtil_getAppNotificationStatus(env);

  const bool is_notifications_enabled =
      (status == kAppNotificationsStatusEnabled ||
       status == kAppNotificationStatusUndeterminable);

  const bool is_foreground = BackgroundHelper::GetInstance()->IsForeground();

  const bool is_notification_channel_enabled =
      IsHnsAdsNotificationChannelEnabled(is_foreground);

  bool can_show_native_notifications =
      is_notifications_enabled && is_notification_channel_enabled;

  if (!is_foreground) {
    can_show_native_notifications =
        can_show_native_notifications &&
        CanShowSystemNotificationsWhileBrowserIsBackgrounded();
  }

  return can_show_native_notifications;
}

bool NotificationHelperImplAndroid::
    CanShowSystemNotificationsWhileBrowserIsBackgrounded() const {
  JNIEnv* env = base::android::AttachCurrentThread();
  return Java_HnsAdsSignupDialog_showAdsInBackground(env);
}

bool NotificationHelperImplAndroid::ShowOnboardingNotification() {
  const bool should_show_custom_notifications =
      IsCustomNotificationAdFeatureEnabled();

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsAdsSignupDialog_enqueueOnboardingNotificationNative(
      env, should_show_custom_notifications);

  return true;
}

}  // namespace hns_ads
