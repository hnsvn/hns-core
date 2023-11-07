// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/hns_wallet/browser/hns_wallet_pin_service.h"

#include <utility>

#include "base/json/values_util.h"
#include "base/ranges/algorithm.h"
#include "base/strings/strcat.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/task/bind_post_task.h"
#include "base/task/thread_pool.h"
#include "base/values.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/json_rpc_response_parser.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/ipfs/ipfs_constants.h"
#include "hns/components/ipfs/ipfs_utils.h"
#include "components/prefs/scoped_user_pref_update.h"
#include "services/network/public/cpp/resource_request.h"

namespace hns_wallet {

const char kAssetStatus[] = "status";
const char kValidateTimestamp[] = "validate_timestamp";
const char kError[] = "error";
const char kErrorCode[] = "error_code";
const char kErrorMessage[] = "error_message";
const char kAssetUrlListKey[] = "urls";

namespace {
// Solana NFTs don't have token_id
const char kEmptyTokenIdPart[] = "*";
const char kNftPart[] = "nft";
/**
 * Service name used in prefs for local pinning service.
 * Use absl::nullopt in methods to perform operations on
 * the local pinning service.
 */
const char kLocalService[] = "local";

absl::optional<mojom::TokenPinStatusCode> StringToStatus(
    const std::string& status) {
  if (status == "not_pinned") {
    return mojom::TokenPinStatusCode::STATUS_NOT_PINNED;
  } else if (status == "pinning_failed") {
    return mojom::TokenPinStatusCode::STATUS_PINNING_FAILED;
  } else if (status == "pinned") {
    return mojom::TokenPinStatusCode::STATUS_PINNED;
  } else if (status == "pinning_in_progress") {
    return mojom::TokenPinStatusCode::STATUS_PINNING_IN_PROGRESS;
  } else if (status == "unpinning_in_progress") {
    return mojom::TokenPinStatusCode::STATUS_UNPINNING_IN_PROGRESS;
  } else if (status == "unpinning_failed") {
    return mojom::TokenPinStatusCode::STATUS_UNPINNING_FAILED;
  } else if (status == "pinning_pendig") {
    return mojom::TokenPinStatusCode::STATUS_PINNING_PENDING;
  } else if (status == "unpinning_pendig") {
    return mojom::TokenPinStatusCode::STATUS_UNPINNING_PENDING;
  }
  return absl::nullopt;
}

absl::optional<mojom::WalletPinServiceErrorCode> StringToErrorCode(
    const std::string& error) {
  if (error == "ERR_WRONG_TOKEN") {
    return mojom::WalletPinServiceErrorCode::ERR_WRONG_TOKEN;
  } else if (error == "ERR_NON_IPFS_TOKEN_URL") {
    return mojom::WalletPinServiceErrorCode::ERR_NON_IPFS_TOKEN_URL;
  } else if (error == "ERR_FETCH_METADATA_FAILED") {
    return mojom::WalletPinServiceErrorCode::ERR_FETCH_METADATA_FAILED;
  } else if (error == "ERR_WRONG_METADATA_FORMAT") {
    return mojom::WalletPinServiceErrorCode::ERR_WRONG_METADATA_FORMAT;
  } else if (error == "ERR_ALREADY_PINNED") {
    return mojom::WalletPinServiceErrorCode::ERR_ALREADY_PINNED;
  } else if (error == "ERR_NOT_PINNED") {
    return mojom::WalletPinServiceErrorCode::ERR_NOT_PINNED;
  } else if (error == "ERR_PINNING_FAILED") {
    return mojom::WalletPinServiceErrorCode::ERR_PINNING_FAILED;
  } else if (error == "ERR_MEDIA_TYPE_UNSUPPORTED") {
    return mojom::WalletPinServiceErrorCode::ERR_MEDIA_TYPE_UNSUPPORTED;
  }
  return absl::nullopt;
}

absl::optional<std::string> ExtractIpfsUrl(const std::string& url) {
  GURL gurl = GURL(url);

  if (!gurl.is_valid()) {
    return absl::nullopt;
  }

  if (gurl.SchemeIs(ipfs::kIPFSScheme)) {
    return gurl.spec();
  }

  auto source = ipfs::ExtractSourceFromGateway(gurl);
  if (!source || !source->SchemeIs(ipfs::kIPFSScheme)) {
    return absl::nullopt;
  }

  return source.value().spec();
}

}  // namespace

// static
std::string HnsWalletPinService::StatusToString(
    const mojom::TokenPinStatusCode& status) {
  switch (status) {
    case mojom::TokenPinStatusCode::STATUS_NOT_PINNED:
      return "not_pinned";
    case mojom::TokenPinStatusCode::STATUS_PINNED:
      return "pinned";
    case mojom::TokenPinStatusCode::STATUS_PINNING_IN_PROGRESS:
      return "pinning_in_progress";
    case mojom::TokenPinStatusCode::STATUS_UNPINNING_IN_PROGRESS:
      return "unpinning_in_progress";
    case mojom::TokenPinStatusCode::STATUS_UNPINNING_FAILED:
      return "unpinning_failed";
    case mojom::TokenPinStatusCode::STATUS_PINNING_FAILED:
      return "pinning_failed";
    case mojom::TokenPinStatusCode::STATUS_PINNING_PENDING:
      return "pinning_pendig";
    case mojom::TokenPinStatusCode::STATUS_UNPINNING_PENDING:
      return "unpinning_pendig";
  }
  NOTREACHED();
  return "";
}

// static
std::string HnsWalletPinService::ErrorCodeToString(
    const mojom::WalletPinServiceErrorCode& error_code) {
  switch (error_code) {
    case mojom::WalletPinServiceErrorCode::ERR_WRONG_TOKEN:
      return "ERR_WRONG_TOKEN";
    case mojom::WalletPinServiceErrorCode::ERR_NON_IPFS_TOKEN_URL:
      return "ERR_NON_IPFS_TOKEN_URL";
    case mojom::WalletPinServiceErrorCode::ERR_FETCH_METADATA_FAILED:
      return "ERR_FETCH_METADATA_FAILED";
    case mojom::WalletPinServiceErrorCode::ERR_WRONG_METADATA_FORMAT:
      return "ERR_WRONG_METADATA_FORMAT";
    case mojom::WalletPinServiceErrorCode::ERR_ALREADY_PINNED:
      return "ERR_ALREADY_PINNED";
    case mojom::WalletPinServiceErrorCode::ERR_NOT_PINNED:
      return "ERR_NOT_PINNED";
    case mojom::WalletPinServiceErrorCode::ERR_PINNING_FAILED:
      return "ERR_PINNING_FAILED";
    case mojom::WalletPinServiceErrorCode::ERR_MEDIA_TYPE_UNSUPPORTED:
      return "ERR_MEDIA_TYPE_UNSUPPORTED";
  }
  NOTREACHED();
  return "";
}

ContentTypeChecker::ContentTypeChecker(
    PrefService* pref_service,
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory)
    : pref_service_(pref_service),
      url_loader_factory_(std::move(url_loader_factory)) {}

ContentTypeChecker::ContentTypeChecker() = default;

ContentTypeChecker::~ContentTypeChecker() = default;

void ContentTypeChecker::CheckContentTypeSupported(
    const std::string& ipfs_url,
    base::OnceCallback<void(absl::optional<bool>)> callback) {
  // Create a request with no data or cookies.
  auto resource_request = std::make_unique<network::ResourceRequest>();
  GURL translated_url;
  if (!ipfs::TranslateIPFSURI(GURL(ipfs_url), &translated_url,
                              ipfs::GetDefaultNFTIPFSGateway(pref_service_),
                              false)) {
    std::move(callback).Run(false);
    return;
  }
  resource_request->url = translated_url;
  resource_request->credentials_mode = network::mojom::CredentialsMode::kOmit;
  resource_request->redirect_mode = ::network::mojom::RedirectMode::kFollow;

  auto annotation =
      net::DefineNetworkTrafficAnnotation("hns_wallet_pin_service", R"(
            semantics {
              sender: "Hns wallet pin service"
              description:
                "This service is used to pin NFTs"
                "which are added to the wallet."
              trigger:
                "Triggered by enable auto-pinning mode"
                "from the Hns Wallet page or settings."
              data:
                "Options of the commands."
              destination: WEBSITE
            }
            policy {
              cookies_allowed: NO
              setting:
                "You can enable or disable this feature in hns://settings."
              policy_exception_justification:
                "Not implemented."
            }
          )");

  auto url_loader =
      network::SimpleURLLoader::Create(std::move(resource_request), annotation);
  auto* url_loader_ptr = url_loader.get();
  auto it = loaders_in_progress_.insert(loaders_in_progress_.end(),
                                        std::move(url_loader));

  url_loader_ptr->SetTimeoutDuration(base::Seconds(60));
  url_loader_ptr->SetRetryOptions(
      5, network::SimpleURLLoader::RetryMode::RETRY_ON_5XX |
             network::SimpleURLLoader::RetryMode::RETRY_ON_NETWORK_CHANGE);
  url_loader_ptr->DownloadHeadersOnly(
      url_loader_factory_.get(),
      base::BindOnce(&ContentTypeChecker::OnHeadersFetched,
                     weak_ptr_factory_.GetWeakPtr(), std::move(it),
                     std::move(callback)));
}

void ContentTypeChecker::OnHeadersFetched(
    UrlLoaderList::iterator loader_it,
    base::OnceCallback<void(absl::optional<bool>)> callback,
    scoped_refptr<net::HttpResponseHeaders> headers) {
  loaders_in_progress_.erase(loader_it);
  if (!headers) {
    std::move(callback).Run(absl::nullopt);
    return;
  }
  std::string content_type_value;
  headers->GetMimeType(&content_type_value);
  if (base::StartsWith(content_type_value, "image/")) {
    std::move(callback).Run(true);
  } else {
    std::move(callback).Run(false);
  }
}

// static
bool HnsWalletPinService::IsTokenSupportedForPinning(
    const mojom::BlockchainTokenPtr& token) {
  return token->is_erc721 ||
         (token->is_nft && token->coin == mojom::CoinType::SOL);
}

/**
 * Structure of kPinnedNFTAssets prefs:
 * // Type of pinned content
 * "nft" : {
 *   // List of services
 *   "local" : {
 *     // Coin type
 *     "60": {
 *       // Chain id
 *       "0x1": {
 *         // Contract
 *         "0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d" : {
 *           // Token id
 *           "3139" : {
 *             "status": <status code>,
 *             "validate_timestamp": <last validate timestamp>,
 *             "error": {
 *               "error_code": <error code>,
 *               "error_message": <error message>,
 *             },
 *             cids: [
 *               // List of related CIDs
 *               "bafy..",
 *               ...
 *             ]
 *           }
 *           ...
 *         }
 *         ...
 *       }
 *       ...
 *     }
 *     ...
 *   },
 *   // Remote service
 *   nftstorage : {
 *     ...
 *   }
 * }
 */
HnsWalletPinService::HnsWalletPinService(
    PrefService* prefs,
    JsonRpcService* service,
    ipfs::IpfsLocalPinService* local_pin_service,
    IpfsService* ipfs_service,
    std::unique_ptr<ContentTypeChecker> content_type_checker)
    : prefs_(prefs),
      json_rpc_service_(service),
      local_pin_service_(local_pin_service),
      ipfs_service_(ipfs_service),
      content_type_checker_(std::move(content_type_checker)) {
  ipfs_service_->AddObserver(this);
}

// For testing
HnsWalletPinService::HnsWalletPinService() = default;

void HnsWalletPinService::Restore() {
  local_pin_service_->ScheduleGcTask();
}

void HnsWalletPinService::Reset(base::OnceCallback<void(bool)> callback) {
  weak_ptr_factory_.InvalidateWeakPtrs();
  local_pin_service_->Reset(
      base::BindOnce(&HnsWalletPinService::OnResetLocalPinService,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback)));
}

