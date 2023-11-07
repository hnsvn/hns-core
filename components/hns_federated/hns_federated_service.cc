/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_federated/hns_federated_service.h"

#include <memory>
#include <utility>

#include "base/logging.h"
#include "base/path_service.h"
#include "hns/components/hns_federated/data_store_service.h"
#include "hns/components/hns_federated/data_stores/data_store.h"
#include "hns/components/hns_federated/eligibility_service.h"
#include "hns/components/hns_federated/features.h"
#include "hns/components/hns_federated/operational_patterns.h"
#include "hns/components/p3a/pref_names.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace hns_federated {

HnsFederatedService::HnsFederatedService(
    PrefService* prefs,
    PrefService* local_state,
    const base::FilePath& browser_context_path,
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory)
    : prefs_(prefs),
      local_state_(local_state),
      url_loader_factory_(url_loader_factory) {
  Init(browser_context_path);
}

HnsFederatedService::~HnsFederatedService() = default;

void HnsFederatedService::RegisterProfilePrefs(PrefRegistrySimple* registry) {
  OperationalPatterns::RegisterPrefs(registry);
}

DataStoreService* HnsFederatedService::GetDataStoreService() const {
  DCHECK(data_store_service_);
  return data_store_service_.get();
}

///////////////////////////////////////////////////////////////////////////////

void HnsFederatedService::Init(const base::FilePath& browser_context_path) {
  VLOG(1) << "Initialising federated service";

  local_state_change_registrar_.Init(local_state_);
  local_state_change_registrar_.Add(
      p3a::kP3AEnabled,
      base::BindRepeating(&HnsFederatedService::OnPreferenceChanged,
                          base::Unretained(this)));

  base::FilePath db_path(browser_context_path.AppendASCII("data_store.sqlite"));
  data_store_service_ = std::make_unique<DataStoreService>(db_path);
  data_store_service_->Init();

  eligibility_service_ = std::make_unique<EligibilityService>();

  operational_patterns_ =
      std::make_unique<OperationalPatterns>(prefs_, url_loader_factory_);

  MaybeStartOperationalPatterns();
}

void HnsFederatedService::OnPreferenceChanged(const std::string& pref_name) {
  if (pref_name == p3a::kP3AEnabled) {
    MaybeStartOrStopOperationalPatterns();
  }
}

bool HnsFederatedService::IsFederatedLearningEnabled() {
  return hns_federated::features::IsFederatedLearningEnabled();
}

bool HnsFederatedService::IsOperationalPatternsEnabled() {
  return hns_federated::features::IsOperationalPatternsEnabled();
}

bool HnsFederatedService::IsP3AEnabled() {
  return local_state_->GetBoolean(p3a::kP3AEnabled);
}

bool HnsFederatedService::ShouldStartOperationalPatterns() {
  return IsP3AEnabled() && IsOperationalPatternsEnabled();
}

void HnsFederatedService::MaybeStartOrStopOperationalPatterns() {
  MaybeStartOperationalPatterns();
  MaybeStopOperationalPatterns();
}

void HnsFederatedService::MaybeStartOperationalPatterns() {
  DCHECK(operational_patterns_);
  if (!operational_patterns_->IsRunning() && ShouldStartOperationalPatterns()) {
    operational_patterns_->Start();
  }
}

void HnsFederatedService::MaybeStopOperationalPatterns() {
  DCHECK(operational_patterns_);
  if (operational_patterns_->IsRunning() && !ShouldStartOperationalPatterns()) {
    operational_patterns_->Stop();
  }
}

}  // namespace hns_federated
