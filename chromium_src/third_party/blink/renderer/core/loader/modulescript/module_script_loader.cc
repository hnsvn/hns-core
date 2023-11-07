/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_page_graph/common/buildflags.h"
#include "third_party/blink/renderer/core/probe/core_probes.h"

#define HNS_MODULE_SCRIPT_LOADER_FETCH_INTERNAL                    \
  IF_BUILDFLAG(ENABLE_HNS_PAGE_GRAPH, {                            \
    if (CoreProbeSink::HasAgentsGlobal(CoreProbeSink::kPageGraph)) { \
      options.initiator_info.dom_node_id =                           \
          module_request.Options().GetDOMNodeId();                   \
      options.initiator_info.parent_script_id =                      \
          module_request.Options().GetParentScriptId();              \
    }                                                                \
  })

#include "src/third_party/blink/renderer/core/loader/modulescript/module_script_loader.cc"

#undef HNS_MODULE_SCRIPT_LOADER_FETCH_INTERNAL
