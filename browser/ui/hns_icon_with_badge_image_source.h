/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_ICON_WITH_BADGE_IMAGE_SOURCE_H_
#define HNS_BROWSER_UI_HNS_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#include "chrome/browser/ui/extensions/icon_with_badge_image_source.h"

namespace gfx {
class Canvas;
class Rect;
}  // namespace gfx

namespace hns {

extern const SkColor kBadgeNotificationBG;
extern const SkColor kBadgeTextColor;

// The purpose of this subclass is to:
// - Paint the HnsAction badge in a custom location and with a different size
//   to regular BrowserAction extensions.
class HnsIconWithBadgeImageSource : public IconWithBadgeImageSource {
 public:
  HnsIconWithBadgeImageSource(
      const gfx::Size& size,
      GetColorProviderCallback get_color_provider_callback,
      size_t content_image_size,
      size_t image_left_margin_extra);

  HnsIconWithBadgeImageSource(const HnsIconWithBadgeImageSource&) = delete;
  HnsIconWithBadgeImageSource& operator=(
      const HnsIconWithBadgeImageSource&) = delete;

  void SetAllowEmptyText(bool v);

  static gfx::Size GetMaxBadgeSize();

 protected:
  virtual void PaintBadgeWithoutText(const gfx::Rect& badge_rect,
                                     gfx::Canvas* canvas);

 private:
  void PaintBadge(gfx::Canvas* canvas) override;
  void PaintBadgeWithText(gfx::Canvas* canvas);

  gfx::Rect GetIconAreaRect() const override;
  gfx::Rect GetBadgeRect(size_t badge_width) const;

  absl::optional<int> GetCustomGraphicSize() override;
  absl::optional<int> GetCustomGraphicXOffset() override;
  absl::optional<int> GetCustomGraphicYOffset() override;

  bool allow_empty_text_ = false;
  size_t content_image_size_;
  size_t image_left_margin_extra_;
};

}  // namespace hns

#endif  // HNS_BROWSER_UI_HNS_ICON_WITH_BADGE_IMAGE_SOURCE_H_
