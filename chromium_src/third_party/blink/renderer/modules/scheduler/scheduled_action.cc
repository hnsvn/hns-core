/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/modules/scheduler/scheduled_action.h"

#include "hns/components/hns_page_graph/common/buildflags.h"
#include "hns/v8/include/v8-isolate-page-graph-utils.h"
#include "third_party/blink/renderer/core/probe/core_probes.h"
#include "third_party/blink/renderer/core/script/classic_script.h"
#include "third_party/blink/renderer/platform/loader/fetch/script_fetch_options.h"

#define HNS_SCHEDULED_ACTION_STRING_HANDLER_CONSTRUCTOR                \
  IF_BUILDFLAG(ENABLE_HNS_PAGE_GRAPH, {                                \
    if (!code_.empty() &&                                                \
        CoreProbeSink::HasAgentsGlobal(CoreProbeSink::kPageGraph)) {     \
      parent_script_id_ =                                                \
          v8::page_graph::GetExecutingScript(script_state->GetIsolate()) \
              .script_id;                                                \
    }                                                                    \
  })

#define ScriptFetchOptions GetScriptFetchOptions

#include "src/third_party/blink/renderer/modules/scheduler/scheduled_action.cc"

#undef ScriptFetchOptions
#undef HNS_SCHEDULED_ACTION_STRING_HANDLER_CONSTRUCTOR

namespace blink {

ScriptFetchOptions ScheduledAction::GetScriptFetchOptions() const {
  ScriptFetchOptions script_fetch_options;
#if BUILDFLAG(ENABLE_HNS_PAGE_GRAPH)
  script_fetch_options.SetParentScriptId(parent_script_id_);
#endif
  return script_fetch_options;
}

}  // namespace blink
