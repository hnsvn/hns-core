/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/tx_meta.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace hns_wallet {

TEST(TxMetaUnitTest, GenerateMetaID) {
  EXPECT_NE(TxMeta::GenerateMetaID(), TxMeta::GenerateMetaID());
}

}  // namespace hns_wallet
