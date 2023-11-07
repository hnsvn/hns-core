/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/campaigns_database_util.h"

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/creatives/campaigns_database_table.h"

namespace hns_ads::database {

void DeleteCampaigns() {
  const table::Campaigns database_table;
  database_table.Delete(base::BindOnce([](const bool success) {
    if (!success) {
      return BLOG(0, "Failed to delete campaigns");
    }
  }));
}

}  // namespace hns_ads::database
