/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <utility>

#include "base/base64.h"
#include "base/logging.h"
#include "base/test/gtest_util.h"
#include "base/test/task_environment.h"
#include "hns/components/sync/service/hns_sync_service_impl.h"
#include "hns/components/sync/service/sync_service_impl_delegate.h"
#include "hns/components/sync/test/hns_mock_sync_engine.h"
#include "build/build_config.h"
#include "components/os_crypt/sync/os_crypt.h"
#include "components/os_crypt/sync/os_crypt_mocker.h"
#include "components/sync/engine/nigori/key_derivation_params.h"
#include "components/sync/engine/nigori/nigori.h"
#include "components/sync/service/data_type_manager_impl.h"
#include "components/sync/test/data_type_manager_mock.h"
#include "components/sync/test/fake_data_type_controller.h"
#include "components/sync/test/fake_sync_api_component_factory.h"
#include "components/sync/test/fake_sync_engine.h"
#include "components/sync/test/fake_sync_manager.h"
#include "components/sync/test/sync_service_impl_bundle.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

using testing::_;
using testing::ByMove;
using testing::NiceMock;
using testing::Return;

namespace syncer {

namespace {

const char kValidSyncCode[] =
    "fringe digital begin feed equal output proof cheap "
    "exotic ill sure question trial squirrel glove celery "
    "awkward push jelly logic broccoli almost grocery drift";

// Taken from anonimous namespace from sync_service_crypto_unittest.cc
sync_pb::EncryptedData MakeEncryptedData(
    const std::string& passphrase,
    const KeyDerivationParams& derivation_params) {
  std::unique_ptr<Nigori> nigori =
      Nigori::CreateByDerivation(derivation_params, passphrase);

  std::string nigori_name = nigori->GetKeyName();
  const std::string unencrypted = "test";
  sync_pb::EncryptedData encrypted;
  encrypted.set_key_name(nigori_name);
  *encrypted.mutable_blob() = nigori->Encrypt(unencrypted);
  return encrypted;
}

}  // namespace

class SyncServiceImplDelegateMock : public SyncServiceImplDelegate {
 public:
  SyncServiceImplDelegateMock() = default;
  ~SyncServiceImplDelegateMock() override = default;
  void SuspendDeviceObserverForOwnReset() override {}
  void ResumeDeviceObserver() override {}
  void SetLocalDeviceAppearedCallback(
      base::OnceCallback<void()> local_device_appeared_callback) override {}
};

class SyncServiceObserverMock : public SyncServiceObserver {
 public:
  SyncServiceObserverMock() {}
  ~SyncServiceObserverMock() override {}

  MOCK_METHOD(void, OnStateChanged, (SyncService * sync), (override));
  MOCK_METHOD(void, OnSyncCycleCompleted, (SyncService * sync), (override));
  MOCK_METHOD(void,
              OnSyncConfigurationCompleted,
              (SyncService * sync),
              (override));
  MOCK_METHOD(void, OnSyncShutdown, (SyncService * sync), (override));
};

class HnsSyncServiceImplTest : public testing::Test {
 public:
  HnsSyncServiceImplTest()
      : hns_sync_prefs_(sync_service_impl_bundle_.pref_service()),
        sync_prefs_(sync_service_impl_bundle_.pref_service()) {
    sync_service_impl_bundle_.identity_test_env()
        ->SetAutomaticIssueOfAccessTokens(true);
    hns_sync::Prefs::RegisterProfilePrefs(
        sync_service_impl_bundle_.pref_service()->registry());
  }

  ~HnsSyncServiceImplTest() override { sync_service_impl_->Shutdown(); }

  void CreateSyncService(
      ModelTypeSet registered_types = ModelTypeSet({BOOKMARKS})) {
    DataTypeController::TypeVector controllers;
    for (ModelType type : registered_types) {
      controllers.push_back(std::make_unique<FakeDataTypeController>(type));
    }

    std::unique_ptr<SyncClientMock> sync_client =
        sync_service_impl_bundle_.CreateSyncClientMock();
    ON_CALL(*sync_client, CreateDataTypeControllers(_))
        .WillByDefault(Return(ByMove(std::move(controllers))));

    sync_service_impl_ = std::make_unique<HnsSyncServiceImpl>(
        sync_service_impl_bundle_.CreateBasicInitParams(std::move(sync_client)),
        std::make_unique<SyncServiceImplDelegateMock>());
  }

