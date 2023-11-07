/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_drm_tab_helper.h"

#include <utility>
#include <vector>

#include "base/containers/contains.h"
#include "hns/browser/widevine/widevine_utils.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/widevine/constants.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process_impl.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/navigation_handle.h"

using component_updater::ComponentUpdateService;

namespace {
bool IsAlreadyRegistered(ComponentUpdateService* cus) {
  return base::Contains(cus->GetComponentIDs(), kWidevineComponentId);
}
#if !BUILDFLAG(IS_LINUX)
content::WebContents* GetActiveWebContents() {
  if (Browser* browser = chrome::FindLastActive())
    return browser->tab_strip_model()->GetActiveWebContents();
  return nullptr;
}

void ReloadIfActive(content::WebContents* web_contents) {
  if (GetActiveWebContents() == web_contents)
    web_contents->GetController().Reload(content::ReloadType::NORMAL, false);
}
#endif

}  // namespace

HnsDrmTabHelper::HnsDrmTabHelper(content::WebContents* contents)
    : WebContentsObserver(contents),
      content::WebContentsUserData<HnsDrmTabHelper>(*contents),
      hns_drm_receivers_(contents, this) {
  auto* updater = g_browser_process->component_updater();
  // We don't need to observe if widevine is already registered.
  if (!IsAlreadyRegistered(updater))
    observer_.Observe(updater);
}

HnsDrmTabHelper::~HnsDrmTabHelper() = default;

// static
void HnsDrmTabHelper::BindHnsDRM(
    mojo::PendingAssociatedReceiver<hns_drm::mojom::HnsDRM> receiver,
    content::RenderFrameHost* rfh) {
  auto* web_contents = content::WebContents::FromRenderFrameHost(rfh);
  if (!web_contents)
    return;

  auto* tab_helper = HnsDrmTabHelper::FromWebContents(web_contents);
  if (!tab_helper)
    return;
  tab_helper->hns_drm_receivers_.Bind(rfh, std::move(receiver));
}

bool HnsDrmTabHelper::ShouldShowWidevineOptIn() const {
  // If the user already opted in, don't offer it.
  PrefService* prefs =
      static_cast<Profile*>(web_contents()->GetBrowserContext())->GetPrefs();
  if (IsWidevineOptedIn() || !prefs->GetBoolean(kAskWidevineInstall)) {
    return false;
  }

  return is_widevine_requested_;
}

void HnsDrmTabHelper::DidStartNavigation(
    content::NavigationHandle* navigation_handle) {
  if (!navigation_handle->IsInMainFrame() ||
      navigation_handle->IsSameDocument()) {
    return;
  }
  is_widevine_requested_ = false;
  is_permission_requested_ = false;
}

void HnsDrmTabHelper::OnWidevineKeySystemAccessRequest() {
  is_widevine_requested_ = true;

  if (ShouldShowWidevineOptIn() && !is_permission_requested_) {
    is_permission_requested_ = true;
    RequestWidevinePermission(web_contents(), false /* for_restart */);
  }
}

void HnsDrmTabHelper::OnEvent(Events event, const std::string& id) {
  if (event == ComponentUpdateService::Observer::Events::COMPONENT_UPDATED &&
      id == kWidevineComponentId) {
#if BUILDFLAG(IS_LINUX)
    // Ask restart instead of reloading. Widevine is only usable after
    // restarting on linux. This restart permission request is only shown if
    // this tab asks widevine explicitely.
    if (is_widevine_requested_)
      RequestWidevinePermission(web_contents(), true /* for_restart*/);
#else
    // When widevine is ready to use, only active tab that requests widevine is
    // reloaded automatically.
    if (is_widevine_requested_)
      ReloadIfActive(web_contents());
#endif
    // Stop observing component update event.
    observer_.Reset();
  }
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(HnsDrmTabHelper);
