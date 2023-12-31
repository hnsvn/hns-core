/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_PIPELINE_PIPELINE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_PIPELINE_PIPELINE_UTIL_H_

#include "base/values.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::ml::pipeline {

struct PipelineInfo;

absl::optional<PipelineInfo> ParsePipelineValue(base::Value::Dict dict);

}  // namespace hns_ads::ml::pipeline

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_PIPELINE_PIPELINE_UTIL_H_
