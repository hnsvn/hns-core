/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define HNS_ALL_ALGORITHMS \
  SignatureVerifier::SignatureAlgorithm::ECDSA_SHA256,
#define HNS_MEASURE_VIRTUAL_TMP_OPERATIONS \
  case SignatureVerifier::SignatureAlgorithm::ECDSA_SHA384:
#define HNS_MEASURE_TMP_OPERATIONS_INTERNAL \
  case SignatureVerifier::SignatureAlgorithm::ECDSA_SHA384:

#include "src/crypto/unexportable_key_metrics.cc"

#undef HNS_ALL_ALGORITHMS
#undef HNS_MEASURE_VIRTUAL_TMP_OPERATIONS
#undef HNS_MEASURE_TMP_OPERATIONS_INTERNAL
