/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_TEST_WEB_PAGE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_TEST_WEB_PAGE_UTIL_H_

#include <vector>

#include "base/strings/string_piece_forward.h"
#include "components/schema_org/common/metadata.mojom-forward.h"

namespace hns_ads {

constexpr char kTestWebPagePlacementId[] = "placement-id";

std::vector<::schema_org::mojom::EntityPtr> CreateTestWebPageEntities(
    std::vector<base::StringPiece> attributes_to_skip);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_TEST_WEB_PAGE_UTIL_H_
