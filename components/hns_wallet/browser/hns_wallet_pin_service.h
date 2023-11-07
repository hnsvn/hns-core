// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_PIN_SERVICE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_PIN_SERVICE_H_

#include <list>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "base/containers/cxx20_erase_deque.h"
#include "base/memory/scoped_refptr.h"
#include "base/memory/weak_ptr.h"
#include "base/task/sequenced_task_runner.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/ipfs/ipfs_service.h"
#include "hns/components/ipfs/pin/ipfs_local_pin_service.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote_set.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_wallet {

class ContentTypeChecker {
 public:
  ContentTypeChecker(
      PrefService* pref_service,
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);
  virtual ~ContentTypeChecker();

  virtual void CheckContentTypeSupported(
      const std::string& cid,
      base::OnceCallback<void(absl::optional<bool>)> callback);

 protected:
  // For tests
  ContentTypeChecker();

 private:
  using UrlLoaderList = std::list<std::unique_ptr<network::SimpleURLLoader>>;

  void OnHeadersFetched(UrlLoaderList::iterator iterator,
                        base::OnceCallback<void(absl::optional<bool>)> callback,
                        scoped_refptr<net::HttpResponseHeaders> headers);

  raw_ptr<PrefService> pref_service_;
  scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory_;
  // List of requests are actively being sent.
  UrlLoaderList loaders_in_progress_;

  base::WeakPtrFactory<ContentTypeChecker> weak_ptr_factory_{this};
};

/**
 * At the moment only local pinning is supported so use absl::nullopt
 * for optional service argument.
 */
