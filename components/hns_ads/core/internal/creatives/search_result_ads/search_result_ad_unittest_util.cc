/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/search_result_ads/search_result_ad_unittest_util.h"

#include "base/check.h"
#include "base/uuid.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "url/gurl.h"

namespace hns_ads {

namespace {

constexpr char kHeadlineText[] = "headline";
constexpr char kDescription[] = "description";
constexpr int kValue = 1.0;

constexpr char kConversionUrlPattern[] = "https://hns.com/*";
constexpr base::TimeDelta kConversionObservationWindow = base::Days(3);

}  // namespace

mojom::SearchResultAdInfoPtr BuildSearchResultAdForTesting(
    const bool should_use_random_uuids) {
  mojom::SearchResultAdInfoPtr ad = mojom::SearchResultAdInfo::New();

  ad->placement_id = should_use_random_uuids
                         ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                         : kPlacementId;

  ad->creative_instance_id =
      should_use_random_uuids
          ? base::Uuid::GenerateRandomV4().AsLowercaseString()
          : kCreativeInstanceId;

  ad->creative_set_id = should_use_random_uuids
                            ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                            : kCreativeSetId;

  ad->campaign_id = should_use_random_uuids
                        ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                        : kCampaignId;

  ad->advertiser_id = should_use_random_uuids
                          ? base::Uuid::GenerateRandomV4().AsLowercaseString()
                          : kAdvertiserId;

  ad->target_url = GURL("https://hns.com");

  ad->headline_text = kHeadlineText;

  ad->description = kDescription;

  ad->value = kValue;

  return ad;
}

mojom::SearchResultAdInfoPtr BuildSearchResultAdWithConversionForTesting(
    const bool should_use_random_uuids) {
  mojom::SearchResultAdInfoPtr ad =
      BuildSearchResultAdForTesting(should_use_random_uuids);
  CHECK(ad);

  ad->conversion = mojom::ConversionInfo::New();
  ad->conversion->url_pattern = kConversionUrlPattern;
  ad->conversion->verifiable_advertiser_public_key_base64 =
      kVerifiableConversionAdvertiserPublicKey;
  ad->conversion->observation_window = kConversionObservationWindow;

  return ad;
}

}  // namespace hns_ads
