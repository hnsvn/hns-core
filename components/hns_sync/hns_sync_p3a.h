/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SYNC_HNS_SYNC_P3A_H_
#define HNS_COMPONENTS_HNS_SYNC_HNS_SYNC_P3A_H_

#include "components/sync/base/user_selectable_type.h"

namespace hns_sync {
namespace p3a {

// TODO(alexeybarabash): move here also "Hns.Sync.Status.2" and
// "Hns.Sync.ProgressTokenEverReset"
constexpr char kEnabledTypesHistogramName[] = "Hns.Sync.EnabledTypes";
constexpr char kSyncedObjectsCountHistogramName[] =
    "Hns.Sync.SyncedObjectsCount";

enum class EnabledTypesAnswer {
  kEmptyOrBookmarksOnly = 0,
  kBookmarksAndHistory = 1,
  kMoreThanBookmarksAndHistory = 2,
  kAllTypes = 3,
  kMaxValue = kAllTypes
};

void RecordEnabledTypes(bool sync_everything_enabled,
                        const syncer::UserSelectableTypeSet& selected_types);
void RecordSyncedObjectsCount(int total_entities);

}  // namespace p3a
}  // namespace hns_sync

#endif  // HNS_COMPONENTS_HNS_SYNC_HNS_SYNC_P3A_H_
