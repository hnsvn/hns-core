/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_SEARCH_ENGINE_SEARCH_ENGINE_DOMAIN_EXTENSION_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_SEARCH_ENGINE_SEARCH_ENGINE_DOMAIN_EXTENSION_CONSTANTS_H_

#include "base/containers/span.h"
#include "base/strings/string_piece.h"

namespace hns_ads {

base::span<const base::StringPiece> GetAmazonSearchEngineDomainExtensions();
base::span<const base::StringPiece> GetGoogleSearchEngineDomainExtensions();
base::span<const base::StringPiece> GetMojeekSearchEngineDomainExtensions();

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_SEARCH_ENGINE_SEARCH_ENGINE_DOMAIN_EXTENSION_CONSTANTS_H_
