/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_ANDROID_HNS_SYNC_WORKER_H_
#define HNS_BROWSER_ANDROID_HNS_SYNC_WORKER_H_

#include <jni.h>
#include <string>

#include "base/android/jni_weak_ref.h"
#include "base/memory/raw_ptr.h"
#include "base/scoped_multi_source_observation.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_service_observer.h"

class Profile;

namespace syncer {
class HnsSyncServiceImpl;
}  // namespace syncer

namespace chrome {
namespace android {

class HnsSyncWorker : public syncer::SyncServiceObserver {
 public:
  HnsSyncWorker(JNIEnv* env,
                  const base::android::JavaRef<jobject>& obj);
  HnsSyncWorker(const HnsSyncWorker&) = delete;
  HnsSyncWorker& operator=(const HnsSyncWorker&) = delete;
  ~HnsSyncWorker() override;

  void Destroy(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jstring> GetSyncCodeWords(JNIEnv* env);

  void SaveCodeWords(JNIEnv* env,
                     const base::android::JavaParamRef<jstring>& passphrase);

  void RequestSync(JNIEnv* env);

  void FinalizeSyncSetup(JNIEnv* env);

  bool IsInitialSyncFeatureSetupComplete(JNIEnv* env);

  void ResetSync(JNIEnv* env);

  bool GetSyncV1WasEnabled(JNIEnv* env);

  bool GetSyncV2MigrateNoticeDismissed(JNIEnv* env);

  void SetSyncV2MigrateNoticeDismissed(
      JNIEnv* env,
      bool sync_v2_migration_notice_dismissed);

  void PermanentlyDeleteAccount(
      JNIEnv* env,
      const base::android::JavaParamRef<jobject>& callback);

  void SetJoinSyncChainCallback(
      JNIEnv* env,
      const base::android::JavaParamRef<jobject>& callback);

  void ClearAccountDeletedNoticePending(JNIEnv* env);
  bool IsAccountDeletedNoticePending(JNIEnv* env);

 private:
  syncer::HnsSyncServiceImpl* GetSyncService() const;
  void MarkFirstSetupComplete();

  // syncer::SyncServiceObserver implementation.
  void OnStateChanged(syncer::SyncService* service) override;

  void OnResetDone();

  void SetEncryptionPassphrase(syncer::SyncService* service);
  void SetDecryptionPassphrase(syncer::SyncService* service);

  JavaObjectWeakGlobalRef weak_java_hns_sync_worker_;
  raw_ptr<Profile> profile_ = nullptr;

  std::string passphrase_;

  base::ScopedMultiSourceObservation<syncer::SyncService,
                                     syncer::SyncServiceObserver>
      sync_service_observer_{this};
  base::WeakPtrFactory<HnsSyncWorker> weak_ptr_factory_{this};
};

}  // namespace android
}  // namespace chrome

#endif  // HNS_BROWSER_ANDROID_HNS_SYNC_WORKER_H_
