/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_processor_util.h"

#include "base/strings/string_util.h"
#include "hns/components/hns_ads/core/internal/common/strings/string_strip_util.h"

namespace hns_ads {

std::string SanitizeText(const std::string& text) {
  return base::ToLowerASCII(StripNonAlphaCharacters(text));
}

}  // namespace hns_ads
