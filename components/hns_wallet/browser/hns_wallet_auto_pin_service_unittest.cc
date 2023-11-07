// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/hns_wallet/browser/hns_wallet_auto_pin_service.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/test/bind.h"
#include "base/time/time_override.h"
#include "hns/components/hns_wallet/browser/hns_wallet_pin_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_prefs.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/ipfs/pin/ipfs_local_pin_service.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/scoped_user_pref_update.h"
#include "components/prefs/testing_pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using testing::_;

namespace hns_wallet {

namespace {

mojom::BlockchainTokenPtr GetErc721Token(const std::string& pref_path) {
  mojom::BlockchainTokenPtr token =
      HnsWalletPinService::TokenFromPrefPath(pref_path);
  token->is_erc721 = true;
  return token;
}

class MockHnsWalletPinService : public HnsWalletPinService {
 public:
  MockHnsWalletPinService() : HnsWalletPinService() {}
  MOCK_METHOD0(Restore, void());
  MOCK_METHOD3(AddPin,
               void(mojom::BlockchainTokenPtr,
                    const absl::optional<std::string>&,
                    HnsWalletPinService::AddPinCallback callback));
  MOCK_METHOD3(RemovePin,
               void(mojom::BlockchainTokenPtr,
                    const absl::optional<std::string>&,
                    HnsWalletPinService::RemovePinCallback callback));
  MOCK_METHOD3(Validate,
               void(mojom::BlockchainTokenPtr,
                    const absl::optional<std::string>&,
                    HnsWalletPinService::ValidateCallback));
  MOCK_METHOD1(GetTokens,
               std::set<std::string>(const absl::optional<std::string>&));
  MOCK_METHOD2(GetTokenStatus,
               mojom::TokenPinStatusPtr(const absl::optional<std::string>&,
                                        const mojom::BlockchainTokenPtr&));
  MOCK_METHOD2(GetLastValidateTime,
               absl::optional<base::Time>(const absl::optional<std::string>&,
                                          const mojom::BlockchainTokenPtr&));
  MOCK_METHOD2(MarkAsPendingForPinning,
               void(const mojom::BlockchainTokenPtr&,
                    const absl::optional<std::string>&));
  MOCK_METHOD2(MarkAsPendingForUnpinning,
               void(const mojom::BlockchainTokenPtr&,
                    const absl::optional<std::string>&));
  MOCK_METHOD1(Reset, void(base::OnceCallback<void(bool)> callback));
};

class MockHnsWalletService : public HnsWalletService {
 public:
  MOCK_METHOD1(GetAllUserAssets,
               void(HnsWalletService::GetUserAssetsCallback));
};

MATCHER_P(TokenPathMatches, path, "") {
  auto token = GetErc721Token(path);
  return token->coin == arg->coin && token->chain_id == arg->chain_id &&
         token->contract_address == arg->contract_address &&
         token->token_id == arg->token_id;
}

}  // namespace

class HnsWalletAutoPinServiceTest : public testing::Test {
 public:
  HnsWalletAutoPinServiceTest() = default;

  HnsWalletAutoPinService* service() {
    return hns_wallet_auto_pin_service_.get();
  }

