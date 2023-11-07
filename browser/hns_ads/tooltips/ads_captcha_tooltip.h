/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_CAPTCHA_TOOLTIP_H_
#define HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_CAPTCHA_TOOLTIP_H_

#include <string>

#include "base/functional/callback.h"
#include "hns/browser/ui/hns_tooltips/hns_tooltip.h"
#include "hns/browser/ui/hns_tooltips/hns_tooltip_attributes.h"
#include "hns/components/hns_ads/browser/ads_tooltips_delegate.h"

namespace hns_ads {

extern const char kScheduledCaptchaTooltipId[];

class AdsCaptchaTooltip : public hns_tooltips::HnsTooltip {
 public:
  AdsCaptchaTooltip(ShowScheduledCaptchaCallback show_captcha_callback,
                    SnoozeScheduledCaptchaCallback snooze_captcha_callback,
                    const hns_tooltips::HnsTooltipAttributes& attributes,
                    const std::string& payment_id,
                    const std::string& captcha_id);

  AdsCaptchaTooltip(const AdsCaptchaTooltip&) = delete;
  AdsCaptchaTooltip& operator=(const AdsCaptchaTooltip&) = delete;

  AdsCaptchaTooltip(AdsCaptchaTooltip&&) noexcept = delete;
  AdsCaptchaTooltip& operator=(AdsCaptchaTooltip&&) noexcept = delete;

  ~AdsCaptchaTooltip() override;

  const std::string& payment_id() const { return payment_id_; }
  const std::string& captcha_id() const { return captcha_id_; }

  // hns_tooltips::HnsTooltip:
  void PerformOkButtonAction() override;
  void PerformCancelButtonAction() override;

 private:
  ShowScheduledCaptchaCallback show_captcha_callback_;
  SnoozeScheduledCaptchaCallback snooze_captcha_callback_;
  std::string payment_id_;
  std::string captcha_id_;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_CAPTCHA_TOOLTIP_H_
