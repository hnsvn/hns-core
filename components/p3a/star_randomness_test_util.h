/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_P3A_STAR_RANDOMNESS_TEST_UTIL_H_
#define HNS_COMPONENTS_P3A_STAR_RANDOMNESS_TEST_UTIL_H_

#include <string>

namespace network {
struct ResourceRequest;
}  // namespace network

namespace p3a {

std::string HandleRandomnessRequest(const network::ResourceRequest& request,
                                    uint8_t expected_epoch);

}  // namespace p3a

#endif  // HNS_COMPONENTS_P3A_STAR_RANDOMNESS_TEST_UTIL_H_
