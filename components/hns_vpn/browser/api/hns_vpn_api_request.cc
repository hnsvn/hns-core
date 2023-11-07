/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/api/hns_vpn_api_request.h"

#include <utility>

#include "base/debug/dump_without_crashing.h"
#include "base/json/json_writer.h"
#include "hns/components/hns_vpn/browser/api/hns_vpn_api_helper.h"
#include "hns/components/hns_vpn/browser/api/vpn_response_parser.h"
#include "hns/components/hns_vpn/common/hns_vpn_constants.h"
#include "hns/components/skus/browser/skus_utils.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "url/gurl.h"
#include "url/url_constants.h"

namespace {

net::NetworkTrafficAnnotationTag GetNetworkTrafficAnnotationTag() {
  return net::DefineNetworkTrafficAnnotation("hns_vpn_service", R"(
      semantics {
        sender: "Hns VPN Service"
        description:
          "This service is used to communicate with Guardian VPN apis"
          "on behalf of the user interacting with the Hns VPN."
        trigger:
          "Triggered by user connecting the Hns VPN."
        data:
          "Servers, hosts and credentials for Hns VPN"
        destination: WEBSITE
      }
      policy {
        cookies_allowed: NO
        policy_exception_justification:
          "Not implemented."
      }
    )");
}

GURL GetURLWithPath(const std::string& host, const std::string& path) {
  return GURL(std::string(url::kHttpsScheme) + "://" + host).Resolve(path);
}

std::string CreateJSONRequestBody(base::ValueView node) {
  std::string json;
  base::JSONWriter::Write(node, &json);
  return json;
}

}  // namespace

namespace hns_vpn {

HnsVpnAPIRequest::HnsVpnAPIRequest(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory)
    : api_request_helper_(GetNetworkTrafficAnnotationTag(),
                          url_loader_factory) {}

HnsVpnAPIRequest::~HnsVpnAPIRequest() = default;

void HnsVpnAPIRequest::GetAllServerRegions(ResponseCallback callback) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetResponse,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url = GetURLWithPath(kVpnHost, kAllServerRegions);
  OAuthRequest(base_url, "GET", "", std::move(internal_callback));
}

void HnsVpnAPIRequest::GetTimezonesForRegions(ResponseCallback callback) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetResponse,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url = GetURLWithPath(kVpnHost, kTimezonesForRegions);
  OAuthRequest(base_url, "GET", "", std::move(internal_callback));
}

void HnsVpnAPIRequest::GetHostnamesForRegion(ResponseCallback callback,
                                               const std::string& region) {
  DCHECK(!region.empty());
  static bool dump_sent = false;
  if (!dump_sent && region.empty()) {
    base::debug::DumpWithoutCrashing();
    dump_sent = true;
  }

  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetResponse,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url = GetURLWithPath(kVpnHost, kHostnameForRegion);
  base::Value::Dict dict;
  dict.Set("region", region);
  std::string request_body = CreateJSONRequestBody(dict);
  OAuthRequest(base_url, "POST", request_body, std::move(internal_callback));
}

void HnsVpnAPIRequest::GetProfileCredentials(
    ResponseCallback callback,
    const std::string& subscriber_credential,
    const std::string& hostname) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetResponse,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url = GetURLWithPath(hostname, kProfileCredential);
  base::Value::Dict dict;
  dict.Set("subscriber-credential", subscriber_credential);
  std::string request_body = CreateJSONRequestBody(dict);
  OAuthRequest(base_url, "POST", request_body, std::move(internal_callback));
}

void HnsVpnAPIRequest::GetWireguardProfileCredentials(
    ResponseCallback callback,
    const std::string& subscriber_credential,
    const std::string& public_key,
    const std::string& hostname) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetResponse,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url = GetURLWithPath(hostname, kCredential);
  base::Value::Dict dict;
  dict.Set("subscriber-credential", subscriber_credential);
  dict.Set("public-key", public_key);
  dict.Set("transport-protocol", "wireguard");
  std::string request_body = CreateJSONRequestBody(dict);
  OAuthRequest(base_url, "POST", request_body, std::move(internal_callback));
}

void HnsVpnAPIRequest::VerifyCredentials(
    ResponseCallback callback,
    const std::string& hostname,
    const std::string& client_id,
    const std::string& subscriber_credential,
    const std::string& api_auth_token) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetResponse,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url =
      GetURLWithPath(hostname, kCredential + client_id + "/verify-credentials");
  base::Value::Dict dict;
  dict.Set("subscriber-credential", subscriber_credential);
  dict.Set("api-auth-token", api_auth_token);
  std::string request_body = CreateJSONRequestBody(dict);
  OAuthRequest(base_url, "POST", request_body, std::move(internal_callback));
}

