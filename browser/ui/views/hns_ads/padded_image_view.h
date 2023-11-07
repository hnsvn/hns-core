/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ADS_PADDED_IMAGE_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ADS_PADDED_IMAGE_VIEW_H_

#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/controls/image_view.h"

namespace hns_ads {

class PaddedImageView : public views::ImageView {
 public:
  METADATA_HEADER(PaddedImageView);

  PaddedImageView();

  PaddedImageView(const PaddedImageView&) = delete;
  PaddedImageView& operator=(const PaddedImageView&) = delete;

  PaddedImageView(PaddedImageView&&) noexcept = delete;
  PaddedImageView& operator=(PaddedImageView&&) noexcept = delete;

  ~PaddedImageView() override = default;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ADS_PADDED_IMAGE_VIEW_H_