 protected:
  void SetUp() override {
    ON_CALL(*GetHnsWalletPinService(), Restore())
        .WillByDefault(::testing::Invoke([]() {}));
    auto* registry = pref_service_.registry();
    registry->RegisterBooleanPref(kAutoPinEnabled, true);
    hns_wallet_auto_pin_service_ =
        std::make_unique<HnsWalletAutoPinService>(
            GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
  }

  PrefService* GetPrefs() { return &pref_service_; }

  testing::NiceMock<MockHnsWalletPinService>* GetHnsWalletPinService() {
    return &hns_wallet_pin_service_;
  }

  testing::NiceMock<MockHnsWalletService>* GetHnsWalletService() {
    return &hns_wallet_service_;
  }

  void SetAutoPinEnabled(bool value) {}

  TestingPrefServiceSimple pref_service_;

  testing::NiceMock<MockHnsWalletPinService> hns_wallet_pin_service_;
  testing::NiceMock<MockHnsWalletService> hns_wallet_service_;

  std::unique_ptr<HnsWalletAutoPinService> hns_wallet_auto_pin_service_;

  content::BrowserTaskEnvironment task_environment_;
};

TEST_F(HnsWalletAutoPinServiceTest, Autopin_WhenTokenAdded) {
  service()->SetAutoPinEnabled(true);

  ON_CALL(*GetHnsWalletPinService(), AddPin(_, _, _))
      .WillByDefault(
          ::testing::Invoke([](BlockchainTokenPtr token,
                               const absl::optional<std::string>& service,
                               HnsWalletPinService::AddPinCallback callback) {
            std::move(callback).Run(true, nullptr);
          }));
  EXPECT_CALL(*GetHnsWalletPinService(), AddPin(_, _, _)).Times(3);

  {
    mojom::BlockchainTokenPtr token = GetErc721Token(
        "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1");
    token->is_erc721 = true;
    service()->OnTokenAdded(std::move(token));
  }

  {
    mojom::BlockchainTokenPtr token = GetErc721Token(
        "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2");
    token->is_erc721 = true;
    service()->OnTokenAdded(std::move(token));
  }

  {
    mojom::BlockchainTokenPtr token = GetErc721Token(
        "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3");
    token->is_erc721 = true;
    service()->OnTokenAdded(std::move(token));
  }
}

TEST_F(HnsWalletAutoPinServiceTest, TokenRemoved) {
  ON_CALL(*GetHnsWalletPinService(), AddPin(_, _, _))
      .WillByDefault(
          ::testing::Invoke([](BlockchainTokenPtr token,
                               const absl::optional<std::string>& service,
                               HnsWalletPinService::AddPinCallback callback) {
            std::move(callback).Run(true, nullptr);
          }));
}

TEST_F(HnsWalletAutoPinServiceTest, UnpinUnknownTokens_WhenRestore) {
  std::set<std::string> known_tokens;

  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3");

  ON_CALL(*GetHnsWalletPinService(),
          GetTokenStatus(testing::Eq(absl::nullopt), _))
      .WillByDefault(
          ::testing::Invoke([](absl::optional<std::string> service,
                               const mojom::BlockchainTokenPtr& token)
                                -> mojom::TokenPinStatusPtr {
            mojom::TokenPinStatusPtr status = mojom::TokenPinStatus::New();
            status->code = mojom::TokenPinStatusCode::STATUS_PINNED;
            status->validate_time = base::Time::Now();
            return status;
          }));
  ON_CALL(*GetHnsWalletPinService(), GetTokens(_))
      .WillByDefault(::testing::Return(known_tokens));
  ON_CALL(*GetHnsWalletService(), GetAllUserAssets(_))
      .WillByDefault(::testing::Invoke([](HnsWalletService::
                                              GetUserAssetsCallback callback) {
        std::vector<mojom::BlockchainTokenPtr> result;
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"));
        std::move(callback).Run(std::move(result));
      }));

  EXPECT_CALL(*GetHnsWalletPinService(),
              RemovePin(TokenPathMatches(
                            "nft.local.60.0x1."
                            "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"),
                        testing::Eq(absl::nullopt), _))
      .Times(1);

  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
}

TEST_F(HnsWalletAutoPinServiceTest, ValidateOldTokens_WhenRestore) {
  std::set<std::string> known_tokens;

  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4");
  ON_CALL(*GetHnsWalletPinService(),
          GetTokenStatus(testing::Eq(absl::nullopt), _))
      .WillByDefault(
          ::testing::Invoke([](absl::optional<std::string> service,
                               const mojom::BlockchainTokenPtr& token)
                                -> mojom::TokenPinStatusPtr {
            mojom::TokenPinStatusPtr status = mojom::TokenPinStatus::New();

            if ("0x1" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_PINNED;
              status->validate_time = base::Time::Now() - base::Days(20);
            } else if ("0x2" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_PINNED;
            } else if ("0x3" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_PINNED;
              status->validate_time = base::Time::Now() + base::Days(20);
            } else if ("0x4" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_NOT_PINNED;
            }
            return status;
          }));
  ON_CALL(*GetHnsWalletPinService(), GetTokens(_))
      .WillByDefault(::testing::Return(known_tokens));
  ON_CALL(*GetHnsWalletService(), GetAllUserAssets(_))
      .WillByDefault(::testing::Invoke([](HnsWalletService::
                                              GetUserAssetsCallback callback) {
        std::vector<mojom::BlockchainTokenPtr> result;
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"));
        std::move(callback).Run(std::move(result));
      }));

  ON_CALL(*GetHnsWalletPinService(), Validate(_, _, _))
      .WillByDefault(::testing::Invoke(
          [](BlockchainTokenPtr token,
             const absl::optional<std::string>& service,
             HnsWalletPinService::ValidateCallback callback) {
            std::move(callback).Run(
                mojom::TokenValidationResult::kValidationPassed);
          }));

  EXPECT_CALL(*GetHnsWalletPinService(),
              Validate(TokenPathMatches(
                           "nft.local.60.0x1."
                           "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"),
                       testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(*GetHnsWalletPinService(),
              Validate(TokenPathMatches(
                           "nft.local.60.0x1."
                           "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"),
                       testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(*GetHnsWalletPinService(),
              Validate(TokenPathMatches(
                           "nft.local.60.0x1."
                           "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"),
                       testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(*GetHnsWalletPinService(),
              Validate(TokenPathMatches(
                           "nft.local.60.0x1."
                           "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"),
                       testing::Eq(absl::nullopt), _))
      .Times(0);

  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
}

TEST_F(HnsWalletAutoPinServiceTest, PinContinue_WhenRestore) {
  std::set<std::string> known_tokens;

  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3");

  ON_CALL(*GetHnsWalletPinService(),
          GetTokenStatus(testing::Eq(absl::nullopt), _))
      .WillByDefault(
          ::testing::Invoke([](absl::optional<std::string> service,
                               const mojom::BlockchainTokenPtr& token)
                                -> mojom::TokenPinStatusPtr {
            mojom::TokenPinStatusPtr status = mojom::TokenPinStatus::New();
            if ("0x1" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_PINNING_FAILED;
            } else if ("0x2" == token->token_id) {
              status->code =
                  mojom::TokenPinStatusCode::STATUS_PINNING_IN_PROGRESS;
            } else if ("0x3" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_PINNING_PENDING;
            } else if ("0x4" == token->token_id) {
              status->code =
                  mojom::TokenPinStatusCode::STATUS_UNPINNING_PENDING;
            } else if ("0x5" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_UNPINNING_FAILED;
            }
            return status;
          }));
  ON_CALL(*GetHnsWalletPinService(), GetTokens(_))
      .WillByDefault(::testing::Return(known_tokens));
  ON_CALL(*GetHnsWalletService(), GetAllUserAssets(_))
      .WillByDefault(::testing::Invoke([](HnsWalletService::
                                              GetUserAssetsCallback callback) {
        std::vector<mojom::BlockchainTokenPtr> result;
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x5"));
        std::move(callback).Run(std::move(result));
      }));

  ON_CALL(*GetHnsWalletPinService(), AddPin(_, _, _))
      .WillByDefault(
          ::testing::Invoke([](BlockchainTokenPtr token,
                               const absl::optional<std::string>& service,
                               HnsWalletPinService::AddPinCallback callback) {
            std::move(callback).Run(true, nullptr);
          }));

  EXPECT_CALL(
      *GetHnsWalletPinService(),
      AddPin(TokenPathMatches("nft.local.60.0x1."
                              "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"),
             testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(
      *GetHnsWalletPinService(),
      AddPin(TokenPathMatches("nft.local.60.0x1."
                              "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"),
             testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(
      *GetHnsWalletPinService(),
      AddPin(TokenPathMatches("nft.local.60.0x1."
                              "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"),
             testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(
      *GetHnsWalletPinService(),
      AddPin(TokenPathMatches("nft.local.60.0x1."
                              "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"),
             testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(
      *GetHnsWalletPinService(),
      AddPin(TokenPathMatches("nft.local.60.0x1."
                              "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x5"),
             testing::Eq(absl::nullopt), _))
      .Times(1);

  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
}

TEST_F(HnsWalletAutoPinServiceTest, UnpinContinue_WhenRestore) {
  std::set<std::string> known_tokens;

  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4");

  ON_CALL(*GetHnsWalletPinService(),
          GetTokenStatus(testing::Eq(absl::nullopt), _))
      .WillByDefault(
          ::testing::Invoke([](absl::optional<std::string> service,
                               const mojom::BlockchainTokenPtr& token)
                                -> mojom::TokenPinStatusPtr {
            mojom::TokenPinStatusPtr status = mojom::TokenPinStatus::New();
            if ("0x1" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_UNPINNING_FAILED;
            } else if ("0x2" == token->token_id) {
              status->code =
                  mojom::TokenPinStatusCode::STATUS_UNPINNING_IN_PROGRESS;
            } else if ("0x3" == token->token_id) {
              status->code =
                  mojom::TokenPinStatusCode::STATUS_UNPINNING_PENDING;
            } else if ("0x4" == token->token_id) {
              status->code =
                  mojom::TokenPinStatusCode::STATUS_UNPINNING_PENDING;
            }
            return status;
          }));
  ON_CALL(*GetHnsWalletPinService(), GetTokens(_))
      .WillByDefault(::testing::Return(known_tokens));
  ON_CALL(*GetHnsWalletService(), GetAllUserAssets(_))
      .WillByDefault(::testing::Invoke([](HnsWalletService::
                                              GetUserAssetsCallback callback) {
        std::vector<mojom::BlockchainTokenPtr> result;
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"));
        std::move(callback).Run(std::move(result));
      }));

  ON_CALL(*GetHnsWalletPinService(), RemovePin(_, _, _))
      .WillByDefault(::testing::Invoke(
          [](BlockchainTokenPtr token,
             const absl::optional<std::string>& service,
             HnsWalletPinService::RemovePinCallback callback) {
            std::move(callback).Run(true, nullptr);
          }));

  ON_CALL(*GetHnsWalletPinService(), AddPin(_, _, _))
      .WillByDefault(::testing::Invoke(
          [](BlockchainTokenPtr token,
             const absl::optional<std::string>& service,
             HnsWalletPinService::RemovePinCallback callback) {
            std::move(callback).Run(true, nullptr);
          }));

  EXPECT_CALL(*GetHnsWalletPinService(),
              RemovePin(TokenPathMatches(
                            "nft.local.60.0x1."
                            "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"),
                        testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(*GetHnsWalletPinService(),
              RemovePin(TokenPathMatches(
                            "nft.local.60.0x1."
                            "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"),
                        testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(*GetHnsWalletPinService(),
              RemovePin(TokenPathMatches(
                            "nft.local.60.0x1."
                            "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"),
                        testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(*GetHnsWalletPinService(),
              RemovePin(TokenPathMatches(
                            "nft.local.60.0x1."
                            "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"),
                        testing::Eq(absl::nullopt), _))
      .Times(0);

  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
}

TEST_F(HnsWalletAutoPinServiceTest, DoNotAutoPin_WhenAutoPinDisabled) {
  service()->SetAutoPinEnabled(false);

  ON_CALL(*GetHnsWalletPinService(), AddPin(_, _, _))
      .WillByDefault(
          ::testing::Invoke([](BlockchainTokenPtr token,
                               const absl::optional<std::string>& service,
                               HnsWalletPinService::AddPinCallback callback) {
            std::move(callback).Run(true, nullptr);
          }));
  EXPECT_CALL(*GetHnsWalletPinService(), AddPin(_, _, _)).Times(0);

  {
    mojom::BlockchainTokenPtr token = GetErc721Token(
        "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1");
    service()->OnTokenAdded(std::move(token));
  }
}

TEST_F(HnsWalletAutoPinServiceTest, PinOldTokens_WhenAutoPinEnabled) {
  std::set<std::string> known_tokens;

  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2");

  ON_CALL(*GetHnsWalletPinService(),
          GetTokenStatus(testing::Eq(absl::nullopt), _))
      .WillByDefault(
          ::testing::Invoke([](absl::optional<std::string> service,
                               const mojom::BlockchainTokenPtr& token)
                                -> mojom::TokenPinStatusPtr {
            mojom::TokenPinStatusPtr status = mojom::TokenPinStatus::New();
            if ("0x1" == token->token_id) {
              return nullptr;
            } else if ("0x2" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_NOT_PINNED;
            }
            return status;
          }));
  ON_CALL(*GetHnsWalletPinService(), GetTokens(_))
      .WillByDefault(::testing::Return(known_tokens));
  ON_CALL(*GetHnsWalletService(), GetAllUserAssets(_))
      .WillByDefault(::testing::Invoke([](HnsWalletService::
                                              GetUserAssetsCallback callback) {
        std::vector<mojom::BlockchainTokenPtr> result;
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"));
        std::move(callback).Run(std::move(result));
      }));

  ON_CALL(*GetHnsWalletPinService(), AddPin(_, _, _))
      .WillByDefault(
          ::testing::Invoke([](BlockchainTokenPtr token,
                               const absl::optional<std::string>& service,
                               HnsWalletPinService::AddPinCallback callback) {
            std::move(callback).Run(true, nullptr);
          }));

  EXPECT_CALL(
      *GetHnsWalletPinService(),
      AddPin(TokenPathMatches("nft.local.60.0x1."
                              "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"),
             testing::Eq(absl::nullopt), _))
      .Times(1);
  EXPECT_CALL(
      *GetHnsWalletPinService(),
      AddPin(TokenPathMatches("nft.local.60.0x1."
                              "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"),
             testing::Eq(absl::nullopt), _))
      .Times(1);

  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
}

TEST_F(HnsWalletAutoPinServiceTest, QueueCleared_WhenAutoPinDisabled) {
  ON_CALL(*GetHnsWalletService(), GetAllUserAssets(_))
      .WillByDefault(::testing::Invoke([](HnsWalletService::
                                              GetUserAssetsCallback callback) {
        std::vector<mojom::BlockchainTokenPtr> result;
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"));
        std::move(callback).Run(std::move(result));
      }));
  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
  // First one is in progress
  EXPECT_EQ(auto_pin_service.queue_.size(), 2u);

  auto_pin_service.SetAutoPinEnabled(false);
  EXPECT_EQ(auto_pin_service.queue_.size(), 0u);
  EXPECT_EQ(auto_pin_service.tokens_.size(), 0u);
}

TEST_F(HnsWalletAutoPinServiceTest, RemoveQueuedTokens) {
  ON_CALL(*GetHnsWalletService(), GetAllUserAssets(_))
      .WillByDefault(::testing::Invoke([](HnsWalletService::
                                              GetUserAssetsCallback callback) {
        std::vector<mojom::BlockchainTokenPtr> result;
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"));
        std::move(callback).Run(std::move(result));
      }));

  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
  // First one is in progress
  EXPECT_EQ(auto_pin_service.queue_.size(), 2u);

  auto_pin_service.OnTokenRemoved(GetErc721Token(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"));

  EXPECT_EQ(auto_pin_service.queue_.size(), 3u);
  EXPECT_TRUE(auto_pin_service.queue_[2u]->Equals(
      std::make_unique<HnsWalletAutoPinService::IntentData>(
          GetErc721Token("nft.local.60.0x1."
                         "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"),
          HnsWalletAutoPinService::Operation::kDelete, absl::nullopt)));
  auto_pin_service.OnTokenRemoved(GetErc721Token(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"));

  EXPECT_EQ(auto_pin_service.queue_.size(), 3u);

  EXPECT_TRUE(auto_pin_service.queue_[2u]->Equals(
      std::make_unique<HnsWalletAutoPinService::IntentData>(
          GetErc721Token("nft.local.60.0x1."
                         "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"),
          HnsWalletAutoPinService::Operation::kDelete, absl::nullopt)));
}

TEST_F(HnsWalletAutoPinServiceTest, AddQueuedTokens) {
  std::set<std::string> known_tokens;

  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3");
  known_tokens.insert(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4");

  ON_CALL(*GetHnsWalletPinService(),
          GetTokenStatus(testing::Eq(absl::nullopt), _))
      .WillByDefault(
          ::testing::Invoke([](absl::optional<std::string> service,
                               const mojom::BlockchainTokenPtr& token)
                                -> mojom::TokenPinStatusPtr {
            mojom::TokenPinStatusPtr status = mojom::TokenPinStatus::New();
            if ("0x1" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_NOT_PINNED;
            } else if ("0x2" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_PINNED;
              status->validate_time = base::Time::Now();
            } else if ("0x3" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_PINNED;
              status->validate_time = base::Time::Now();
            } else if ("0x4" == token->token_id) {
              status->code = mojom::TokenPinStatusCode::STATUS_PINNED;
            }
            return status;
          }));
  ON_CALL(*GetHnsWalletPinService(), GetTokens(_))
      .WillByDefault(::testing::Return(known_tokens));

  ON_CALL(*GetHnsWalletService(), GetAllUserAssets(_))
      .WillByDefault(::testing::Invoke([](HnsWalletService::
                                              GetUserAssetsCallback callback) {
        std::vector<mojom::BlockchainTokenPtr> result;
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x1"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x2"));
        result.push_back(GetErc721Token(
            "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x3"));
        std::move(callback).Run(std::move(result));
      }));

  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());

  // First one is in progress
  EXPECT_EQ(auto_pin_service.queue_.size(), 1u);
  EXPECT_TRUE(auto_pin_service.queue_[0]->Equals(
      std::make_unique<HnsWalletAutoPinService::IntentData>(
          GetErc721Token("nft.local.60.0x1."
                         "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"),
          HnsWalletAutoPinService::Operation::kDelete, absl::nullopt)));

  auto_pin_service.OnTokenAdded(GetErc721Token(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"));
  EXPECT_EQ(auto_pin_service.queue_.size(), 1u);

  auto_pin_service.queue_[0u]->Equals(std::make_unique<
                                      HnsWalletAutoPinService::IntentData>(
      GetErc721Token(
          "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x4"),
      HnsWalletAutoPinService::Operation::kAdd, absl::nullopt));

  auto_pin_service.OnTokenAdded(GetErc721Token(
      "nft.local.60.0x1.0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x5"));

  EXPECT_EQ(auto_pin_service.queue_.size(), 2u);

  EXPECT_TRUE(auto_pin_service.queue_[1u]->Equals(
      std::make_unique<HnsWalletAutoPinService::IntentData>(
          GetErc721Token("nft.local.60.0x1."
                         "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d.0x5"),
          HnsWalletAutoPinService::Operation::kAdd, absl::nullopt)));
}

TEST_F(HnsWalletAutoPinServiceTest, RestoreNotCalled_WhenAutoPinDisabled) {
  service()->SetAutoPinEnabled(false);
  HnsWalletAutoPinService auto_pin_service(
      GetPrefs(), GetHnsWalletService(), GetHnsWalletPinService());
  EXPECT_CALL(*GetHnsWalletPinService(), Restore()).Times(0);
}

TEST_F(HnsWalletAutoPinServiceTest, Reset) {
  service()->SetAutoPinEnabled(true);
  ON_CALL(*GetHnsWalletPinService(), Reset(_))
      .WillByDefault(
          ::testing::Invoke([](base::OnceCallback<void(bool)> callback) {
            std::move(callback).Run(true);
          }));
  service()->Reset();
  EXPECT_FALSE(service()->IsAutoPinEnabled());
}

}  // namespace hns_wallet
