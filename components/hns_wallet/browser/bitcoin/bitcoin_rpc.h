/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_RPC_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_RPC_H_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/types/expected.h"
#include "hns/components/api_request_helper/api_request_helper.h"
#include "hns/components/hns_wallet/browser/bitcoin/bitcoin_transaction.h"
#include "components/prefs/pref_service.h"

namespace hns_wallet {

// TODO(apaymyshev): test this class
// TODO(apaymyshev): consider this being a separate keyed service
class BitcoinRpc {
 public:
  explicit BitcoinRpc(
      PrefService* prefs,
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);
  ~BitcoinRpc();

  using APIRequestHelper = api_request_helper::APIRequestHelper;
  using APIRequestResult = api_request_helper::APIRequestResult;
  using RequestIntermediateCallback =
      base::OnceCallback<void(APIRequestResult api_request_result)>;

  using GetChainHeightCallback =
      base::OnceCallback<void(base::expected<uint32_t, std::string>)>;
  using GetAddressHistoryCallback = base::OnceCallback<void(
      base::expected<std::vector<bitcoin::Transaction>, std::string>)>;
  using PostTransactionCallback =
      base::OnceCallback<void(base::expected<std::string, std::string>)>;

  void GetChainHeight(const std::string& network_id,
                      GetChainHeightCallback callback);

  void GetAddressHistory(const std::string& network_id,
                         const std::string& address,
                         const uint32_t max_block_height,
                         const std::string& last_seen_txid,
                         GetAddressHistoryCallback callback);

  void PostTransaction(const std::string& network_id,
                       const std::vector<uint8_t>& transaction,
                       PostTransactionCallback callback);

 private:
  void RequestInternal(const GURL& request_url,
                       RequestIntermediateCallback callback,
                       APIRequestHelper::ResponseConversionCallback
                           conversion_callback = base::NullCallback());

  void OnGetChainHeight(GetChainHeightCallback callback,
                        APIRequestResult api_request_result);

  void OnGetAddressHistory(const uint32_t max_block_height,
                           GetAddressHistoryCallback callback,
                           APIRequestResult api_request_result);

  void OnPostTransaction(PostTransactionCallback callback,
                         APIRequestResult api_request_result);

  const raw_ptr<PrefService> prefs_;
  scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory_;
  std::unique_ptr<APIRequestHelper> api_request_helper_;
  base::WeakPtrFactory<BitcoinRpc> weak_ptr_factory_{this};
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_RPC_H_
