/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_SERVICE_IMPL_H_
#define HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_SERVICE_IMPL_H_

#include <memory>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/sync/protocol/sync_protocol_error.h"
#include "components/sync/service/sync_service_impl.h"

namespace syncer {

class HnsSyncAuthManager;
class SyncServiceImplDelegate;
class SyncServiceCrypto;
struct SyncProtocolError;
struct TypeEntitiesCount;

class HnsSyncServiceImpl : public SyncServiceImpl {
 public:
  explicit HnsSyncServiceImpl(
      InitParams init_params,
      std::unique_ptr<SyncServiceImplDelegate> sync_service_impl_delegate);
  ~HnsSyncServiceImpl() override;

  // SyncServiceImpl implementation
  bool IsSetupInProgress() const override;
  void StopAndClear() override;

  // SyncEngineHost override.
  void OnEngineInitialized(bool success,
                           bool is_first_time_sync_configure) override;
  void OnSyncCycleCompleted(const SyncCycleSnapshot& snapshot) override;

  // SyncPrefObserver implementation.
  void OnPreferredDataTypesPrefChange(
      bool payments_integration_enabled_changed) override;

  std::string GetOrCreateSyncCode();
  bool SetSyncCode(const std::string& sync_code);

  // This should only be called by helper function, hns_sync::ResetSync, or by
  // OnDeviceInfoChange internally
  void OnSelfDeviceInfoDeleted(base::OnceClosure cb);

  // These functions are for disabling device_info_observer_ from firing
  // when the device is doing own reset sync operation, to prevent early call
  // of StopAndClear prior to device sends delete record
  void SuspendDeviceObserverForOwnReset();
  void ResumeDeviceObserver();

  void Initialize() override;

  const hns_sync::Prefs& prefs() { return hns_sync_prefs_; }

  void PermanentlyDeleteAccount(
      base::OnceCallback<void(const SyncProtocolError&)> callback);

  void SetJoinChainResultCallback(base::OnceCallback<void(bool)> callback);

 private:
  friend class HnsSyncServiceImplTest;
  FRIEND_TEST_ALL_PREFIXES(HnsSyncServiceImplTest,
                           ForcedSetDecryptionPassphrase);
  FRIEND_TEST_ALL_PREFIXES(HnsSyncServiceImplTest, OnAccountDeleted_Success);
  FRIEND_TEST_ALL_PREFIXES(HnsSyncServiceImplTest,
                           OnAccountDeleted_FailureAndRetry);
  FRIEND_TEST_ALL_PREFIXES(HnsSyncServiceImplTest, JoinActiveOrNewChain);
  FRIEND_TEST_ALL_PREFIXES(HnsSyncServiceImplTest, JoinDeletedChain);

  HnsSyncAuthManager* GetHnsSyncAuthManager();
  SyncServiceCrypto* GetCryptoForTests();

  void OnHnsSyncPrefsChanged(const std::string& path);

  void PermanentlyDeleteAccountImpl(
      const int current_attempt,
      base::OnceCallback<void(const SyncProtocolError&)> callback);

  void OnAccountDeleted(
      const int current_attempt,
      base::OnceCallback<void(const SyncProtocolError&)> callback,
      const SyncProtocolError&);

  void ResetEngine(ShutdownReason shutdown_reason,
                   ResetEngineReason reset_reason) override;

  void LocalDeviceAppeared();

  void UpdateP3AObjectsNumber();
  void OnGotEntityCounts(
      const std::vector<syncer::TypeEntitiesCount>& entity_counts);

  hns_sync::Prefs hns_sync_prefs_;

  PrefChangeRegistrar hns_sync_prefs_change_registrar_;

  // This is set to true between |PermanentlyDeleteAccount| succeeded call and
  // new sync chain setup or browser exit. This is used to avoid show the
  // infobar to ourselves, because we know what we have done
  bool initiated_delete_account_ = false;

  // This flag is used to detect the case when we are trying to connect
  // deleted sync chain. It is true between SetSyncCode and LocalDeviceAppeared.
  bool initiated_join_chain_ = false;

  // This flag is used to separate cases of normal leave the chain procedure and
  // delete account case. When it's a normal leave procedure, we must not call
  // HnsSyncServiceImpl::StopAndClear from HnsSyncServiceImpl::ResetEngine
  bool initiated_self_device_info_deleted_ = false;

  int completed_cycles_count_ = 0;

  std::unique_ptr<SyncServiceImplDelegate> sync_service_impl_delegate_;
  base::OnceCallback<void(bool)> join_chain_result_callback_;
  base::WeakPtrFactory<HnsSyncServiceImpl> weak_ptr_factory_;

  HnsSyncServiceImpl(const HnsSyncServiceImpl&) = delete;
  HnsSyncServiceImpl& operator=(const HnsSyncServiceImpl&) = delete;
};

}  // namespace syncer

#endif  // HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_SERVICE_IMPL_H_
