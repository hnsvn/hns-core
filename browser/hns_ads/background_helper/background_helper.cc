/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_ads/background_helper/background_helper.h"

#include "hns/browser/hns_ads/background_helper/background_helper_holder.h"
#include "build/build_config.h"

namespace hns_ads {

BackgroundHelper::BackgroundHelper() = default;

BackgroundHelper::~BackgroundHelper() = default;

bool BackgroundHelper::IsForeground() const {
  return true;
}

void BackgroundHelper::AddObserver(Observer* observer) {
  observers_.AddObserver(observer);
}

void BackgroundHelper::RemoveObserver(Observer* observer) {
  observers_.RemoveObserver(observer);
}

void BackgroundHelper::TriggerOnForeground() {
  for (auto& observer : observers_) {
    observer.OnBrowserDidEnterForeground();
  }
}

void BackgroundHelper::TriggerOnBackground() {
  for (auto& observer : observers_) {
    observer.OnBrowserDidEnterBackground();
  }
}

// static
BackgroundHelper* BackgroundHelper::GetInstance() {
  BackgroundHelperHolder* holder = BackgroundHelperHolder::GetInstance();
  return holder->GetBackgroundHelper();
}

}  // namespace hns_ads
