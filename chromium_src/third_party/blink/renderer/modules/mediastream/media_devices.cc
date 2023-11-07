/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/farbling/hns_session_cache.h"
#include "third_party/abseil-cpp/absl/random/random.h"
#include "third_party/blink/renderer/modules/mediastream/media_device_info.h"

using blink::ExecutionContext;
using blink::MediaDeviceInfoVector;

namespace hns {

void FarbleMediaDevices(ExecutionContext* context,
                        MediaDeviceInfoVector* media_devices) {
  // |media_devices| is guaranteed not to be null here.
  if (media_devices->size() <= 2)
    return;
  if (GetHnsFarblingLevelFor(context, HnsFarblingLevel::OFF) ==
      HnsFarblingLevel::OFF)
    return;
  // Shuffle the list of devices pseudo-randomly, based on the
  // domain+session key, starting with the second device.
  FarblingPRNG prng =
      HnsSessionCache::From(*context).MakePseudoRandomGenerator();
  MediaDeviceInfoVector::iterator it_begin = media_devices->begin();
  std::shuffle(++it_begin, media_devices->end(), prng);
}

}  // namespace hns

#define HNS_MEDIA_DEVICES_DEVICES_ENUMERATED                        \
  if (ExecutionContext* context =                                     \
          ExecutionContext::From(result_tracker->GetScriptState())) { \
    hns::FarbleMediaDevices(context, &media_devices);               \
  }

#include "src/third_party/blink/renderer/modules/mediastream/media_devices.cc"
#undef HNS_MEDIA_DEVICES_DEVICES_ENUMERATED
