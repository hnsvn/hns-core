/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_sync/features.h"

#include "base/feature_list.h"

namespace hns_sync {
namespace features {

BASE_FEATURE(kHnsSync, "HnsSync", base::FEATURE_ENABLED_BY_DEFAULT);

// When this feature is enabled through hns://flags it adds to history entry's
// title additional info for sync diagnostics:
// - whether history entry should be synced;
// - typed count;
// - page transition.
BASE_FEATURE(kHnsSyncHistoryDiagnostics,
             "HnsSyncHistoryDiagnostics",
             base::FEATURE_DISABLED_BY_DEFAULT);

// When this feature is enabled, Sync sends to remote server all the history
// entries, including page transition beyond typed url (link, bookmark, reload,
// etc).
BASE_FEATURE(kHnsSyncSendAllHistory,
             "HnsSyncSendAllHistory",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace features
}  // namespace hns_sync
