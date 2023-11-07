/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_ANDROID_H_
#define HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_ANDROID_H_

#include <memory>

#include "base/android/application_status_listener.h"
#include "base/memory/weak_ptr.h"
#include "hns/browser/hns_ads/background_helper/background_helper.h"

namespace hns_ads {

class BackgroundHelperAndroid
    : public BackgroundHelper,
      public base::SupportsWeakPtr<BackgroundHelperAndroid> {
 public:
  BackgroundHelperAndroid(const BackgroundHelperAndroid&) = delete;
  BackgroundHelperAndroid& operator=(const BackgroundHelperAndroid&) = delete;

  BackgroundHelperAndroid(BackgroundHelperAndroid&&) noexcept = delete;
  BackgroundHelperAndroid& operator=(BackgroundHelperAndroid&&) noexcept =
      delete;

  ~BackgroundHelperAndroid() override;

 protected:
  friend class BackgroundHelperHolder;

  BackgroundHelperAndroid();

 private:
  void OnApplicationStateChange(base::android::ApplicationState state);

  // BackgroundHelper impl
  bool IsForeground() const override;

  std::unique_ptr<base::android::ApplicationStatusListener>
      app_status_listener_;

  base::android::ApplicationState last_state_;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_ANDROID_H_