void HnsWalletPinService::OnResetLocalPinService(
    base::OnceCallback<void(bool)> callback,
    bool result) {
  if (!result) {
    std::move(callback).Run(false);
    return;
  }
  prefs_->ClearPref(kPinnedNFTAssets);
  std::move(callback).Run(true);
}

HnsWalletPinService::~HnsWalletPinService() {
  if (ipfs_service_) {
    ipfs_service_->RemoveObserver(this);
  }
}

mojo::PendingRemote<mojom::WalletPinService>
HnsWalletPinService::MakeRemote() {
  mojo::PendingRemote<WalletPinService> remote;
  receivers_.Add(this, remote.InitWithNewPipeAndPassReceiver());
  return remote;
}

void HnsWalletPinService::Bind(
    mojo::PendingReceiver<mojom::WalletPinService> receiver) {
  receivers_.Add(this, std::move(receiver));
}

void HnsWalletPinService::AddObserver(
    ::mojo::PendingRemote<mojom::HnsWalletPinServiceObserver> observer) {
  observers_.Add(std::move(observer));
}

// static
absl::optional<std::string> HnsWalletPinService::GetTokenPrefPath(
    const absl::optional<std::string>& service,
    const mojom::BlockchainTokenPtr& token) {
  if (service && base::ContainsOnlyChars(service.value(), ".")) {
    return absl::nullopt;
  }
  if (base::ContainsOnlyChars(token->contract_address, ".")) {
    return absl::nullopt;
  }
  if (!token->token_id.empty() &&
      base::ContainsOnlyChars(token->token_id, ".")) {
    return absl::nullopt;
  }
  DCHECK(!base::ContainsOnlyChars(token->chain_id, "."));
  return base::StrCat(
      {kNftPart, ".", service.value_or(kLocalService), ".",
       base::NumberToString(static_cast<int>(token->coin)), ".",
       token->chain_id, ".", token->contract_address, ".",
       (token->token_id.empty() ? kEmptyTokenIdPart : token->token_id)});
}

