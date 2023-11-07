// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_ADAPTIVE_CAPTCHA_PREF_NAMES_H_
#define HNS_COMPONENTS_HNS_ADAPTIVE_CAPTCHA_PREF_NAMES_H_

namespace hns_adaptive_captcha {
namespace prefs {

extern const char kScheduledCaptchaId[];
extern const char kScheduledCaptchaPaymentId[];
extern const char kScheduledCaptchaSnoozeCount[];
extern const char kScheduledCaptchaFailedAttempts[];
extern const char kScheduledCaptchaPaused[];

}  // namespace prefs
}  // namespace hns_adaptive_captcha

#endif  // HNS_COMPONENTS_HNS_ADAPTIVE_CAPTCHA_PREF_NAMES_H_
