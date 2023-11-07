/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/core/execution_context/navigator_base.h"

#include "base/compiler_specific.h"
#include "base/system/sys_info.h"
#include "hns/third_party/blink/renderer/core/farbling/hns_session_cache.h"
#include "third_party/abseil-cpp/absl/random/random.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/loader/frame_loader.h"
#include "third_party/blink/renderer/core/probe/core_probes.h"

namespace blink {
namespace probe {

void ApplyHnsHardwareConcurrencyOverride(blink::ExecutionContext* context,
                                           unsigned int* hardware_concurrency) {
  const unsigned kFakeMinProcessors = 2;
  const unsigned kFakeMaxProcessors = 8;
  unsigned true_value =
      static_cast<unsigned>(base::SysInfo::NumberOfProcessors());
  if (true_value <= 2) {
    *hardware_concurrency = true_value;
    return;
  }
  unsigned farbled_value = true_value;
  switch (hns::GetHnsFarblingLevelFor(context, HnsFarblingLevel::OFF)) {
    case HnsFarblingLevel::OFF: {
      break;
    }
    case HnsFarblingLevel::MAXIMUM: {
      true_value = kFakeMaxProcessors;
      // "Maximum" behavior is "balanced" behavior but with a fake maximum,
      // so fall through here.
      [[fallthrough]];
    }
    case HnsFarblingLevel::BALANCED: {
      hns::FarblingPRNG prng =
          hns::HnsSessionCache::From(*context).MakePseudoRandomGenerator();
      farbled_value =
          kFakeMinProcessors + (prng() % (true_value + 1 - kFakeMinProcessors));
      break;
    }
    default:
      NOTREACHED();
  }
  *hardware_concurrency = farbled_value;
}

}  // namespace probe
}  // namespace blink

#define userAgent userAgent_ChromiumImpl
#define ApplyHardwareConcurrencyOverride                        \
  ApplyHnsHardwareConcurrencyOverride(GetExecutionContext(),  \
                                        &hardware_concurrency); \
  probe::ApplyHardwareConcurrencyOverride

#include "src/third_party/blink/renderer/core/execution_context/navigator_base.cc"
#undef ApplyHardwareConcurrencyOverride
#undef userAgent

namespace blink {

String NavigatorBase::userAgent() const {
  if (ExecutionContext* context = GetExecutionContext()) {
    if (!hns::AllowFingerprinting(context)) {
      return hns::HnsSessionCache::From(*context).FarbledUserAgent(
          context->UserAgent());
    }
  }

  return userAgent_ChromiumImpl();
}

}  // namespace blink
