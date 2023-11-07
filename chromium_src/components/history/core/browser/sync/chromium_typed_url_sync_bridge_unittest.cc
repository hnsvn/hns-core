/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_sync/features.h"
#include "hns/components/history/core/browser/sync/hns_typed_url_sync_bridge.h"
#include "components/history/core/browser/sync/typed_url_sync_bridge.h"
#include "components/sync/model/model_type_sync_bridge.h"

#define HNS_TEST_MEMBERS_DECLARE \
  base::test::ScopedFeatureList scoped_feature_list_;

#define HNS_TEST_MEMBERS_INIT          \
  scoped_feature_list_.InitWithFeatures( \
      {}, {hns_sync::features::kHnsSyncSendAllHistory});

#define TypedURLSyncBridge HnsTypedURLSyncBridge

#include "src/components/history/core/browser/sync/typed_url_sync_bridge_unittest.cc"

#undef TypedURLSyncBridge

#undef HNS_TEST_MEMBERS_INIT
#undef HNS_TEST_MEMBERS_DECLARE