// static
mojom::BlockchainTokenPtr HnsWalletPinService::TokenFromPrefPath(
    const std::string& path) {
  std::vector<std::string> parts =
      base::SplitString(path, ".", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  if (parts.size() != 6) {
    return nullptr;
  }
  mojom::BlockchainTokenPtr token = mojom::BlockchainToken::New();
  int32_t coin;
  if (!base::StringToInt(parts.at(2), &coin)) {
    return nullptr;
  }
  token->coin = static_cast<mojom::CoinType>(coin);
  token->chain_id = parts.at(3);
  token->contract_address = parts.at(4);
  token->token_id = parts.at(5) == kEmptyTokenIdPart ? "" : parts.at(5);
  token->is_nft = true;
  return token;
}

// static
absl::optional<std::string> HnsWalletPinService::ServiceFromPrefPath(
    const std::string& path) {
  std::vector<std::string> parts =
      base::SplitString(path, ".", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  if (parts.size() != 6) {
    return nullptr;
  }
  if (parts.at(1) == kLocalService) {
    return absl::nullopt;
  } else {
    return parts.at(1);
  }
}

void HnsWalletPinService::Validate(mojom::BlockchainTokenPtr token,
                                     const absl::optional<std::string>& service,
                                     ValidateCallback callback) {
  auto path = GetTokenPrefPath(absl::nullopt, token);
  if (!path) {
    std::move(callback).Run(mojom::TokenValidationResult::kValidationIgnored);
    return;
  }

  mojom::TokenPinStatusPtr status = GetTokenStatus(service, token);
  if (!status) {
    std::move(callback).Run(mojom::TokenValidationResult::kValidationIgnored);
    return;
  }

  if (status->code != mojom::TokenPinStatusCode::STATUS_PINNED) {
    std::move(callback).Run(mojom::TokenValidationResult::kValidationIgnored);
    return;
  }

  absl::optional<std::vector<std::string>> ipfs_urls =
      ResolvePinItems(service, token);

  if (!ipfs_urls) {
    SetTokenStatus(service, std::move(token),
                   mojom::TokenPinStatusCode::STATUS_PINNING_IN_PROGRESS,
                   nullptr);
    std::move(callback).Run(mojom::TokenValidationResult::kValidationFailed);
    return;
  }

  if (!service) {
    local_pin_service_->ValidatePins(
        path.value(), ipfs_urls.value(),
        base::BindOnce(&HnsWalletPinService::OnTokenValidated,
                       weak_ptr_factory_.GetWeakPtr(), service,
                       std::move(callback), std::move(token)));
  } else {
    // Remote pinning not implemented yet
    std::move(callback).Run(mojom::TokenValidationResult::kValidationIgnored);
  }
}

void HnsWalletPinService::IsLocalNodeRunning(
    IsLocalNodeRunningCallback callback) {
  std::move(callback).Run(ipfs_service_->IsDaemonLaunched());
}

void HnsWalletPinService::IsTokenSupported(
    mojom::BlockchainTokenPtr token,
    IsTokenSupportedCallback callback) {
  std::move(callback).Run(IsTokenSupportedForPinning(token));
}

void HnsWalletPinService::OnIpfsLaunched(bool result, int64_t pid) {
  for (const auto& observer : observers_) {
    observer->OnLocalNodeStatusChanged(result);
  }
}

void HnsWalletPinService::OnIpfsShutdown() {
  for (const auto& observer : observers_) {
    observer->OnLocalNodeStatusChanged(false);
  }
}

void HnsWalletPinService::MarkAsPendingForPinning(
    const mojom::BlockchainTokenPtr& token,
    const absl::optional<std::string>& service) {
  SetTokenStatus(service, token,
                 mojom::TokenPinStatusCode::STATUS_PINNING_PENDING, nullptr);
}

void HnsWalletPinService::MarkAsPendingForUnpinning(
    const mojom::BlockchainTokenPtr& token,
    const absl::optional<std::string>& service) {
  SetTokenStatus(service, token,
                 mojom::TokenPinStatusCode::STATUS_UNPINNING_PENDING, nullptr);
}

void HnsWalletPinService::AddPin(mojom::BlockchainTokenPtr token,
                                   const absl::optional<std::string>& service,
                                   AddPinCallback callback) {
  if (!IsTokenSupportedForPinning(token)) {
    auto pin_error =
        mojom::PinError::New(mojom::WalletPinServiceErrorCode::ERR_WRONG_TOKEN,
                             "Token pinning is not supported");
    std::move(callback).Run(false, std::move(pin_error));
    return;
  }
  auto token_status = GetTokenStatus(service, token);
  if (token_status &&
      token_status->code == mojom::TokenPinStatusCode::STATUS_PINNED) {
    auto pin_error = mojom::PinError::New(
        mojom::WalletPinServiceErrorCode::ERR_ALREADY_PINNED, "Already pinned");
    std::move(callback).Run(true, std::move(pin_error));
    return;
  }

  if (token->is_erc721) {
    json_rpc_service_->GetERC721Metadata(
        token->contract_address, token->token_id, token->chain_id,
        base::BindOnce(&HnsWalletPinService::OnTokenMetaDataReceived,
                       weak_ptr_factory_.GetWeakPtr(), service,
                       std::move(callback), token.Clone()));
  } else if (token->is_nft && token->coin == mojom::CoinType::SOL) {
    json_rpc_service_->GetSolTokenMetadata(
        token->chain_id, token->contract_address,
        base::BindOnce(&HnsWalletPinService::OnSolTokenMetaDataReceived,
                       weak_ptr_factory_.GetWeakPtr(), service,
                       std::move(callback), token.Clone()));
  } else {
    NOTREACHED();
  }
}

void HnsWalletPinService::RemovePin(
    mojom::BlockchainTokenPtr token,
    const absl::optional<std::string>& service,
    RemovePinCallback callback) {
  auto token_status = GetTokenStatus(service, token);
  if (!token_status) {
    std::move(callback).Run(true, nullptr);
    return;
  }

  auto path = GetTokenPrefPath(absl::nullopt, token);
  if (!path) {
    std::move(callback).Run(
        false,
        mojom::PinError::New(mojom::WalletPinServiceErrorCode::ERR_WRONG_TOKEN,
                             "Wrong token data"));
    return;
  }

  SetTokenStatus(service, token,
                 mojom::TokenPinStatusCode::STATUS_UNPINNING_IN_PROGRESS,
                 nullptr);

  if (!service) {
    local_pin_service_->RemovePins(
        path.value(), base::BindOnce(&HnsWalletPinService::OnPinsRemoved,
                                     weak_ptr_factory_.GetWeakPtr(), service,
                                     std::move(callback), std::move(token)));
  } else {
    // Remote pinning not implemented yet
    std::move(callback).Run(false, nullptr);
  }
}

void HnsWalletPinService::GetTokenStatus(mojom::BlockchainTokenPtr token,
                                           GetTokenStatusCallback callback) {
  mojom::TokenPinOverviewPtr result = mojom::TokenPinOverview::New();
  result->local = GetTokenStatus(absl::nullopt, token);
  std::move(callback).Run(std::move(result), nullptr);
}

void HnsWalletPinService::OnPinsRemoved(absl::optional<std::string> service,
                                          RemovePinCallback callback,
                                          mojom::BlockchainTokenPtr token,
                                          bool result) {
  if (result) {
    RemoveToken(service, token);
  } else {
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_UNPINNING_FAILED, nullptr);
  }

  std::move(callback).Run(result, nullptr);
}

void HnsWalletPinService::OnSolTokenMetaDataReceived(
    absl::optional<std::string> service,
    AddPinCallback callback,
    mojom::BlockchainTokenPtr token,
    const std::string& token_url,
    const std::string& result,
    mojom::SolanaProviderError error,
    const std::string& error_message) {
  if (error != mojom::SolanaProviderError::kSuccess) {
    auto pin_error = mojom::PinError::New(
        mojom::WalletPinServiceErrorCode::ERR_FETCH_METADATA_FAILED,
        "Failed to obtain token metadata");
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_FAILED, pin_error);
    std::move(callback).Run(false, std::move(pin_error));
    return;
  }

  ProcessTokenMetadata(service, token, token_url, result, std::move(callback));
}

