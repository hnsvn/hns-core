/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/database/database_column_util.h"

#include "base/check_op.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

namespace hns_ads::database {

int ColumnInt(mojom::DBRecordInfo* record, const size_t index) {
  CHECK(record);
  CHECK_LT(index, record->fields.size());
  CHECK_EQ(mojom::DBValue::Tag::kIntValue, record->fields.at(index)->which());

  return record->fields.at(index)->get_int_value();
}

int64_t ColumnInt64(mojom::DBRecordInfo* record, const size_t index) {
  CHECK(record);
  CHECK_LT(index, record->fields.size());
  CHECK_EQ(mojom::DBValue::Tag::kInt64Value, record->fields.at(index)->which());

  return record->fields.at(index)->get_int64_value();
}

double ColumnDouble(mojom::DBRecordInfo* record, const size_t index) {
  CHECK(record);
  CHECK_LT(index, record->fields.size());
  CHECK_EQ(mojom::DBValue::Tag::kDoubleValue,
           record->fields.at(index)->which());

  return record->fields.at(index)->get_double_value();
}

bool ColumnBool(mojom::DBRecordInfo* record, const size_t index) {
  CHECK(record);
  CHECK_LT(index, record->fields.size());
  CHECK_EQ(mojom::DBValue::Tag::kBoolValue, record->fields.at(index)->which());

  return record->fields.at(index)->get_bool_value();
}

std::string ColumnString(mojom::DBRecordInfo* record, const size_t index) {
  CHECK(record);
  CHECK_LT(index, record->fields.size());
  CHECK_EQ(mojom::DBValue::Tag::kStringValue,
           record->fields.at(index)->which());

  return record->fields.at(index)->get_string_value();
}

}  // namespace hns_ads::database
