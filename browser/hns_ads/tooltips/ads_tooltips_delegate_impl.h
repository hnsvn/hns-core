/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_TOOLTIPS_DELEGATE_IMPL_H_
#define HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_TOOLTIPS_DELEGATE_IMPL_H_

#include <string>

#include "hns/browser/hns_ads/tooltips/ads_tooltips_controller.h"
#include "hns/components/hns_ads/browser/ads_tooltips_delegate.h"

class Profile;

namespace hns_ads {

class AdsTooltipsDelegateImpl : public AdsTooltipsDelegate {
 public:
  explicit AdsTooltipsDelegateImpl(Profile* profile);

  AdsTooltipsDelegateImpl(const AdsTooltipsDelegateImpl&) = delete;
  AdsTooltipsDelegateImpl& operator=(const AdsTooltipsDelegateImpl&) = delete;

  AdsTooltipsDelegateImpl(AdsTooltipsDelegateImpl&&) noexcept = delete;
  AdsTooltipsDelegateImpl& operator=(AdsTooltipsDelegateImpl&&) noexcept =
      delete;

  ~AdsTooltipsDelegateImpl() override = default;

  void ShowCaptchaTooltip(
      const std::string& payment_id,
      const std::string& captcha_id,
      bool include_cancel_button,
      ShowScheduledCaptchaCallback show_captcha_callback,
      SnoozeScheduledCaptchaCallback snooze_captcha_callback) override;
  void CloseCaptchaTooltip() override;

 private:
  AdsTooltipsController ads_tooltips_controller_;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_TOOLTIPS_DELEGATE_IMPL_H_