void HnsWalletPinService::OnTokenMetaDataReceived(
    absl::optional<std::string> service,
    AddPinCallback callback,
    mojom::BlockchainTokenPtr token,
    const std::string& token_url,
    const std::string& result,
    mojom::ProviderError error,
    const std::string& error_message) {
  if (error != mojom::ProviderError::kSuccess) {
    auto pin_error = mojom::PinError::New(
        mojom::WalletPinServiceErrorCode::ERR_FETCH_METADATA_FAILED,
        "Failed to obtain token metadata");
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_FAILED, pin_error);
    std::move(callback).Run(false, std::move(pin_error));
    return;
  }

  ProcessTokenMetadata(service, token, token_url, result, std::move(callback));
}

void HnsWalletPinService::ProcessTokenMetadata(
    const absl::optional<std::string>& service,
    const mojom::BlockchainTokenPtr& token,
    const std::string& token_url,
    const std::string& result,
    AddPinCallback callback) {
  auto metadata_ipfs_url = ExtractIpfsUrl(token_url);
  if (!metadata_ipfs_url) {
    auto pin_error = mojom::PinError::New(
        mojom::WalletPinServiceErrorCode::ERR_NON_IPFS_TOKEN_URL,
        "Metadata has non-ipfs url");
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_FAILED, pin_error);
    std::move(callback).Run(false, std::move(pin_error));
    return;
  }

  absl::optional<base::Value> parsed_result = base::JSONReader::Read(
      result, base::JSON_PARSE_CHROMIUM_EXTENSIONS |
                  base::JSONParserOptions::JSON_PARSE_RFC);
  if (!parsed_result || !parsed_result->is_dict()) {
    auto pin_error = mojom::PinError::New(
        mojom::WalletPinServiceErrorCode::ERR_WRONG_METADATA_FORMAT,
        "Wrong metadata format");
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_FAILED, pin_error);
    std::move(callback).Run(false, std::move(pin_error));
    return;
  }

  auto* image_url = parsed_result->GetDict().FindString("image");
  auto image_ipfs_url =
      image_url != nullptr ? ExtractIpfsUrl(*image_url) : absl::nullopt;

  if (!image_ipfs_url) {
    auto pin_error = mojom::PinError::New(
        mojom::WalletPinServiceErrorCode::ERR_NON_IPFS_TOKEN_URL,
        "Can't find proper image field");
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_FAILED, pin_error);
    std::move(callback).Run(false, std::move(pin_error));
    return;
  }

  std::vector<std::string> ipfs_urls;
  ipfs_urls.push_back(metadata_ipfs_url.value());
  ipfs_urls.push_back(image_ipfs_url.value());

  content_type_checker_->CheckContentTypeSupported(
      image_ipfs_url.value(),
      base::BindOnce(&HnsWalletPinService::OnContentTypeChecked,
                     weak_ptr_factory_.GetWeakPtr(), service, token.Clone(),
                     std::move(ipfs_urls), std::move(callback)));
}

