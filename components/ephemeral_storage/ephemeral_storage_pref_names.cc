/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/ephemeral_storage/ephemeral_storage_pref_names.h"

namespace ephemeral_storage {

// Stores origins to perform a storage cleanup on browser restart.
const char kFirstPartyStorageOriginsToCleanup[] =
    "ephemeral_storage.first_party_storage_origins_to_cleanup";

}  // namespace ephemeral_storage
