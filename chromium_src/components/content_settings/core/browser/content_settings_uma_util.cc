/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/content_settings/core/browser/content_settings_uma_util.h"

// Leave a gap between Chromium values and our values in the kHistogramValue
// array so that we don't have to renumber when new content settings types are
// added upstream.
namespace {

// Do not change the value arbitrarily. This is used to validate we have a gap
// between Chromium's and Hns's histograms. This value must be less than 1000
// as upstream performs a sanity check that the total number of buckets isn't
// unreasonably large.
constexpr int kHnsValuesStart = 900;

constexpr int hns_value(int incr) {
  return kHnsValuesStart + incr;
}

}  // namespace

static_assert(static_cast<int>(ContentSettingsType::NUM_TYPES) <
                  kHnsValuesStart,
              "There must a gap between the histograms used by Chromium, and "
              "the ones used by Hns.");

// clang-format off
#define HNS_HISTOGRAM_VALUE_LIST                                        \
  {ContentSettingsType::HNS_ADS, hns_value(0)},                       \
  {ContentSettingsType::HNS_COSMETIC_FILTERING, hns_value(1)},        \
  {ContentSettingsType::HNS_TRACKERS, hns_value(2)},                  \
  {ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES, hns_value(3)}, \
  {ContentSettingsType::HNS_FINGERPRINTING_V2, hns_value(4)},         \
  {ContentSettingsType::HNS_SHIELDS, hns_value(5)},                   \
  {ContentSettingsType::HNS_REFERRERS, hns_value(6)},                 \
  {ContentSettingsType::HNS_COOKIES, hns_value(7)},                   \
  {ContentSettingsType::HNS_SPEEDREADER, hns_value(8)},               \
  {ContentSettingsType::HNS_ETHEREUM, hns_value(9)},                  \
  {ContentSettingsType::HNS_SOLANA, hns_value(10)},                   \
  {ContentSettingsType::HNS_GOOGLE_SIGN_IN, hns_value(11)},           \
  {ContentSettingsType::HNS_HTTPS_UPGRADE, hns_value(12)},            \
  {ContentSettingsType::HNS_REMEMBER_1P_STORAGE, hns_value(13)},      \
  {ContentSettingsType::HNS_LOCALHOST_ACCESS, hns_value(14)}
// clang-format on

#include "src/components/content_settings/core/browser/content_settings_uma_util.cc"

#undef HNS_HISTOGRAM_VALUE_LIST
