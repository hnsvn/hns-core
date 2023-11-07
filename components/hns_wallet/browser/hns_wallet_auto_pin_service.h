// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_AUTO_PIN_SERVICE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_AUTO_PIN_SERVICE_H_

#include <deque>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_refptr.h"
#include "base/task/sequenced_task_runner.h"
#include "hns/components/hns_wallet/browser/blockchain_registry.h"
#include "hns/components/hns_wallet/browser/hns_wallet_pin_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service_observer_base.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "components/prefs/pref_service.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote_set.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

using hns_wallet::mojom::BlockchainTokenPtr;

namespace hns_wallet {

class HnsWalletAutoPinService
    : public KeyedService,
      public hns_wallet::mojom::WalletAutoPinService,
      public hns_wallet::mojom::HnsWalletServiceTokenObserver,
      public hns_wallet::HnsWalletServiceObserverBase {
 public:
  HnsWalletAutoPinService(PrefService* prefs,
                            HnsWalletService* hns_wallet_service,
                            HnsWalletPinService* hns_wallet_pin_service);
  ~HnsWalletAutoPinService() override;

  void Reset();

  mojo::PendingRemote<mojom::WalletAutoPinService> MakeRemote();
  void Bind(mojo::PendingReceiver<mojom::WalletAutoPinService> receiver);

  void SetAutoPinEnabled(bool enabled) override;
  void IsAutoPinEnabled(IsAutoPinEnabledCallback callback) override;
  void AddObserver(::mojo::PendingRemote<mojom::WalletAutoPinServiceObserver>
                       observer) override;

  // HnsWalletServiceTokenObserver
  void OnTokenAdded(mojom::BlockchainTokenPtr token) override;
  void OnTokenRemoved(mojom::BlockchainTokenPtr token) override;

  // HnsWalletServiceObserverBase
  void OnResetWallet() override;

  bool IsAutoPinEnabled();

 private:
  FRIEND_TEST_ALL_PREFIXES(HnsWalletAutoPinServiceTest,
                           QueueCleared_WhenAutoPinDisabled);
  FRIEND_TEST_ALL_PREFIXES(HnsWalletAutoPinServiceTest, RemoveQueuedTokens);
  FRIEND_TEST_ALL_PREFIXES(HnsWalletAutoPinServiceTest, AddQueuedTokens);
  enum Operation { kAdd = 0, kDelete = 1, kValidate = 2 };

  struct IntentData {
    BlockchainTokenPtr token;
    Operation operation;
    absl::optional<std::string> service;
    size_t attempt = 0;

    IntentData(const BlockchainTokenPtr& token,
               Operation operation,
               absl::optional<std::string> service);
    ~IntentData();

    bool Equals(
        const std::unique_ptr<HnsWalletAutoPinService::IntentData>& other);
  };

  void OnAutoPinStatusChanged();

  void ResetLocalState();
  // Iterates through user tokens and manages their pin statuses.
  void Restore();
  void OnTokenListResolved(std::vector<BlockchainTokenPtr>);

  void CheckQueue();
  void AddOrExecute(std::unique_ptr<IntentData> data);
  void PostRetry(std::unique_ptr<IntentData> data);

  std::vector<absl::optional<std::string>> GetServicesToPin();
  std::vector<absl::optional<std::string>> GetKnownServices();

  void ValidateToken(const std::unique_ptr<IntentData>& data);
  void PinToken(const std::unique_ptr<IntentData>& data);
  void UnpinToken(const std::unique_ptr<IntentData>& data);

  void OnTaskFinished(bool result, mojom::PinErrorPtr error);
  void OnValidateTaskFinished(mojom::TokenValidationResult result);

  mojo::Receiver<hns_wallet::mojom::HnsWalletServiceTokenObserver>
      token_observer_{this};
  mojo::Receiver<hns_wallet::mojom::HnsWalletServiceObserver>
      hns_wallet_service_observer_{this};
  mojo::ReceiverSet<hns_wallet::mojom::WalletAutoPinService> receivers_;

  raw_ptr<PrefService> pref_service_;
  raw_ptr<HnsWalletService> hns_wallet_service_;
  raw_ptr<HnsWalletPinService> hns_wallet_pin_service_;

  // List of all known tokens, GetTokenPrefPath representation is used
  std::set<std::string> tokens_;
  std::unique_ptr<IntentData> current_;
  std::deque<std::unique_ptr<IntentData>> queue_;

  std::unique_ptr<PrefChangeRegistrar> pref_change_registrar_;
  mojo::RemoteSet<mojom::WalletAutoPinServiceObserver> observers_;

  base::WeakPtrFactory<HnsWalletAutoPinService> weak_ptr_factory_{this};
  base::WeakPtrFactory<HnsWalletAutoPinService> tasks_weak_ptr_factory_{this};
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_AUTO_PIN_SERVICE_H_
