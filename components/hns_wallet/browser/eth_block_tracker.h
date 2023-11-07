/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_BLOCK_TRACKER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_BLOCK_TRACKER_H_

#include <map>
#include <string>

#include "base/functional/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/observer_list_types.h"
#include "hns/components/hns_wallet/browser/block_tracker.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/hns_wallet_types.h"

namespace hns_wallet {

class JsonRpcService;

class EthBlockTracker : public BlockTracker {
 public:
  explicit EthBlockTracker(JsonRpcService* json_rpc_service);
  ~EthBlockTracker() override;
  EthBlockTracker(const EthBlockTracker&) = delete;
  EthBlockTracker operator=(const EthBlockTracker&) = delete;

  class Observer : public base::CheckedObserver {
   public:
    // Fires for each latest block check
    virtual void OnLatestBlock(const std::string& chain_id,
                               uint256_t block_num) = 0;
    // Only fires when there is a new block
    virtual void OnNewBlock(const std::string& chain_id,
                            uint256_t block_num) = 0;
  };

  // If timer is already running, it will be replaced with new interval
  void Start(const std::string& chain_id, base::TimeDelta interval) override;

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  uint256_t GetCurrentBlock(const std::string& chain_id) const;

  void CheckForLatestBlock(
      const std::string& chain_id,
      base::OnceCallback<void(uint256_t block_num,
                              mojom::ProviderError error,
                              const std::string& error_message)>);

 private:
  void SendGetBlockNumber(
      const std::string& chain_id,
      base::OnceCallback<void(uint256_t block_num,
                              mojom::ProviderError error,
                              const std::string& error_message)>);
  void GetBlockNumber(const std::string& chain_id);
  void OnGetBlockNumber(const std::string& chain_id,
                        uint256_t block_num,
                        mojom::ProviderError error,
                        const std::string& error_message);

  // <chain_id, block_number>
  std::map<std::string, uint256_t> current_block_map_;
  base::ObserverList<Observer> observers_;

  base::WeakPtrFactory<EthBlockTracker> weak_factory_;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_BLOCK_TRACKER_H_
