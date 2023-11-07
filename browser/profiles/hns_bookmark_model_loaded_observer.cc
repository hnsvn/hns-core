/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/profiles/hns_bookmark_model_loaded_observer.h"

#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/prefs/pref_service.h"

using bookmarks::BookmarkModel;

HnsBookmarkModelLoadedObserver::HnsBookmarkModelLoadedObserver(
    Profile* profile)
    : BookmarkModelLoadedObserver(profile) {}

void HnsBookmarkModelLoadedObserver::BookmarkModelLoaded(
    BookmarkModel* model,
    bool ids_reassigned) {
  if (!profile_->GetPrefs()->GetBoolean(kOtherBookmarksMigrated)) {
    HnsMigrateOtherNodeFolder(model);
    profile_->GetPrefs()->SetBoolean(kOtherBookmarksMigrated, true);
  }

  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  if (!hns_sync_prefs.IsSyncV1MetaInfoCleared()) {
    HnsClearSyncV1MetaInfo(model);
    hns_sync_prefs.SetSyncV1MetaInfoCleared(true);
  }

  BookmarkModelLoadedObserver::BookmarkModelLoaded(model, ids_reassigned);
}
