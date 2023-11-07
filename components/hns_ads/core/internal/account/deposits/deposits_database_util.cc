/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/deposits/deposits_database_util.h"

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/account/deposits/deposits_database_table.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"

namespace hns_ads::database {

void PurgeExpiredDeposits() {
  const database::table::Deposits database_table;
  database_table.PurgeExpired(base::BindOnce([](const bool success) {
    if (!success) {
      return BLOG(0, "Failed to purge expired deposits");
    }

    BLOG(3, "Successfully purged expired deposits");
  }));
}

}  // namespace hns_ads::database