void HnsWalletPinService::OnContentTypeChecked(
    absl::optional<std::string> service,
    mojom::BlockchainTokenPtr token,
    std::vector<std::string> ipfs_urls,
    AddPinCallback callback,
    absl::optional<bool> result) {
  if (!result.has_value()) {
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_FAILED, nullptr);
    std::move(callback).Run(
        false, mojom::PinError::New(
                   mojom::WalletPinServiceErrorCode::ERR_FETCH_METADATA_FAILED,
                   "Failed to verify media type"));
    return;
  }

  if (!result.value()) {
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_FAILED, nullptr);
    std::move(callback).Run(
        false, mojom::PinError::New(
                   mojom::WalletPinServiceErrorCode::ERR_MEDIA_TYPE_UNSUPPORTED,
                   "Media type not supported"));
    return;
  }

  auto path = GetTokenPrefPath(service, token);
  if (!path) {
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_FAILED, nullptr);
    std::move(callback).Run(
        false, mojom::PinError::New(
                   mojom::WalletPinServiceErrorCode::ERR_WRONG_METADATA_FORMAT,
                   "Wrong token data"));
    return;
  }

  AddToken(service, token, ipfs_urls);
  SetTokenStatus(service, token,
                 mojom::TokenPinStatusCode::STATUS_PINNING_IN_PROGRESS,
                 nullptr);

  if (!service) {
    local_pin_service_->AddPins(
        path.value(), ipfs_urls,
        base::BindOnce(&HnsWalletPinService::OnTokenPinned,
                       weak_ptr_factory_.GetWeakPtr(), absl::nullopt,
                       std::move(callback), std::move(token)));
  } else {
    // Remote pinning not implemented yet
    std::move(callback).Run(false, nullptr);
  }
}

