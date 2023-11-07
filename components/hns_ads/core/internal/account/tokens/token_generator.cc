/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/tokens/token_generator.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token.h"

namespace hns_ads {

std::vector<cbr::Token> TokenGenerator::Generate(const size_t count) const {
  return std::vector<cbr::Token>(count);
}

}  // namespace hns_ads
