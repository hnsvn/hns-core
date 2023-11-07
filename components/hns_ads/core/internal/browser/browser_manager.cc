/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/browser/browser_manager.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"

namespace hns_ads {

BrowserManager::BrowserManager() {
  AdsClientHelper::AddObserver(this);
}

BrowserManager::~BrowserManager() {
  AdsClientHelper::RemoveObserver(this);
}

// static
BrowserManager& BrowserManager::GetInstance() {
  return GlobalState::GetInstance()->GetBrowserManager();
}

void BrowserManager::AddObserver(BrowserManagerObserver* observer) {
  CHECK(observer);
  observers_.AddObserver(observer);
}

void BrowserManager::RemoveObserver(BrowserManagerObserver* observer) {
  CHECK(observer);
  observers_.RemoveObserver(observer);
}

///////////////////////////////////////////////////////////////////////////////

void BrowserManager::NotifyBrowserDidBecomeActive() const {
  for (BrowserManagerObserver& observer : observers_) {
    observer.OnBrowserDidBecomeActive();
  }
}

void BrowserManager::NotifyBrowserDidResignActive() const {
  for (BrowserManagerObserver& observer : observers_) {
    observer.OnBrowserDidResignActive();
  }
}

void BrowserManager::LogBrowserActiveState() const {
  if (is_active_) {
    BLOG(1, "Browser did become active");
  } else {
    BLOG(1, "Browser did resign active");
  }
}

void BrowserManager::NotifyBrowserDidEnterForeground() const {
  for (BrowserManagerObserver& observer : observers_) {
    observer.OnBrowserDidEnterForeground();
  }
}

void BrowserManager::NotifyBrowserDidEnterBackground() const {
  for (BrowserManagerObserver& observer : observers_) {
    observer.OnBrowserDidEnterBackground();
  }
}

void BrowserManager::LogBrowserForegroundState() const {
  if (is_in_foreground_) {
    BLOG(1, "Browser did enter foreground");
  } else {
    BLOG(1, "Browser did enter background");
  }
}

void BrowserManager::OnNotifyDidInitializeAds() {
  const bool is_browser_active =
      AdsClientHelper::GetInstance()->IsBrowserActive();

  is_active_ = is_browser_active;
  LogBrowserActiveState();

  is_in_foreground_ = is_browser_active;
  LogBrowserForegroundState();
}

void BrowserManager::OnNotifyBrowserDidBecomeActive() {
  if (is_active_) {
    return;
  }

  is_active_ = true;
  LogBrowserActiveState();

  NotifyBrowserDidBecomeActive();
}

void BrowserManager::OnNotifyBrowserDidResignActive() {
  if (!is_active_) {
    return;
  }

  is_active_ = false;
  LogBrowserActiveState();

  NotifyBrowserDidResignActive();
}

void BrowserManager::OnNotifyBrowserDidEnterForeground() {
  if (is_in_foreground_) {
    return;
  }

  is_in_foreground_ = true;
  LogBrowserForegroundState();

  NotifyBrowserDidEnterForeground();
}

void BrowserManager::OnNotifyBrowserDidEnterBackground() {
  if (!is_in_foreground_) {
    return;
  }

  is_in_foreground_ = false;
  LogBrowserForegroundState();

  NotifyBrowserDidEnterBackground();
}

}  // namespace hns_ads