void HnsWalletPinService::OnTokenPinned(absl::optional<std::string> service,
                                          AddPinCallback callback,
                                          mojom::BlockchainTokenPtr token,
                                          bool result) {
  auto error = !result
                   ? mojom::PinError::New(
                         mojom::WalletPinServiceErrorCode::ERR_PINNING_FAILED,
                         "Pinning failed")
                   : nullptr;
  SetTokenStatus(service, token,
                 result ? mojom::TokenPinStatusCode::STATUS_PINNED
                        : mojom::TokenPinStatusCode::STATUS_PINNING_FAILED,
                 error);

  std::move(callback).Run(result, std::move(error));
}

void HnsWalletPinService::OnTokenValidated(
    absl::optional<std::string> service,
    ValidateCallback callback,
    mojom::BlockchainTokenPtr token,
    absl::optional<bool> result) {
  if (!result.has_value()) {
    std::move(callback).Run(mojom::TokenValidationResult::kValidationError);
    return;
  }

  if (!result.value()) {
    SetTokenStatus(service, token,
                   mojom::TokenPinStatusCode::STATUS_PINNING_IN_PROGRESS,
                   nullptr);
    std::move(callback).Run(mojom::TokenValidationResult::kValidationFailed);
  } else {
    // Also updates verification timestamp
    SetTokenStatus(service, token, mojom::TokenPinStatusCode::STATUS_PINNED,
                   nullptr);
    std::move(callback).Run(mojom::TokenValidationResult::kValidationPassed);
  }
}

