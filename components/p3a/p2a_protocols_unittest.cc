// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/p3a/p2a_protocols.h"

#include <stdint.h>

#include <vector>

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=P2A*

namespace {
const size_t kTrials = 1000;
}  // namespace

namespace p3a {

TEST(P2AProtocolsTest, PerturbationTest) {
  // Arrange
  const uint16_t kBucketCount = 7;
  const uint64_t kTrueValue = 4;

  // Act
  size_t true_value_count = 0;

  for (size_t i = 0; i < kTrials; i++) {
    if (kTrueValue ==
        DirectEncodingProtocol::Perturb(kBucketCount, kTrueValue)) {
      true_value_count++;
    }
  }

  // Assert
  // TODO(Moritz Haller): Maybe assert that |true_value_count| is within
  // confidence interval (e.g. |53 < true_value_count < 61| for 99% confidence
  // level)
  EXPECT_TRUE(true_value_count < kTrials);
}

}  // namespace p3a