class HnsWalletPinService : public KeyedService,
                              public hns_wallet::mojom::WalletPinService,
                              public ipfs::IpfsServiceObserver {
 public:
  HnsWalletPinService(
      PrefService* prefs,
      JsonRpcService* service,
      ipfs::IpfsLocalPinService* local_pin_service,
      IpfsService* ipfs_service,
      std::unique_ptr<ContentTypeChecker> content_type_checker);
  ~HnsWalletPinService() override;

  virtual void Restore();
  virtual void Reset(base::OnceCallback<void(bool)> callback);

  mojo::PendingRemote<mojom::WalletPinService> MakeRemote();
  void Bind(mojo::PendingReceiver<mojom::WalletPinService> receiver);

  static absl::optional<std::string> GetTokenPrefPath(
      const absl::optional<std::string>& service,
      const mojom::BlockchainTokenPtr& token);
  static mojom::BlockchainTokenPtr TokenFromPrefPath(const std::string& path);
  static absl::optional<std::string> ServiceFromPrefPath(
      const std::string& path);
  static std::string StatusToString(const mojom::TokenPinStatusCode& status);
  static std::string ErrorCodeToString(
      const mojom::WalletPinServiceErrorCode& error_code);
  static bool IsTokenSupportedForPinning(
      const mojom::BlockchainTokenPtr& token);

  // WalletPinService
  void AddObserver(::mojo::PendingRemote<mojom::HnsWalletPinServiceObserver>
                       observer) override;
  void AddPin(mojom::BlockchainTokenPtr token,
              const absl::optional<std::string>& service,
              AddPinCallback callback) override;
  void RemovePin(mojom::BlockchainTokenPtr token,
                 const absl::optional<std::string>& service,
                 RemovePinCallback callback) override;
  void GetTokenStatus(mojom::BlockchainTokenPtr token,
                      GetTokenStatusCallback callback) override;
  void Validate(mojom::BlockchainTokenPtr token,
                const absl::optional<std::string>& service,
                ValidateCallback callback) override;
  void IsLocalNodeRunning(IsLocalNodeRunningCallback callback) override;
  void IsTokenSupported(mojom::BlockchainTokenPtr token,
                        IsTokenSupportedCallback callback) override;

  virtual void MarkAsPendingForPinning(
      const mojom::BlockchainTokenPtr& token,
      const absl::optional<std::string>& service);
  virtual void MarkAsPendingForUnpinning(
      const mojom::BlockchainTokenPtr& token,
      const absl::optional<std::string>& service);

  virtual mojom::TokenPinStatusPtr GetTokenStatus(
      const absl::optional<std::string>& service,
      const mojom::BlockchainTokenPtr& token);
  mojom::TokenPinStatusPtr GetTokenStatus(const std::string& path);
  virtual absl::optional<base::Time> GetLastValidateTime(
      const absl::optional<std::string>& service,
      const mojom::BlockchainTokenPtr& token);
  // Returns list of known tokens for the provided pinning service.
  // Tokens are returned in the format of string path.
  // See HnsWalletPinService::GetTokenPrefPath.
  virtual std::set<std::string> GetTokens(
      const absl::optional<std::string>& service);

  size_t GetPinnedTokensCount();

 protected:
  // For testing
  HnsWalletPinService();

 private:
  bool AddToken(const absl::optional<std::string>& service,
                const mojom::BlockchainTokenPtr& token,
                const std::vector<std::string>& cids);
  bool RemoveToken(const absl::optional<std::string>& service,
                   const mojom::BlockchainTokenPtr& token);
  bool SetTokenStatus(const absl::optional<std::string>& service,
                      const mojom::BlockchainTokenPtr& token,
                      mojom::TokenPinStatusCode,
                      const mojom::PinErrorPtr& error);

  absl::optional<std::vector<std::string>> ResolvePinItems(
      const absl::optional<std::string>& service,
      const mojom::BlockchainTokenPtr& token);

  void OnPinsRemoved(absl::optional<std::string> service,
                     RemovePinCallback callback,
                     mojom::BlockchainTokenPtr token,
                     bool result);
  void OnTokenPinned(absl::optional<std::string> service,
                     AddPinCallback callback,
                     mojom::BlockchainTokenPtr,
                     bool result);
  void OnTokenValidated(absl::optional<std::string> service,
                        ValidateCallback callback,
                        mojom::BlockchainTokenPtr,
                        absl::optional<bool> result);

  void ProcessTokenMetadata(const absl::optional<std::string>& service,
                            const mojom::BlockchainTokenPtr& token,
                            const std::string& token_url,
                            const std::string& result,
                            AddPinCallback callback);

  void OnTokenMetaDataReceived(absl::optional<std::string> service,
                               AddPinCallback callback,
                               mojom::BlockchainTokenPtr token,
                               const std::string& token_url,
                               const std::string& result,
                               mojom::ProviderError error,
                               const std::string& error_message);
  void OnContentTypeChecked(absl::optional<std::string> service,
                            mojom::BlockchainTokenPtr token,
                            std::vector<std::string> cids,
                            AddPinCallback callback,
                            absl::optional<bool> result);

  void OnSolTokenMetaDataReceived(absl::optional<std::string> service,
                                  AddPinCallback callback,
                                  mojom::BlockchainTokenPtr token,
                                  const std::string& token_url,
                                  const std::string& result,
                                  mojom::SolanaProviderError error,
                                  const std::string& error_message);

  // ipfs::IpfsServiceObserver
  void OnIpfsLaunched(bool result, int64_t pid) override;
  void OnIpfsShutdown() override;

  void OnResetLocalPinService(base::OnceCallback<void(bool)> callback,
                              bool result);

  mojo::ReceiverSet<hns_wallet::mojom::WalletPinService> receivers_;
  mojo::RemoteSet<mojom::HnsWalletPinServiceObserver> observers_;

  // Prefs service is used to store list of pinned items
  raw_ptr<PrefService> prefs_ = nullptr;

  // JsonRpcService is used to fetch token metadata
  raw_ptr<JsonRpcService> json_rpc_service_ = nullptr;
  raw_ptr<ipfs::IpfsLocalPinService> local_pin_service_ = nullptr;
  raw_ptr<IpfsService> ipfs_service_ = nullptr;
  std::unique_ptr<ContentTypeChecker> content_type_checker_;

  base::WeakPtrFactory<HnsWalletPinService> weak_ptr_factory_{this};
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_PIN_SERVICE_H_
