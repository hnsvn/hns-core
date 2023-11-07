/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ADS_NOTIFICATION_AD_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ADS_NOTIFICATION_AD_VIEW_H_

#include "hns/browser/ui/hns_ads/notification_ad.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
class Point;
}  // namespace gfx

namespace hns_ads {

class NotificationAdView : public views::View {
 public:
  METADATA_HEADER(NotificationAdView);

  explicit NotificationAdView(const NotificationAd& notification_ad);

  NotificationAdView(const NotificationAdView&) = delete;
  NotificationAdView& operator=(const NotificationAdView&) = delete;

  NotificationAdView(NotificationAdView&&) noexcept = delete;
  NotificationAdView& operator=(NotificationAdView&&) noexcept = delete;

  ~NotificationAdView() override;

  // Update notification contents to |notification_ad|
  virtual void UpdateContents(const NotificationAd& notification_ad);

  void OnCloseButtonPressed();

  // views::View:
  void GetAccessibleNodeData(ui::AXNodeData* node_data) override;
  void OnDeviceScaleFactorChanged(float old_device_scale_factor,
                                  float new_device_scale_factor) override;
  void OnThemeChanged() override;

 private:
  NotificationAd notification_ad_;

  bool is_closing_ = false;

  void CreateView();

  std::u16string accessible_name_;
  void MaybeNotifyAccessibilityEvent();
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ADS_NOTIFICATION_AD_VIEW_H_
