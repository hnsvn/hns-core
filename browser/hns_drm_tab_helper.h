/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_DRM_TAB_HELPER_H_
#define HNS_BROWSER_HNS_DRM_TAB_HELPER_H_

#include <string>

#include "base/scoped_observation.h"
#include "hns/components/hns_drm/hns_drm.mojom.h"
#include "components/component_updater/component_updater_service.h"
#include "content/public/browser/render_frame_host_receiver_set.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

// Reacts to DRM content detected on the renderer side.
class HnsDrmTabHelper final
    : public content::WebContentsObserver,
      public content::WebContentsUserData<HnsDrmTabHelper>,
      public hns_drm::mojom::HnsDRM,
      public component_updater::ComponentUpdateService::Observer {
 public:
  explicit HnsDrmTabHelper(content::WebContents* contents);
  ~HnsDrmTabHelper() override;

  static void BindHnsDRM(
      mojo::PendingAssociatedReceiver<hns_drm::mojom::HnsDRM> receiver,
      content::RenderFrameHost* rfh);

  bool ShouldShowWidevineOptIn() const;

  // content::WebContentsObserver
  void DidStartNavigation(
      content::NavigationHandle* navigation_handle) override;

  // blink::mojom::HnsDRM
  void OnWidevineKeySystemAccessRequest() override;

  // component_updater::ComponentUpdateService::Observer
  void OnEvent(Events event, const std::string& id) override;

  WEB_CONTENTS_USER_DATA_KEY_DECL();

 private:
  content::RenderFrameHostReceiverSet<hns_drm::mojom::HnsDRM>
      hns_drm_receivers_;

  // Permission request is done only once during the navigation. If user
  // chooses dismiss/deny, additional request is added again only when new
  // main frame navigation is started.
  bool is_permission_requested_ = false;

  // True if we are notified that a page requested widevine availability.
  bool is_widevine_requested_ = false;

  base::ScopedObservation<component_updater::ComponentUpdateService,
                          component_updater::ComponentUpdateService::Observer>
      observer_{this};
};

#endif  // HNS_BROWSER_HNS_DRM_TAB_HELPER_H_
