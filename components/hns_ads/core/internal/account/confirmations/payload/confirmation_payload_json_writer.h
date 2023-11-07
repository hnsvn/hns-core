/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_PAYLOAD_CONFIRMATION_PAYLOAD_JSON_WRITER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_PAYLOAD_CONFIRMATION_PAYLOAD_JSON_WRITER_H_

#include <string>

namespace hns_ads {

struct ConfirmationInfo;

namespace json::writer {

std::string WriteConfirmationPayload(const ConfirmationInfo& confirmation);

}  // namespace json::writer
}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_PAYLOAD_CONFIRMATION_PAYLOAD_JSON_WRITER_H_
