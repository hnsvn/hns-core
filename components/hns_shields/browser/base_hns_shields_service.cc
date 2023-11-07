/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_shields/browser/base_hns_shields_service.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "base/logging.h"
#include "base/memory/ptr_util.h"

namespace hns_shields {

BaseHnsShieldsService::BaseHnsShieldsService(
    scoped_refptr<base::SequencedTaskRunner> task_runner)
    : initialized_(false), task_runner_(task_runner) {}

BaseHnsShieldsService::~BaseHnsShieldsService() = default;

bool BaseHnsShieldsService::IsInitialized() const {
  return initialized_;
}

void BaseHnsShieldsService::InitShields() {
  if (Init()) {
    std::lock_guard<std::mutex> guard(initialized_mutex_);
    initialized_ = true;
  }
}

bool BaseHnsShieldsService::Start() {
  if (initialized_) {
    return true;
  }

  InitShields();
  return false;
}

void BaseHnsShieldsService::ShouldStartRequest(
    const GURL& url,
    blink::mojom::ResourceType resource_type,
    const std::string& tab_host,
    bool aggressive_blocking,
    bool* did_match_rule,
    bool* did_match_exception,
    bool* did_match_important,
    std::string* mock_data_url) {}

scoped_refptr<base::SequencedTaskRunner>
BaseHnsShieldsService::GetTaskRunner() {
  return task_runner_;
}

}  // namespace hns_shields
