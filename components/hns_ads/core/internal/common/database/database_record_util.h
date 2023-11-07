/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_RECORD_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_RECORD_UTIL_H_

#include <vector>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "sql/statement.h"

namespace hns_ads::database {

mojom::DBRecordInfoPtr CreateRecord(
    sql::Statement* statement,
    const std::vector<mojom::DBCommandInfo::RecordBindingType>& bindings);

}  // namespace hns_ads::database

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_RECORD_UTIL_H_
