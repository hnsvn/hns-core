/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "hns/ios/browser/api/sync/driver/hns_sync_profile_service.h"

#include <unordered_map>

#include "components/sync/base/model_type.h"
#include "components/sync/base/user_selectable_type.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_user_settings.h"
#include "ios/web/public/thread/web_thread.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace hns {
namespace ios {
std::unordered_map<syncer::UserSelectableType, HnsSyncUserSelectableTypes>
    mapping = {
        {syncer::UserSelectableType::kBookmarks,
         HnsSyncUserSelectableTypes_BOOKMARKS},
        {syncer::UserSelectableType::kPreferences,
         HnsSyncUserSelectableTypes_PREFERENCES},
        {syncer::UserSelectableType::kPasswords,
         HnsSyncUserSelectableTypes_PASSWORDS},
        {syncer::UserSelectableType::kAutofill,
         HnsSyncUserSelectableTypes_AUTOFILL},
        {syncer::UserSelectableType::kThemes,
         HnsSyncUserSelectableTypes_THEMES},
        {syncer::UserSelectableType::kHistory,
         HnsSyncUserSelectableTypes_HISTORY},
        {syncer::UserSelectableType::kExtensions,
         HnsSyncUserSelectableTypes_EXTENSIONS},
        {syncer::UserSelectableType::kApps, HnsSyncUserSelectableTypes_APPS},
        {syncer::UserSelectableType::kReadingList,
         HnsSyncUserSelectableTypes_READING_LIST},
        {syncer::UserSelectableType::kTabs, HnsSyncUserSelectableTypes_TABS}};

syncer::UserSelectableTypeSet user_types_from_options(
    HnsSyncUserSelectableTypes options) {
  syncer::UserSelectableTypeSet results;
  for (auto it = mapping.begin(); it != mapping.end(); ++it) {
    if (options & it->second) {
      results.Put(it->first);
    }
  }
  return results;
}

HnsSyncUserSelectableTypes options_from_user_types(
    const syncer::UserSelectableTypeSet& types) {
  HnsSyncUserSelectableTypes results = HnsSyncUserSelectableTypes_NONE;
  for (auto it = mapping.begin(); it != mapping.end(); ++it) {
    if (types.Has(it->first)) {
      results |= it->second;
    }
  }
  return results;
}
}  // namespace ios
}  // namespace hns

@interface HnsSyncProfileServiceIOS () {
  syncer::SyncService* sync_service_;
  std::unordered_map<syncer::UserSelectableType, HnsSyncUserSelectableTypes>
      type_mapping;
}
@end

@implementation HnsSyncProfileServiceIOS

- (instancetype)initWithProfileSyncService:(syncer::SyncService*)syncService {
  if ((self = [super init])) {
    DCHECK_CURRENTLY_ON(web::WebThread::UI);
    sync_service_ = syncService;
  }
  return self;
}

- (bool)isSyncFeatureActive {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  return sync_service_->IsSyncFeatureActive();
}

- (HnsSyncUserSelectableTypes)activeSelectableTypes {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::ModelTypeSet active_types = sync_service_->GetActiveDataTypes();

  syncer::UserSelectableTypeSet user_types;
  for (syncer::UserSelectableType type : syncer::UserSelectableTypeSet::All()) {
    if (active_types.Has(
            syncer::UserSelectableTypeToCanonicalModelType(type))) {
      user_types.Put(type);
    }
  }
  return hns::ios::options_from_user_types(user_types);
}

- (HnsSyncUserSelectableTypes)userSelectedTypes {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::UserSelectableTypeSet types =
      sync_service_->GetUserSettings()->GetSelectedTypes();
  return hns::ios::options_from_user_types(types);
}

- (void)setUserSelectedTypes:(HnsSyncUserSelectableTypes)options {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  bool sync_everything = false;
  syncer::UserSelectableTypeSet selected_types =
      hns::ios::user_types_from_options(options);
  sync_service_->GetUserSettings()->SetSelectedTypes(sync_everything,
                                                     selected_types);
}

@end