bool HnsWalletPinService::AddToken(
    const absl::optional<std::string>& service,
    const mojom::BlockchainTokenPtr& token,
    const std::vector<std::string>& ipfs_urls) {
  auto path = GetTokenPrefPath(service, token);
  if (!path) {
    return false;
  }

  {
    ScopedDictPrefUpdate update(prefs_, kPinnedNFTAssets);
    base::Value::Dict& update_dict = update.Get();

    base::Value::Dict token_data;
    base::Value::List ipfs_urls_list;

    for (const auto& ipfs_url : ipfs_urls) {
      ipfs_urls_list.Append(ipfs_url);
    }

    token_data.Set(kAssetUrlListKey, std::move(ipfs_urls_list));
    token_data.Set(
        kAssetStatus,
        StatusToString(mojom::TokenPinStatusCode::STATUS_NOT_PINNED));

    update_dict.SetByDottedPath(path.value(), std::move(token_data));
  }
  return true;
}

bool HnsWalletPinService::RemoveToken(
    const absl::optional<std::string>& service,
    const mojom::BlockchainTokenPtr& token) {
  auto path = GetTokenPrefPath(service, token);
  if (!path) {
    return false;
  }

  {
    ScopedDictPrefUpdate update(prefs_, kPinnedNFTAssets);
    base::Value::Dict& update_dict = update.Get();
    update_dict.RemoveByDottedPath(path.value());
  }
  for (const auto& observer : observers_) {
    observer->OnTokenStatusChanged(service, token.Clone(),
                                   GetTokenStatus(service, token));
  }
  return true;
}

bool HnsWalletPinService::SetTokenStatus(
    const absl::optional<std::string>& service,
    const mojom::BlockchainTokenPtr& token,
    mojom::TokenPinStatusCode status,
    const mojom::PinErrorPtr& error) {
  auto path = GetTokenPrefPath(service, token);
  if (!path) {
    return false;
  }

  {
    ScopedDictPrefUpdate update(prefs_, kPinnedNFTAssets);
    base::Value::Dict& update_dict = update.Get();

    update_dict.SetByDottedPath(path.value() + "." + kAssetStatus,
                                StatusToString(status));
    if (error) {
      base::Value::Dict error_dict;
      error_dict.Set(kErrorCode, ErrorCodeToString(error->error_code));
      error_dict.Set(kErrorMessage, error->message);
      update_dict.SetByDottedPath(path.value() + "." + kError,
                                  std::move(error_dict));
    } else {
      update_dict.RemoveByDottedPath(path.value() + "." + kError);
    }

    if (status == mojom::TokenPinStatusCode::STATUS_PINNED) {
      update_dict.SetByDottedPath(path.value() + "." + kValidateTimestamp,
                                  base::TimeToValue(base::Time::Now()));
    } else {
      update_dict.RemoveByDottedPath(path.value() + "." + kValidateTimestamp);
    }
  }
  for (const auto& observer : observers_) {
    observer->OnTokenStatusChanged(service, token.Clone(),
                                   GetTokenStatus(service, token));
  }
  return true;
}

absl::optional<std::vector<std::string>> HnsWalletPinService::ResolvePinItems(
    const absl::optional<std::string>& service,
    const mojom::BlockchainTokenPtr& token) {
  const base::Value::Dict& pinned_assets_pref =
      prefs_->GetDict(kPinnedNFTAssets);

  auto path = GetTokenPrefPath(service, token);
  if (!path) {
    return absl::nullopt;
  }

  auto* token_data_as_dict =
      pinned_assets_pref.FindDictByDottedPath(path.value());
  if (!token_data_as_dict) {
    return absl::nullopt;
  }

  auto* ipfs_urls = token_data_as_dict->FindList(kAssetUrlListKey);
  if (!ipfs_urls) {
    return absl::nullopt;
  }

  std::vector<std::string> result;
  for (const base::Value& item : *ipfs_urls) {
    result.push_back(item.GetString());
  }

  return result;
}

