/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/database/database_table_interface.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/legacy_migration/database/database_constants.h"

namespace hns_ads::database {

void TableInterface::Migrate(mojom::DBTransactionInfo* transaction,
                             const int to_version) {
  CHECK(transaction);

  if (to_version == database::kVersion) {
    Create(transaction);
  }
}

}  // namespace hns_ads::database
