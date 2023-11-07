/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/types/default_conversion/creative_set_conversion_url_pattern/creative_set_conversion_url_pattern_util.h"

#include "hns/components/hns_ads/core/internal/common/url/url_util.h"
#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_info.h"
#include "url/gurl.h"

namespace hns_ads {

bool DoesCreativeSetConversionUrlPatternMatchRedirectChain(
    const CreativeSetConversionInfo& creative_set_conversion,
    const std::vector<GURL>& redirect_chain) {
  return MatchUrlPattern(redirect_chain, creative_set_conversion.url_pattern);
}

}  // namespace hns_ads
