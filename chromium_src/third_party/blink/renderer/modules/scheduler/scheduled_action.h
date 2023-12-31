/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_SCHEDULER_SCHEDULED_ACTION_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_SCHEDULER_SCHEDULED_ACTION_H_

#include "hns/components/hns_page_graph/common/buildflags.h"

namespace blink {
class ScriptFetchOptions;
}

#define arguments_                                                  \
  arguments_;                                                       \
  IF_BUILDFLAG(ENABLE_HNS_PAGE_GRAPH, int parent_script_id_ = 0;) \
  ScriptFetchOptions GetScriptFetchOptions() const

#include "src/third_party/blink/renderer/modules/scheduler/scheduled_action.h"  // IWYU pragma: export

#undef arguments_

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_SCHEDULER_SCHEDULED_ACTION_H_
