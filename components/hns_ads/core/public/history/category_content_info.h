/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_HISTORY_CATEGORY_CONTENT_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_HISTORY_CATEGORY_CONTENT_INFO_H_

#include <string>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/export.h"

namespace hns_ads {

struct ADS_EXPORT CategoryContentInfo final {
  std::string category;
  mojom::UserReactionType user_reaction_type =
      mojom::UserReactionType::kNeutral;
};

bool operator==(const CategoryContentInfo&, const CategoryContentInfo&);
bool operator!=(const CategoryContentInfo&, const CategoryContentInfo&);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_HISTORY_CATEGORY_CONTENT_INFO_H_
