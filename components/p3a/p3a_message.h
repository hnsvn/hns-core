/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_P3A_P3A_MESSAGE_H_
#define HNS_COMPONENTS_P3A_P3A_MESSAGE_H_

#include <cstdint>
#include <string>

#include "base/time/time.h"
#include "base/values.h"
#include "hns/components/p3a/metric_log_type.h"

class PrefService;

namespace p3a {

constexpr const char* kP3AMessageConstellationKeyValueSeparator = "|";
constexpr const char* kP3AMessageConstellationLayerSeparator = ";";

class MessageMetainfo {
 public:
  MessageMetainfo();
  ~MessageMetainfo();

  void Init(PrefService* local_state,
            std::string hns_channel,
            std::string week_of_install);

  void Update();

  std::string platform;
  std::string version;
  std::string channel;
  base::Time date_of_install;
  base::Time date_of_survey;
  int woi;  // Week of install. Remove this occasionally and extract from above.
  std::string country_code;

 private:
  // Used to report major/minor version numbers to reduce amount of
  // Constellation tags
  void InitVersion();

  // Ensures that country represent the big enough cohort that will not
  // let anybody identify the sender.
  void MaybeStripCountry();
};

base::Value::Dict GenerateP3AMessageDict(base::StringPiece metric_name,
                                         uint64_t metric_value,
                                         MetricLogType log_type,
                                         const MessageMetainfo& meta,
                                         const std::string& upload_type);

std::string GenerateP3AConstellationMessage(base::StringPiece metric_name,
                                            uint64_t metric_value,
                                            const MessageMetainfo& meta);

}  // namespace p3a

#endif  // HNS_COMPONENTS_P3A_P3A_MESSAGE_H_
