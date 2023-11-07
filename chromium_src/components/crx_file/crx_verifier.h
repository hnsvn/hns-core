/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_CRX_FILE_CRX_VERIFIER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_CRX_FILE_CRX_VERIFIER_H_

#include "src/components/crx_file/crx_verifier.h"  // IWYU pragma: export

namespace crx_file {

void SetHnsPublisherKeyHashForTesting(const std::vector<uint8_t>& test_key);

}  // namespace crx_file

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_CRX_FILE_CRX_VERIFIER_H_
