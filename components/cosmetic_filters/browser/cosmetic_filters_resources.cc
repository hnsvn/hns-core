/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/cosmetic_filters/browser/cosmetic_filters_resources.h"

#include <utility>

#include "base/json/json_reader.h"
#include "base/values.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace cosmetic_filters {

CosmeticFiltersResources::CosmeticFiltersResources(
    hns_shields::AdBlockService* ad_block_service)
    : ad_block_service_(ad_block_service) {}

CosmeticFiltersResources::~CosmeticFiltersResources() = default;

void CosmeticFiltersResources::HiddenClassIdSelectors(
    const std::string& input,
    const std::vector<std::string>& exceptions,
    HiddenClassIdSelectorsCallback callback) {
  DCHECK(ad_block_service_->GetTaskRunner()->RunsTasksInCurrentSequence());
  absl::optional<base::Value> input_value = base::JSONReader::Read(input);
  if (!input_value) {
    // Nothing to work with
    std::move(callback).Run(base::Value::Dict());

    return;
  }
  base::Value::Dict* input_dict = input_value->GetIfDict();
  if (!input_dict) {
    std::move(callback).Run(base::Value::Dict());
    return;
  }
  std::vector<std::string> classes;
  base::Value::List* classes_list = input_dict->FindList("classes");
  if (classes_list) {
    for (const auto& class_item : *classes_list) {
      if (!class_item.is_string()) {
        continue;
      }
      classes.push_back(class_item.GetString());
    }
  }
  std::vector<std::string> ids;
  base::Value::List* ids_list = input_dict->FindList("ids");
  if (ids_list) {
    for (const auto& id_item : *ids_list) {
      if (!id_item.is_string()) {
        continue;
      }
      ids.push_back(id_item.GetString());
    }
  }

  auto selectors =
      ad_block_service_->HiddenClassIdSelectors(classes, ids, exceptions);

  std::move(callback).Run(std::move(selectors));
}

void CosmeticFiltersResources::UrlCosmeticResources(
    const std::string& url,
    bool aggressive_blocking,
    UrlCosmeticResourcesCallback callback) {
  DCHECK(ad_block_service_->GetTaskRunner()->RunsTasksInCurrentSequence());
  auto resources =
      ad_block_service_->UrlCosmeticResources(url, aggressive_blocking);
  std::move(callback).Run(base::Value(std::move(resources)));
}

}  // namespace cosmetic_filters
