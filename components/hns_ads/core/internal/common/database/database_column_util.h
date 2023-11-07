/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_COLUMN_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_COLUMN_UTIL_H_

#include <cstdint>
#include <string>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads::database {

[[nodiscard]] int ColumnInt(mojom::DBRecordInfo* record, size_t index);
[[nodiscard]] int64_t ColumnInt64(mojom::DBRecordInfo* record, size_t index);
[[nodiscard]] double ColumnDouble(mojom::DBRecordInfo* record, size_t index);
[[nodiscard]] bool ColumnBool(mojom::DBRecordInfo* record, size_t index);
[[nodiscard]] std::string ColumnString(mojom::DBRecordInfo* record,
                                       size_t index);

}  // namespace hns_ads::database

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_COLUMN_UTIL_H_
