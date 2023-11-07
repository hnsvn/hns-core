/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_ACTIONS_CONVERSION_ACTION_TYPES_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_ACTIONS_CONVERSION_ACTION_TYPES_H_

namespace hns_ads {

enum class ConversionActionType {
  kUndefined = 0,

  // View-through conversions occur when a user is exposed to an ad impression
  // and later completes a desired action, such as making a purchase or filling
  // out a form. The conversion is attributed to the ad view rather than a
  // direct click.
  kViewThrough,

  // Click-through conversions occur when a user clicks on an ad and later
  // completes a desired action, such as making a purchase or filling out a
  // form. Click-through conversions should take priority over view-through.
  kClickThrough
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_ACTIONS_CONVERSION_ACTION_TYPES_H_