/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_builder.h"

#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_id_pattern/verifiable_conversion_id_pattern_util.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_info.h"
#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_info.h"
#include "url/gurl.h"

namespace hns_ads {

absl::optional<VerifiableConversionInfo> MaybeBuildVerifiableConversion(
    const std::vector<GURL>& redirect_chain,
    const std::string& html,
    const ConversionResourceIdPatternMap& resource_id_patterns,
    const CreativeSetConversionInfo& creative_set_conversion) {
  if (!creative_set_conversion.verifiable_advertiser_public_key_base64) {
    return absl::nullopt;
  }

  const absl::optional<std::string> verifiable_conversion_id =
      MaybeParseVerifiableConversionId(redirect_chain, html,
                                       resource_id_patterns);
  if (!verifiable_conversion_id) {
    return absl::nullopt;
  }

  return VerifiableConversionInfo{
      *verifiable_conversion_id,
      *creative_set_conversion.verifiable_advertiser_public_key_base64};
}

}  // namespace hns_ads
