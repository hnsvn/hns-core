/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/issuers/public_key_util.h"

#include "base/containers/contains.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuer_info.h"

namespace hns_ads {

bool PublicKeyExists(const IssuerInfo& issuer, const std::string& public_key) {
  return base::Contains(issuer.public_keys, public_key);
}

}  // namespace hns_ads
