// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/components/hns_adaptive_captcha/pref_names.h"

namespace hns_adaptive_captcha {
namespace prefs {
const char kScheduledCaptchaId[] = "hns.rewards.scheduled_captcha.id";
const char kScheduledCaptchaPaymentId[] =
    "hns.rewards.scheduled_captcha.payment_id";
const char kScheduledCaptchaSnoozeCount[] =
    "hns.rewards.scheduled_captcha.snooze_count";
const char kScheduledCaptchaFailedAttempts[] =
    "hns.rewards.scheduled_captcha.failed_attempts";
const char kScheduledCaptchaPaused[] = "hns.rewards.scheduled_captcha.paused";

}  // namespace prefs
}  // namespace hns_adaptive_captcha
