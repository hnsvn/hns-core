/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/endpoint/gemini/post_account/post_account_gemini.h"

#include <utility>

#include "base/json/json_reader.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "hns/components/hns_rewards/core/gemini/gemini_util.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "net/http/http_status_code.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_rewards::internal::endpoint::gemini {

PostAccount::PostAccount(RewardsEngineImpl& engine) : engine_(engine) {}

PostAccount::~PostAccount() = default;

std::string PostAccount::GetUrl() {
  return GetApiServerUrl("/v1/account");
}

mojom::Result PostAccount::ParseBody(const std::string& body,
                                     std::string* linking_info,
                                     std::string* user_name,
                                     std::string* country_id) {
  DCHECK(linking_info);
  DCHECK(user_name);
  DCHECK(country_id);

  absl::optional<base::Value> value = base::JSONReader::Read(body);
  if (!value || !value->is_dict()) {
    BLOG(0, "Invalid JSON");
    return mojom::Result::FAILED;
  }

  const base::Value::Dict& dict = value->GetDict();
  const base::Value::Dict* account = dict.FindDict("account");
  if (!account) {
    BLOG(0, "Missing account info");
    return mojom::Result::FAILED;
  }

  const auto* linking_information = account->FindString("verificationToken");
  if (!linking_info) {
    BLOG(0, "Missing linking info");
    return mojom::Result::FAILED;
  }

  const auto* users = dict.FindList("users");
  if (!users) {
    BLOG(0, "Missing users");
    return mojom::Result::FAILED;
  }

  if (users->size() == 0) {
    BLOG(0, "No users associated with this token");
    return mojom::Result::FAILED;
  }

  const auto* name = (*users)[0].GetDict().FindString("name");
  if (!name) {
    BLOG(0, "Missing user name");
    return mojom::Result::FAILED;
  }

  const auto* country = (*users)[0].GetDict().FindString("countryCode");

  *linking_info = *linking_information;
  *user_name = *name;
  *country_id = country ? *country : "";

  return mojom::Result::OK;
}

void PostAccount::Request(const std::string& token,
                          PostAccountCallback callback) {
  auto request = mojom::UrlRequest::New();
  request->url = GetUrl();
  request->headers = RequestAuthorization(token);
  request->method = mojom::UrlMethod::POST;

  engine_->LoadURL(std::move(request),
                   base::BindOnce(&PostAccount::OnRequest,
                                  base::Unretained(this), std::move(callback)));
}

void PostAccount::OnRequest(PostAccountCallback callback,
                            mojom::UrlResponsePtr response) {
  DCHECK(response);
  LogUrlResponse(__func__, *response);

  mojom::Result result = CheckStatusCode(response->status_code);
  if (result != mojom::Result::OK) {
    return std::move(callback).Run(result, "", "", "");
  }

  std::string linking_info;
  std::string user_name;
  std::string country_id;
  result = ParseBody(response->body, &linking_info, &user_name, &country_id);
  std::move(callback).Run(result, std::move(linking_info), std::move(user_name),
                          std::move(country_id));
}

}  // namespace hns_rewards::internal::endpoint::gemini