void HnsVpnAPIRequest::InvalidateCredentials(
    ResponseCallback callback,
    const std::string& hostname,
    const std::string& client_id,
    const std::string& subscriber_credential,
    const std::string& api_auth_token) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetResponse,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url = GetURLWithPath(
      hostname, kCredential + client_id + "/invalidate-credentials");
  base::Value::Dict dict;
  dict.Set("subscriber-credential", subscriber_credential);
  dict.Set("api-auth-token", api_auth_token);
  std::string request_body = CreateJSONRequestBody(dict);
  OAuthRequest(base_url, "POST", request_body, std::move(internal_callback));
}

void HnsVpnAPIRequest::VerifyPurchaseToken(ResponseCallback callback,
                                             const std::string& purchase_token,
                                             const std::string& product_id,
                                             const std::string& product_type,
                                             const std::string& bundle_id) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetResponse,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url = GetURLWithPath(kVpnHost, kVerifyPurchaseToken);
  base::Value::Dict dict;
  dict.Set("purchase-token", purchase_token);
  dict.Set("product-id", product_id);
  dict.Set("product-type", product_type);
  dict.Set("bundle-id", bundle_id);
  std::string request_body = CreateJSONRequestBody(dict);
  OAuthRequest(base_url, "POST", request_body, std::move(internal_callback));
}

void HnsVpnAPIRequest::GetSubscriberCredential(
    ResponseCallback callback,
    const std::string& product_type,
    const std::string& product_id,
    const std::string& validation_method,
    const std::string& purchase_token,
    const std::string& bundle_id) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetSubscriberCredential,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));
  GURL base_url = GetURLWithPath(kVpnHost, kCreateSubscriberCredentialV12);
  base::Value::Dict dict;
  dict.Set("product-type", product_type);
  dict.Set("product-id", product_id);
  dict.Set("validation-method", validation_method);
  dict.Set("purchase-token", purchase_token);
  dict.Set("bundle-id", bundle_id);
  std::string request_body = CreateJSONRequestBody(dict);
  OAuthRequest(base_url, "POST", request_body, std::move(internal_callback));
}

void HnsVpnAPIRequest::GetSubscriberCredentialV12(
    ResponseCallback callback,
    const std::string& skus_credential,
    const std::string& environment) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnGetSubscriberCredential,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));

  const GURL base_url =
      GetURLWithPath(kVpnHost, kCreateSubscriberCredentialV12);
  base::Value::Dict dict;
  dict.Set("validation-method", "hns-premium");
  dict.Set("hns-vpn-premium-monthly-pass", skus_credential);
  std::string request_body = CreateJSONRequestBody(dict);
  OAuthRequest(base_url, "POST", request_body, std::move(internal_callback),
               {{"Hns-Payments-Environment", environment}});
}

void HnsVpnAPIRequest::CreateSupportTicket(
    ResponseCallback callback,
    const std::string& email,
    const std::string& subject,
    const std::string& body,
    const std::string& subscriber_credential) {
  auto internal_callback =
      base::BindOnce(&HnsVpnAPIRequest::OnCreateSupportTicket,
                     weak_ptr_factory_.GetWeakPtr(), std::move(callback));

  OAuthRequest(
      GetURLWithPath(kVpnHost, kCreateSupportTicket), "POST",
      CreateJSONRequestBody(GetValueWithTicketInfos(
          email, subject, body, subscriber_credential, GetTimeZoneName())),
      std::move(internal_callback));
}

void HnsVpnAPIRequest::OAuthRequest(
    const GURL& url,
    const std::string& method,
    const std::string& post_data,
    URLRequestCallback callback,
    const base::flat_map<std::string, std::string>& headers) {
  api_request_helper_.Request(method, url, post_data, "application/json",
                              std::move(callback), headers,
                              {.auto_retry_on_network_change = true});
}

void HnsVpnAPIRequest::OnGetResponse(
    ResponseCallback callback,
    api_request_helper::APIRequestResult result) {
  // NOTE: |api_request_helper_| uses JsonSanitizer to sanitize input made with
  // requests. |body| will be empty when the response from service is invalid
  // json.
  const bool success = result.response_code() == 200;
  std::move(callback).Run(result.body(), success);
}

void HnsVpnAPIRequest::OnGetSubscriberCredential(
    ResponseCallback callback,
    APIRequestResult api_request_result) {
  bool success = api_request_result.response_code() == 200;
  std::string error;
  std::string subscriber_credential =
      ParseSubscriberCredentialFromJson(api_request_result.body(), &error);
  if (!success) {
    subscriber_credential = error;
    VLOG(1) << __func__ << " Response from API was not HTTP 200 (Received "
            << api_request_result.response_code() << ")";
  }
  std::move(callback).Run(subscriber_credential, success);
}

void HnsVpnAPIRequest::OnCreateSupportTicket(
    ResponseCallback callback,
    APIRequestResult api_request_result) {
  bool success = api_request_result.response_code() == 200;
  VLOG(2) << "OnCreateSupportTicket success=" << success
          << "\nresponse_code=" << api_request_result.response_code();
  std::move(callback).Run(api_request_result.body(), success);
}

}  // namespace hns_vpn
