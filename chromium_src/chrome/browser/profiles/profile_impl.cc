/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/profiles/profile_impl.h"

#include "hns/browser/hns_browser_features.h"
#include "hns/browser/profiles/hns_bookmark_model_loaded_observer.h"

#define BookmarkModelLoadedObserver HnsBookmarkModelLoadedObserver
#define ChromeBrowsingDataRemoverDelegate HnsBrowsingDataRemoverDelegate
#define ShouldRestoreOldSessionCookies \
  ShouldRestoreOldSessionCookies_ChromiumImpl

#include "src/chrome/browser/profiles/profile_impl.cc"

#undef ShouldRestoreOldSessionCookies
#undef BookmarkModelLoadedObserver
#undef ChromeBrowsingDataRemoverDelegate

bool ProfileImpl::ShouldRestoreOldSessionCookies() {
  bool should_restore = ShouldRestoreOldSessionCookies_ChromiumImpl();
  if (base::FeatureList::IsEnabled(
          features::kHnsCleanupSessionCookiesOnSessionRestore)) {
#if BUILDFLAG(IS_ANDROID)
    should_restore = false;
#else   // !BUILDFLAG(IS_ANDROID)
    if (ExitTypeService::GetLastSessionExitType(this) != ExitType::kCrashed) {
      should_restore &= StartupBrowserCreator::WasRestarted();
    }
#endif  // BUILDFLAG(IS_ANDROID)
  }
  return should_restore;
}
