/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/embeddings_database_util.h"

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/creatives/embeddings_database_table.h"

namespace hns_ads::database {

void DeleteEmbeddings() {
  const table::Embeddings database_table;
  database_table.Delete(base::BindOnce([](const bool success) {
    if (!success) {
      return BLOG(0, "Failed to delete embeddings");
    }
  }));
}

}  // namespace hns_ads::database