mojom::TokenPinStatusPtr HnsWalletPinService::GetTokenStatus(
    const std::string& path) {
  const base::Value::Dict& pinned_assets_pref =
      prefs_->GetDict(kPinnedNFTAssets);

  auto* token_data_as_dict = pinned_assets_pref.FindDictByDottedPath(path);
  if (!token_data_as_dict) {
    return mojom::TokenPinStatus::New(
        mojom::TokenPinStatusCode::STATUS_NOT_PINNED, nullptr, base::Time());
  }

  auto* status = token_data_as_dict->FindString(kAssetStatus);
  if (!status) {
    return mojom::TokenPinStatus::New(
        mojom::TokenPinStatusCode::STATUS_NOT_PINNED, nullptr, base::Time());
  }

  auto pin_status = StringToStatus(*status).value_or(
      mojom::TokenPinStatusCode::STATUS_NOT_PINNED);
  base::Time validate_timestamp;
  mojom::PinErrorPtr error;

  {
    auto* validate_timestamp_value =
        token_data_as_dict->Find(kValidateTimestamp);
    if (validate_timestamp_value) {
      validate_timestamp =
          base::ValueToTime(validate_timestamp_value).value_or(base::Time());
    }

    auto* error_dict = token_data_as_dict->FindDict(kError);
    if (error_dict) {
      auto* error_message = error_dict->FindString(kErrorMessage);
      auto* error_code = error_dict->FindString(kErrorCode);
      if (error_code && error_message) {
        error = mojom::PinError::New(
            StringToErrorCode(*error_code)
                .value_or(mojom::WalletPinServiceErrorCode::ERR_PINNING_FAILED),
            *error_message);
      }
    }
  }

  return mojom::TokenPinStatus::New(pin_status, std::move(error),
                                    validate_timestamp);
}

mojom::TokenPinStatusPtr HnsWalletPinService::GetTokenStatus(
    const absl::optional<std::string>& service,
    const mojom::BlockchainTokenPtr& token) {
  auto path = GetTokenPrefPath(service, token);
  if (!path) {
    return nullptr;
  }

  return GetTokenStatus(path.value());
}

absl::optional<base::Time> HnsWalletPinService::GetLastValidateTime(
    const absl::optional<std::string>& service,
    const mojom::BlockchainTokenPtr& token) {
  const base::Value::Dict& pinned_assets_pref =
      prefs_->GetDict(kPinnedNFTAssets);

  auto path = GetTokenPrefPath(service, token);
  if (!path) {
    return absl::nullopt;
  }

  auto* token_data_as_dict =
      pinned_assets_pref.FindDictByDottedPath(path.value());
  if (!token_data_as_dict) {
    return absl::nullopt;
  }

  auto* time = token_data_as_dict->Find(kValidateTimestamp);
  return base::ValueToTime(time);
}

std::set<std::string> HnsWalletPinService::GetTokens(
    const absl::optional<std::string>& service) {
  std::set<std::string> result;

  const base::Value::Dict& pinned_assets_pref =
      prefs_->GetDict(kPinnedNFTAssets);

  const base::Value::Dict* service_dict =
      pinned_assets_pref.FindDictByDottedPath(
          base::StrCat({kNftPart, ".", service.value_or(kLocalService)}));
  if (!service_dict) {
    return result;
  }

  for (auto coin_it : *service_dict) {
    std::string current_coin = coin_it.first;
    auto* network_dict = coin_it.second.GetIfDict();
    if (!network_dict) {
      continue;
    }
    for (auto network_it : *network_dict) {
      std::string current_network = network_it.first;
      const base::Value::Dict* contract_dict = network_it.second.GetIfDict();
      if (!contract_dict) {
        continue;
      }
      for (auto contract_it : *contract_dict) {
        std::string current_contract = contract_it.first;
        const base::Value::Dict* id_dict = contract_it.second.GetIfDict();
        if (!id_dict) {
          continue;
        }
        for (auto token_id : *id_dict) {
          result.insert(
              base::StrCat({kNftPart, ".", service.value_or(kLocalService), ".",
                            current_coin, ".", current_network, ".",
                            current_contract, ".", token_id.first}));
        }
      }
    }
  }

  return result;
}

size_t HnsWalletPinService::GetPinnedTokensCount() {
  return base::ranges::count_if(GetTokens(absl::nullopt), [this](auto& v) {
    auto status = this->GetTokenStatus(v);
    return !status.is_null() &&
           status->code == mojom::TokenPinStatusCode::STATUS_PINNED;
  });
}

}  // namespace hns_wallet
