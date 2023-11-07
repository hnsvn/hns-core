/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_

#define HNS_VISIT_DEVICE_INFO_SPECIFICS_HNS_FIELDS \
VISIT(hns_fields);

#define HNS_VISIT_PROTO_FIELDS_HNS_SPECIFIC_FIELD                  \
VISIT_PROTO_FIELDS(const sync_pb::HnsSpecificFields& proto) {        \
  VISIT(is_self_delete_supported);                                     \
}

#include "src/components/sync/protocol/proto_visitors.h"  // IWYU pragma: export

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_SYNC_PROTOCOL_PROTO_VISITORS_H_
