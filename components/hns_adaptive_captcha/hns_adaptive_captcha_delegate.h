/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADAPTIVE_CAPTCHA_HNS_ADAPTIVE_CAPTCHA_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADAPTIVE_CAPTCHA_HNS_ADAPTIVE_CAPTCHA_DELEGATE_H_

#include <string>

namespace hns_adaptive_captcha {

class HnsAdaptiveCaptchaDelegate {
 public:
  virtual ~HnsAdaptiveCaptchaDelegate() = default;

  virtual bool ShowScheduledCaptcha(const std::string& payment_id,
                                    const std::string& captcha_id) = 0;
};

}  // namespace hns_adaptive_captcha

#endif  // HNS_COMPONENTS_HNS_ADAPTIVE_CAPTCHA_HNS_ADAPTIVE_CAPTCHA_DELEGATE_H_
