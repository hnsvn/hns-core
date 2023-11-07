/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_ENTERPRISE_CONNECTORS_DEVICE_TRUST_KEY_MANAGEMENT_CORE_NETWORK_KEY_UPLOAD_REQUEST_CC_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_ENTERPRISE_CONNECTORS_DEVICE_TRUST_KEY_MANAGEMENT_CORE_NETWORK_KEY_UPLOAD_REQUEST_CC_

#include "hns/chromium_src/crypto/signature_verifier.h"  // IWYU pragma: export

#define ECDSA_SHA256 \
  ECDSA_SHA256:      \
  case crypto::SignatureVerifier::ECDSA_SHA384

#include "src/chrome/browser/enterprise/connectors/device_trust/key_management/core/network/key_upload_request.cc"

#undef ECDSA_SHA256

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_ENTERPRISE_CONNECTORS_DEVICE_TRUST_KEY_MANAGEMENT_CORE_NETWORK_KEY_UPLOAD_REQUEST_CC_
