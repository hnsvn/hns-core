/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_SHIELDS_HNS_SHIELDS_WEB_CONTENTS_OBSERVER_H_
#define HNS_BROWSER_HNS_SHIELDS_HNS_SHIELDS_WEB_CONTENTS_OBSERVER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/synchronization/lock.h"
#include "hns/components/hns_shields/common/hns_shields.mojom.h"
#include "content/public/browser/render_frame_host_receiver_set.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class WebContents;
}

class PrefRegistrySimple;

namespace hns_shields {

class HnsShieldsWebContentsObserver
    : public content::WebContentsObserver,
      public content::WebContentsUserData<HnsShieldsWebContentsObserver>,
      public hns_shields::mojom::HnsShieldsHost {
 public:
  explicit HnsShieldsWebContentsObserver(content::WebContents*);
  HnsShieldsWebContentsObserver(const HnsShieldsWebContentsObserver&) =
      delete;
  HnsShieldsWebContentsObserver& operator=(
      const HnsShieldsWebContentsObserver&) = delete;
  ~HnsShieldsWebContentsObserver() override;

  static void BindHnsShieldsHost(
      mojo::PendingAssociatedReceiver<hns_shields::mojom::HnsShieldsHost>
          receiver,
      content::RenderFrameHost* rfh);

  static void RegisterProfilePrefs(PrefRegistrySimple* registry);
  static void DispatchBlockedEventForWebContents(
      const std::string& block_type,
      const std::string& subresource,
      content::WebContents* web_contents);
  static void DispatchAllowedOnceEventForWebContents(
      const std::string& block_type,
      const std::string& subresource,
      content::WebContents* web_contents);
  static void DispatchBlockedEvent(const GURL& request_url,
                                   int frame_tree_node_id,
                                   const std::string& block_type);
  static GURL GetTabURLFromRenderFrameInfo(int render_frame_tree_node_id);
  void AllowScriptsOnce(const std::vector<std::string>& origins);
  void BlockAllowedScripts(const std::vector<std::string>& origins);
  bool IsBlockedSubresource(const std::string& subresource);
  void AddBlockedSubresource(const std::string& subresource);

 protected:
  // content::WebContentsObserver overrides.
  void RenderFrameCreated(content::RenderFrameHost* host) override;
  void RenderFrameDeleted(content::RenderFrameHost* render_frame_host) override;
  void RenderFrameHostChanged(content::RenderFrameHost* old_host,
                              content::RenderFrameHost* new_host) override;
  void ReadyToCommitNavigation(
      content::NavigationHandle* navigation_handle) override;

  // hns_shields::mojom::HnsShieldsHost.
  void OnJavaScriptBlocked(const std::u16string& details) override;
  void OnJavaScriptAllowedOnce(const std::u16string& details) override;

 private:
  friend class content::WebContentsUserData<HnsShieldsWebContentsObserver>;
  friend class HnsShieldsWebContentsObserverBrowserTest;

  using HnsShieldsRemotesMap = base::flat_map<
      content::RenderFrameHost*,
      mojo::AssociatedRemote<hns_shields::mojom::HnsShields>>;

  // Allows indicating a implementor of hns_shields::mojom::HnsShieldsHost
  // other than this own class, for testing purposes only.
  static void SetReceiverImplForTesting(HnsShieldsWebContentsObserver* impl);

  // Only used from the BindHnsShieldsHost() static method, useful to bind the
  // mojo receiver of hns_shields::mojom::HnsShieldsHost to a different
  // implementor when needed, for testing purposes.
  void BindReceiver(mojo::PendingAssociatedReceiver<
                        hns_shields::mojom::HnsShieldsHost> receiver,
                    content::RenderFrameHost* rfh);

  // Return an already bound remote for the hns_shields::mojom::HnsShields
  // mojo interface. It is an error to call this method with an invalid |rfh|.
  mojo::AssociatedRemote<hns_shields::mojom::HnsShields>&
  GetHnsShieldsRemote(content::RenderFrameHost* rfh);

  std::vector<std::string> allowed_scripts_;
  // We keep a set of the current page's blocked URLs in case the page
  // continually tries to load the same blocked URLs.
  std::set<std::string> blocked_url_paths_;

  content::RenderFrameHostReceiverSet<hns_shields::mojom::HnsShieldsHost>
      receivers_;

  // Map of remote endpoints for the hns_shields::mojom::HnsShields mojo
  // interface, to prevent binding a new remote each time it's used.
  HnsShieldsRemotesMap hns_shields_remotes_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace hns_shields

#endif  // HNS_BROWSER_HNS_SHIELDS_HNS_SHIELDS_WEB_CONTENTS_OBSERVER_H_
