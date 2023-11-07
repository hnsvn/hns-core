/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_component_updater/browser/hns_component_updater_delegate.h"

#include <utility>

#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "hns/components/hns_component_updater/browser/hns_component_installer.h"
#include "hns/components/hns_component_updater/browser/hns_on_demand_updater.h"
#include "components/component_updater/component_updater_service.h"
#include "components/prefs/pref_service.h"

using hns_component_updater::HnsComponent;
using hns_component_updater::HnsOnDemandUpdater;
using component_updater::ComponentUpdateService;

namespace hns {

HnsComponentUpdaterDelegate::HnsComponentUpdaterDelegate(
    ComponentUpdateService* component_updater,
    PrefService* local_state,
    const std::string& locale)
    : component_updater_(
          raw_ref<ComponentUpdateService>::from_ptr(component_updater)),
      local_state_(raw_ref<PrefService>::from_ptr(local_state)),
      locale_(locale),
      task_runner_(base::ThreadPool::CreateSequencedTaskRunner(
          {base::MayBlock(), base::TaskPriority::USER_BLOCKING,
           base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN})) {}

HnsComponentUpdaterDelegate::~HnsComponentUpdaterDelegate() = default;

void HnsComponentUpdaterDelegate::Register(
    const std::string& component_name,
    const std::string& component_base64_public_key,
    base::OnceClosure registered_callback,
    HnsComponent::ReadyCallback ready_callback) {
  hns::RegisterComponent(base::to_address(component_updater_), component_name,
                           component_base64_public_key,
                           std::move(registered_callback),
                           std::move(ready_callback));
}

bool HnsComponentUpdaterDelegate::Unregister(
    const std::string& component_id) {
  return component_updater_->UnregisterComponent(component_id);
}

void HnsComponentUpdaterDelegate::OnDemandUpdate(
    const std::string& component_id) {
  HnsOnDemandUpdater::GetInstance()->OnDemandUpdate(component_id);
}

void HnsComponentUpdaterDelegate::AddObserver(ComponentObserver* observer) {
  component_updater_->AddObserver(observer);
}

void HnsComponentUpdaterDelegate::RemoveObserver(
    ComponentObserver* observer) {
  component_updater_->RemoveObserver(observer);
}

scoped_refptr<base::SequencedTaskRunner>
HnsComponentUpdaterDelegate::GetTaskRunner() {
  return task_runner_;
}

const std::string& HnsComponentUpdaterDelegate::locale() const {
  return locale_;
}

PrefService* HnsComponentUpdaterDelegate::local_state() {
  return base::to_address(local_state_);
}

}  // namespace hns
