/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/hns_ads/padded_image_view.h"

#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/gfx/color_palette.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/views/background.h"
#include "ui/views/border.h"

namespace hns_ads {

namespace {

constexpr SkColor kBackgroundColor = SK_ColorTRANSPARENT;

constexpr auto kBorderInset = gfx::Insets::TLBR(2, 6, 0, 6);

}  // namespace

PaddedImageView::PaddedImageView() {
  SetBackground(views::CreateSolidBackground(kBackgroundColor));

  SetBorder(views::CreateEmptyBorder(kBorderInset));
}

BEGIN_METADATA(PaddedImageView, views::ImageView)
END_METADATA

}  // namespace hns_ads
