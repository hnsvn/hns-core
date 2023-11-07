/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_CONTENT_SETTINGS_RENDERER_HNS_CONTENT_SETTINGS_AGENT_IMPL_H_
#define HNS_COMPONENTS_CONTENT_SETTINGS_RENDERER_HNS_CONTENT_SETTINGS_AGENT_IMPL_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/containers/flat_set.h"
#include "base/gtest_prod_util.h"
#include "hns/components/hns_shields/common/hns_shields.mojom.h"
#include "hns/third_party/blink/renderer/hns_farbling_constants.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/content_settings/renderer/content_settings_agent_impl.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"

#include "url/gurl.h"

namespace blink {
class WebLocalFrame;
}

namespace content_settings {

// Handles blocking content per content settings for each RenderFrame.
class HnsContentSettingsAgentImpl
    : public ContentSettingsAgentImpl,
      public hns_shields::mojom::HnsShields {
 public:
  HnsContentSettingsAgentImpl(content::RenderFrame* render_frame,
                                bool should_whitelist,
                                std::unique_ptr<Delegate> delegate);
  HnsContentSettingsAgentImpl(const HnsContentSettingsAgentImpl&) = delete;
  HnsContentSettingsAgentImpl& operator=(
      const HnsContentSettingsAgentImpl&) = delete;
  ~HnsContentSettingsAgentImpl() override;

  bool IsCosmeticFilteringEnabled(const GURL& url) override;

  bool IsFirstPartyCosmeticFilteringEnabled(const GURL& url) override;

 protected:
  bool AllowScript(bool enabled_per_settings) override;
  bool AllowScriptFromSource(bool enabled_per_settings,
                             const blink::WebURL& script_url) override;
  void DidNotAllowScript() override;

  blink::WebSecurityOrigin GetEphemeralStorageOriginSync() override;
  bool AllowStorageAccessSync(StorageType storage_type) override;

  void HnsSpecificDidBlockJavaScript(const std::u16string& details);
  void HnsSpecificDidAllowJavaScriptOnce(const GURL& details);
  bool AllowAutoplay(bool play_requested) override;

  HnsFarblingLevel GetHnsFarblingLevel() override;

  bool IsReduceLanguageEnabled() override;

 private:
  FRIEND_TEST_ALL_PREFIXES(HnsContentSettingsAgentImplAutoplayBrowserTest,
                           AutoplayBlockedByDefault);
  FRIEND_TEST_ALL_PREFIXES(HnsContentSettingsAgentImplAutoplayBrowserTest,
                           AutoplayAllowedByDefault);

  bool IsHnsShieldsDown(const blink::WebFrame* frame,
                          const GURL& secondary_url);

  bool IsScriptTemporilyAllowed(const GURL& script_url);

  // hns_shields::mojom::HnsShields.
  void SetAllowScriptsFromOriginsOnce(
      const std::vector<std::string>& origins) override;
  void SetReduceLanguageEnabled(bool enabled) override;

  void BindHnsShieldsReceiver(
      mojo::PendingAssociatedReceiver<hns_shields::mojom::HnsShields>
          pending_receiver);

  // Returns the associated remote used to send messages to the browser process,
  // lazily initializing it the first time it's used.
  mojo::AssociatedRemote<hns_shields::mojom::HnsShieldsHost>&
  GetOrCreateHnsShieldsRemote();

  // Origins of scripts which are temporary allowed for this frame in the
  // current load
  base::flat_set<std::string> temporarily_allowed_scripts_;

  // cache blocked script url which will later be used in `DidNotAllowScript()`
  GURL blocked_script_url_;

  // Status of "reduce language identifiability" feature.
  bool reduce_language_enabled_;

  base::flat_map<url::Origin, blink::WebSecurityOrigin>
      cached_ephemeral_storage_origins_;

  mojo::AssociatedRemote<hns_shields::mojom::HnsShieldsHost>
      hns_shields_remote_;

  mojo::AssociatedReceiverSet<hns_shields::mojom::HnsShields>
      hns_shields_receivers_;
};

}  // namespace content_settings

#endif  // HNS_COMPONENTS_CONTENT_SETTINGS_RENDERER_HNS_CONTENT_SETTINGS_AGENT_IMPL_H_
