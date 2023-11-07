/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_APP_HNS_MAIN_DELEGATE_H_
#define HNS_IOS_APP_HNS_MAIN_DELEGATE_H_

#include <string>

#include "ios/chrome/app/startup/ios_chrome_main_delegate.h"

class HnsWebClient;

class HnsMainDelegate : public IOSChromeMainDelegate {
 public:
  HnsMainDelegate();
  HnsMainDelegate(const HnsMainDelegate&) = delete;
  HnsMainDelegate& operator=(const HnsMainDelegate&) = delete;
  ~HnsMainDelegate() override;

 protected:
  // web::WebMainDelegate implementation:
  void BasicStartupComplete() override;

 private:
};

#endif  // HNS_IOS_APP_HNS_MAIN_DELEGATE_H_