  hns_sync::Prefs* hns_sync_prefs() { return &hns_sync_prefs_; }

  SyncPrefs* sync_prefs() { return &sync_prefs_; }

  PrefService* pref_service() {
    return sync_service_impl_bundle_.pref_service();
  }

  HnsSyncServiceImpl* hns_sync_service_impl() {
    return sync_service_impl_.get();
  }

  FakeSyncApiComponentFactory* component_factory() {
    return sync_service_impl_bundle_.component_factory();
  }

  FakeSyncEngine* engine() {
    return component_factory()->last_created_engine();
  }

 protected:
  content::BrowserTaskEnvironment task_environment_;

 private:
  SyncServiceImplBundle sync_service_impl_bundle_;
  hns_sync::Prefs hns_sync_prefs_;
  SyncPrefs sync_prefs_;
  std::unique_ptr<HnsSyncServiceImpl> sync_service_impl_;
};

TEST_F(HnsSyncServiceImplTest, ValidPassphrase) {
  OSCryptMocker::SetUp();

  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  bool set_code_result = hns_sync_service_impl()->SetSyncCode(kValidSyncCode);
  EXPECT_TRUE(set_code_result);

  bool failed_to_decrypt = false;
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), kValidSyncCode);
  EXPECT_FALSE(failed_to_decrypt);

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, InvalidPassphrase) {
  OSCryptMocker::SetUp();

  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  bool set_code_result =
      hns_sync_service_impl()->SetSyncCode("word one and then two");
  EXPECT_FALSE(set_code_result);

  bool failed_to_decrypt = false;
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), "");
  EXPECT_FALSE(failed_to_decrypt);

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, ValidPassphraseLeadingTrailingWhitespace) {
  OSCryptMocker::SetUp();

  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  std::string sync_code_extra_whitespace =
      std::string(" \t\n") + kValidSyncCode + std::string(" \t\n");
  bool set_code_result =
      hns_sync_service_impl()->SetSyncCode(sync_code_extra_whitespace);
  EXPECT_TRUE(set_code_result);

  bool failed_to_decrypt = false;
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), kValidSyncCode);
  EXPECT_FALSE(failed_to_decrypt);

  OSCryptMocker::TearDown();
}

// Google test doc strongly recommends to use ``*DeathTest` naming
// for test suite
using HnsSyncServiceImplDeathTest = HnsSyncServiceImplTest;

// Some tests are failing for Windows x86 CI,
// See https://github.com/hnsvn/hns-browser/issues/22767
#if BUILDFLAG(IS_WIN) && defined(ARCH_CPU_X86)
#define MAYBE_EmulateGetOrCreateSyncCodeCHECK \
  DISABLED_EmulateGetOrCreateSyncCodeCHECK
