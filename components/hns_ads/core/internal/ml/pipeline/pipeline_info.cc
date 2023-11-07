/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/pipeline/pipeline_info.h"

#include <utility>

namespace hns_ads::ml::pipeline {

PipelineInfo::PipelineInfo(const int version,
                           std::string timestamp,
                           std::string locale,
                           TransformationVector transformations,
                           LinearModel linear_model)
    : version(version),
      timestamp(std::move(timestamp)),
      locale(std::move(locale)),
      transformations(std::move(transformations)),
      linear_model(std::move(linear_model)) {}

PipelineInfo::PipelineInfo() = default;

PipelineInfo::PipelineInfo(PipelineInfo&& other) noexcept = default;

PipelineInfo& PipelineInfo::operator=(PipelineInfo&& other) noexcept = default;

PipelineInfo::~PipelineInfo() = default;

}  // namespace hns_ads::ml::pipeline
