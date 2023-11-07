/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_id_pattern/parsers/verifiable_conversion_id_html_meta_tag_parser_util.h"

#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/conversions/conversions_feature.h"
#include "third_party/re2/src/re2/re2.h"

namespace hns_ads {

absl::optional<std::string> MaybeParseVerifableConversionIdFromHtmlMetaTag(
    const std::string& html) {
  const std::string id_pattern = kHtmlMetaTagConversionIdPattern.Get();

  re2::StringPiece html_string_piece(html);
  const RE2 r(id_pattern);
  std::string verifiable_conversion_id;

  if (!RE2::FindAndConsume(&html_string_piece, r, &verifiable_conversion_id)) {
    BLOG(1, "Failed to parse verifiable conversion id for " << id_pattern
                                                            << " id pattern");
    return absl::nullopt;
  }

  return verifiable_conversion_id;
}

}  // namespace hns_ads