#else
#define MAYBE_EmulateGetOrCreateSyncCodeCHECK EmulateGetOrCreateSyncCodeCHECK
#endif
TEST_F(HnsSyncServiceImplDeathTest, MAYBE_EmulateGetOrCreateSyncCodeCHECK) {
  OSCryptMocker::SetUp();

  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  std::string wrong_seed = "123";
  std::string encrypted_wrong_seed;
  EXPECT_TRUE(OSCrypt::EncryptString(wrong_seed, &encrypted_wrong_seed));

  std::string encoded_wrong_seed;
  base::Base64Encode(encrypted_wrong_seed, &encoded_wrong_seed);
  pref_service()->SetString(hns_sync::Prefs::GetSeedPath(),
                            encoded_wrong_seed);

  EXPECT_CHECK_DEATH(hns_sync_service_impl()->GetOrCreateSyncCode());

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, StopAndClearForHnsSeed) {
  OSCryptMocker::SetUp();

  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  bool set_code_result = hns_sync_service_impl()->SetSyncCode(kValidSyncCode);
  EXPECT_TRUE(set_code_result);

  hns_sync_service_impl()->StopAndClear();

  bool failed_to_decrypt = false;
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), "");
  EXPECT_FALSE(failed_to_decrypt);

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, ForcedSetDecryptionPassphrase) {
  OSCryptMocker::SetUp();
  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());
  hns_sync_service_impl()->SetSyncCode(kValidSyncCode);

  // By default Hns enables Bookmarks datatype when sync is enabled.
  // This caused DCHECK at DataTypeManagerImpl::DataTypeManagerImpl
  // after OnEngineInitialized(true, false) call.
  // Current unit test is intended to verify fix for hns/hns-browser#22898
  // and is about set encryption passphrase later setup after right after
  // enabling sync, for example when internet connection is unstable. Related
  // Chromium commit 3241d114b8036bb6d53931ba34b3bf819258c29d Prior to this
  // commit DataTypeManagerImpl wasn't created for bookmarks at
  // ForcedSetDecryptionPassphrase test.
  hns_sync_service_impl()->GetUserSettings()->SetSelectedTypes(
      false, syncer::UserSelectableTypeSet());

  task_environment_.RunUntilIdle();

  hns_sync_service_impl()
      ->GetUserSettings()
      ->SetInitialSyncFeatureSetupComplete(
          syncer::SyncFirstSetupCompleteSource::ADVANCED_FLOW_CONFIRM);

  // Pretend we need the passphrase by triggering OnPassphraseRequired and
  // supplying the encrypted portion of data, as it is done in
  // sync_service_crypto_unittest.cc
  hns_sync_service_impl()->GetCryptoForTests()->OnPassphraseRequired(
      KeyDerivationParams::CreateForPbkdf2(),
      MakeEncryptedData(kValidSyncCode,
                        KeyDerivationParams::CreateForPbkdf2()));

  EXPECT_TRUE(
      hns_sync_service_impl()->GetUserSettings()->IsPassphraseRequired());

  hns_sync_service_impl()->OnEngineInitialized(true, false);
  EXPECT_FALSE(
      hns_sync_service_impl()->GetUserSettings()->IsPassphraseRequired());

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, OnSelfDeviceInfoDeleted) {
  OSCryptMocker::SetUp();
  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  hns_sync_service_impl()->SetSyncCode(kValidSyncCode);
  task_environment_.RunUntilIdle();

  // Replace DataTypeManager with mock who gives CONFIGURING result on state()
  // call. We need this to force SyncService::GetTransportState() give
  // TransportState::CONFIGURING status to test behavior
  // HnsSyncServiceImpl::OnSelfDeviceInfoDeleted
  NiceMock<DataTypeManagerMock> data_type_manager_mock;
  std::unique_ptr<DataTypeManager> data_type_manager_mock_ptr =
      std::unique_ptr<DataTypeManager>(&data_type_manager_mock);

  hns_sync_service_impl()->data_type_manager_ =
      std::move(data_type_manager_mock_ptr);

  ON_CALL(data_type_manager_mock, state())
      .WillByDefault(Return(DataTypeManager::CONFIGURING));

  EXPECT_EQ(hns_sync_service_impl()->GetTransportState(),
            SyncServiceImpl::TransportState::CONFIGURING);

  NiceMock<SyncServiceObserverMock> observer_mock;
  hns_sync_service_impl()->AddObserver(&observer_mock);

  // When OnSelfDeviceInfoDeleted arrived, but transport state is CONFIGURING,
  // we must not stop and clear service.
  EXPECT_CALL(observer_mock, OnStateChanged).Times(0);
  hns_sync_service_impl()->OnSelfDeviceInfoDeleted(base::DoNothing());

  hns_sync_service_impl()->RemoveObserver(&observer_mock);

  // hns_sync_service_impl()->data_type_manager_ is owned by local var
  // |data_type_manager_mock|, so release ownership for the correct destruction
  hns_sync_service_impl()->data_type_manager_.release();

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, PermanentlyDeleteAccount) {
  OSCryptMocker::SetUp();
  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());
  hns_sync_service_impl()->SetSyncCode(kValidSyncCode);
  task_environment_.RunUntilIdle();

  hns_sync_service_impl()
      ->GetUserSettings()
      ->SetInitialSyncFeatureSetupComplete(
          syncer::SyncFirstSetupCompleteSource::ADVANCED_FLOW_CONFIRM);
  EXPECT_TRUE(engine());

  std::unique_ptr<testing::NiceMock<HnsMockSyncEngine>> mock_sync_engine =
      std::make_unique<testing::NiceMock<HnsMockSyncEngine>>();
  EXPECT_CALL(*mock_sync_engine, PermanentlyDeleteAccount).Times(1);
  std::unique_ptr<SyncEngine> engine_orig =
      std::move(hns_sync_service_impl()->engine_);
  hns_sync_service_impl()->engine_ = std::move(mock_sync_engine);
  hns_sync_service_impl()->PermanentlyDeleteAccount(base::BindOnce(
      [](const syncer::SyncProtocolError& sync_protocol_error) {}));
  hns_sync_service_impl()->engine_ = std::move(engine_orig);
  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, OnAccountDeleted_Success) {
  OSCryptMocker::SetUp();
  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  hns_sync_service_impl()->initiated_delete_account_ = true;
  SyncProtocolError sync_protocol_error;
  sync_protocol_error.error_type = SYNC_SUCCESS;

  hns_sync_service_impl()->OnAccountDeleted(
      1, base::BindOnce([](const syncer::SyncProtocolError& spe) {
        EXPECT_EQ(spe.error_type, SYNC_SUCCESS);
      }),
      sync_protocol_error);

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, OnAccountDeleted_FailureAndRetry) {
  OSCryptMocker::SetUp();
  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  hns_sync_service_impl()->initiated_delete_account_ = true;
  SyncProtocolError sync_protocol_error;
  sync_protocol_error.error_type = TRANSIENT_ERROR;

  // Five unsuccessful attempts, the callback must be fired once, for the last
  // one
  bool was_callback_invoked[] = {false, false, false, false, true};

  for (size_t i = 0; i < std::size(was_callback_invoked); ++i) {
    bool on_account_deleted_invoked = false;
    hns_sync_service_impl()->OnAccountDeleted(
        i + 1,
        base::BindOnce(
            [](bool* on_account_deleted_invoked,
               const syncer::SyncProtocolError& spe) {
              *on_account_deleted_invoked = true;
              EXPECT_EQ(spe.error_type, TRANSIENT_ERROR);
            },
            &on_account_deleted_invoked),
        sync_protocol_error);

    EXPECT_EQ(on_account_deleted_invoked, was_callback_invoked[i]);
  }

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, JoinActiveOrNewChain) {
  OSCryptMocker::SetUp();
  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  EXPECT_FALSE(hns_sync_service_impl()->join_chain_result_callback_);

  bool join_chain_callback_invoked = false;

  hns_sync_service_impl()->SetJoinChainResultCallback(base::BindOnce(
      [](bool* join_chain_callback_invoked, bool join_succeeded) {
        *join_chain_callback_invoked = true;
        EXPECT_TRUE(join_succeeded);
      },
      &join_chain_callback_invoked));

  EXPECT_TRUE(hns_sync_service_impl()->join_chain_result_callback_);
  EXPECT_FALSE(join_chain_callback_invoked);
  hns_sync_service_impl()->LocalDeviceAppeared();
  EXPECT_TRUE(join_chain_callback_invoked);

  OSCryptMocker::TearDown();
}

