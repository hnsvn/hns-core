/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_

#include "base/gtest_prod_util.h"

namespace syncer {

FORWARD_DECLARE_TEST(HnsModelTypeWorkerTest, ResetProgressMarker);
FORWARD_DECLARE_TEST(HnsModelTypeWorkerTest, ResetProgressMarkerMaxPeriod);

}  // namespace syncer

#define HNS_MODEL_TYPE_WORKER_H_                                         \
 private:                                                                  \
  friend class HnsModelTypeWorker;                                       \
  friend class HnsModelTypeWorkerTest;                                   \
  FRIEND_TEST_ALL_PREFIXES(HnsModelTypeWorkerTest, ResetProgressMarker); \
  FRIEND_TEST_ALL_PREFIXES(HnsModelTypeWorkerTest,                       \
                           ResetProgressMarkerMaxPeriod);

#define OnCommitResponse virtual OnCommitResponse

#include "src/components/sync/engine/model_type_worker.h"  // IWYU pragma: export

#undef OnCommitResponse
#undef HNS_MODEL_TYPE_WORKER_H_

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_
