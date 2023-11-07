/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/sync/hns_sync_alerts_service.h"

#include "hns/browser/infobars/hns_sync_account_deleted_infobar_delegate.h"
#include "hns/components/sync/service/hns_sync_service_impl.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sync/sync_service_factory.h"
#include "chrome/browser/ui/browser_finder.h"
#include "components/infobars/content/content_infobar_manager.h"

#if BUILDFLAG(IS_ANDROID)
#include "base/android/jni_android.h"
#include "hns/build/android/jni_headers/HnsSyncAccountDeletedInformer_jni.h"
#else
#include "chrome/browser/ui/browser.h"
#endif

using syncer::HnsSyncServiceImpl;

HnsSyncAlertsService::HnsSyncAlertsService(Profile* profile)
    : profile_(profile) {
  DCHECK(SyncServiceFactory::IsSyncAllowed(profile));

  if (SyncServiceFactory::IsSyncAllowed(profile)) {
    HnsSyncServiceImpl* service = static_cast<HnsSyncServiceImpl*>(
        SyncServiceFactory::GetForProfile(profile_));

    if (service) {
      DCHECK(!sync_service_observer_.IsObservingSource(service));
      sync_service_observer_.AddObservation(service);
    }
  }
}

HnsSyncAlertsService::~HnsSyncAlertsService() {}

void HnsSyncAlertsService::OnStateChanged(syncer::SyncService* service) {
  hns_sync::Prefs hns_sync_prefs(profile_->GetPrefs());
  if (!hns_sync_prefs.IsSyncAccountDeletedNoticePending()) {
    return;
  }

  ShowInfobar();
}

void HnsSyncAlertsService::OnSyncShutdown(syncer::SyncService* sync_service) {
  if (sync_service_observer_.IsObservingSource(sync_service)) {
    sync_service_observer_.RemoveObservation(sync_service);
  }
}

void HnsSyncAlertsService::ShowInfobar() {
#if BUILDFLAG(IS_ANDROID)
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsSyncAccountDeletedInformer_show(env);
#else
  Browser* browser = chrome::FindLastActive();
  if (browser) {
    content::WebContents* active_web_contents =
        browser->tab_strip_model()->GetActiveWebContents();
    if (active_web_contents) {
      HnsSyncAccountDeletedInfoBarDelegate::Create(active_web_contents,
                                                     profile_, browser);
    }
  }
#endif
}