TEST_F(HnsSyncServiceImplTest, JoinDeletedChain) {
  OSCryptMocker::SetUp();
  CreateSyncService();

  hns_sync_service_impl()->Initialize();
  EXPECT_FALSE(engine());

  EXPECT_FALSE(hns_sync_service_impl()->join_chain_result_callback_);

  bool join_chain_callback_invoked = false;

  hns_sync_service_impl()->SetJoinChainResultCallback(base::BindOnce(
      [](bool* join_chain_callback_invoked, bool join_succeeded) {
        *join_chain_callback_invoked = true;
        EXPECT_FALSE(join_succeeded);
      },
      &join_chain_callback_invoked));

  EXPECT_TRUE(hns_sync_service_impl()->join_chain_result_callback_);
  EXPECT_FALSE(join_chain_callback_invoked);
  EXPECT_FALSE(hns_sync_service_impl()->initiated_self_device_info_deleted_);

  EXPECT_FALSE(hns_sync_service_impl()->initiated_join_chain_);
  hns_sync_service_impl()->SetSyncCode(kValidSyncCode);
  EXPECT_TRUE(hns_sync_service_impl()->initiated_join_chain_);

  // Normally sync_disabled_by_admin_ is set at
  // SyncServiceImpl::OnActionableError, but we can't invoke it, so set it
  // directly for test
  hns_sync_service_impl()->sync_disabled_by_admin_ = true;
  hns_sync_service_impl()->ResetEngine(
      ShutdownReason::DISABLE_SYNC_AND_CLEAR_DATA,
      SyncServiceImpl::ResetEngineReason::kDisabledAccount);

  EXPECT_TRUE(join_chain_callback_invoked);

  OSCryptMocker::TearDown();
}

}  // namespace syncer
