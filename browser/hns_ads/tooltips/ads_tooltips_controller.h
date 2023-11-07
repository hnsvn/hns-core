/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_TOOLTIPS_CONTROLLER_H_
#define HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_TOOLTIPS_CONTROLLER_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "hns/browser/ui/hns_tooltips/hns_tooltip_delegate.h"
#include "hns/components/hns_ads/browser/ads_tooltips_delegate.h"
#include "chrome/browser/profiles/profile.h"

class Profile;

namespace hns_ads {

class AdsTooltipsController : public AdsTooltipsDelegate,
                              public hns_tooltips::HnsTooltipDelegate {
 public:
  explicit AdsTooltipsController(Profile* profile);

  AdsTooltipsController(const AdsTooltipsController&) = delete;
  AdsTooltipsController& operator=(const AdsTooltipsController&) = delete;

  AdsTooltipsController(AdsTooltipsController&&) noexcept = delete;
  AdsTooltipsController& operator=(AdsTooltipsController&&) noexcept = delete;

  ~AdsTooltipsController() override;

  // AdsTooltipDelegate:
  void ShowCaptchaTooltip(
      const std::string& payment_id,
      const std::string& captcha_id,
      bool include_cancel_button,
      ShowScheduledCaptchaCallback show_captcha_callback,
      SnoozeScheduledCaptchaCallback snooze_captcha_callback) override;
  void CloseCaptchaTooltip() override;

 private:
  // hns_tooltips::HnsTooltipDelegate:
  void OnTooltipWidgetDestroyed(const std::string& tooltip_id) override;

  raw_ptr<Profile> profile_ = nullptr;  // NOT OWNED
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_TOOLTIPS_ADS_TOOLTIPS_CONTROLLER_H_
