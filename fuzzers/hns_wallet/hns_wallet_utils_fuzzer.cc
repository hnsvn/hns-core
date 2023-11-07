/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <fuzzer/FuzzedDataProvider.h>

#include <string>

#include "base/logging.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"

struct Environment {
  Environment() { logging::SetMinLogLevel(logging::LOG_FATAL); }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  static Environment env;

  FuzzedDataProvider data_provider(data, size);

  const size_t array_size =
      data_provider.ConsumeIntegralInRange<size_t>(0, 128);
  const size_t offset = data_provider.ConsumeIntegralInRange<size_t>(0, 1024);
  const std::string input = data_provider.ConsumeRemainingBytesAsString();

  std::string output;
  hns_wallet::EncodeString(input, &output);
  hns_wallet::EncodeStringArray(std::vector<std::string>(array_size, input),
                                  &output);

  hns_wallet::DecodeString(offset, input, &output);
  return 0;
}
