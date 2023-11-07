// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

namespace blink {
class LocalFrame;

namespace {
class MediaKeySystemAccessInitializer;

void MaybeOnWidevineRequest(MediaKeySystemAccessInitializer* initializer,
                            LocalFrame* frame);
}  // namespace
}  // namespace blink

// Notify Hns about the widevine availability request.
#define HNS_NAVIGATOR_REQUEST_MEDIA_KEY_SYSTEM_ACCESS \
  MaybeOnWidevineRequest(initializer, window->GetFrame());

#include "src/third_party/blink/renderer/modules/encryptedmedia/navigator_request_media_key_system_access.cc"
#undef HNS_NAVIGATOR_REQUEST_MEDIA_KEY_SYSTEM_ACCESS

#include "hns/components/hns_drm/hns_drm.mojom-blink.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_provider.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"

namespace blink {
namespace {

// Notifies Hns about the widevine availability request.
void MaybeOnWidevineRequest(MediaKeySystemAccessInitializer* initializer,
                            LocalFrame* frame) {
  if (initializer->KeySystem() == "com.widevine.alpha") {
    if (frame->Client()->GetRemoteNavigationAssociatedInterfaces()) {
      mojo::AssociatedRemote<hns_drm::mojom::blink::HnsDRM>
          hns_drm_binding;
      frame->Client()->GetRemoteNavigationAssociatedInterfaces()->GetInterface(
          &hns_drm_binding);
      DCHECK(hns_drm_binding.is_bound());
      hns_drm_binding->OnWidevineKeySystemAccessRequest();
    }
  }
}

}  // namespace
}  // namespace blink
