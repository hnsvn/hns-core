/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_component_updater/browser/hns_component.h"

#include <utility>

#include "base/functional/bind.h"
#include "base/logging.h"
#include "base/task/sequenced_task_runner.h"

namespace hns_component_updater {

HnsComponent::HnsComponent(Delegate* delegate)
    : delegate_(delegate),
      weak_factory_(this) {}

HnsComponent::~HnsComponent() = default;

void HnsComponent::Register(const std::string& component_name,
                              const std::string& component_id,
                              const std::string& component_base64_public_key) {
  VLOG(2) << "register component: " << component_id;
  component_name_ = component_name;
  component_id_ = component_id;
  component_base64_public_key_ = component_base64_public_key;

  auto registered_callback =
      base::BindOnce(&HnsComponent::OnComponentRegistered,
                     delegate_,
                     component_id);
  auto ready_callback =
      base::BindRepeating(&HnsComponent::OnComponentReadyInternal,
                          weak_factory_.GetWeakPtr(),
                          component_id);

  delegate_->Register(component_name_,
                      component_base64_public_key_,
                      std::move(registered_callback),
                      ready_callback);
}

bool HnsComponent::Unregister() {
  VLOG(2) << "unregister component: " << component_id_;
  return delegate_->Unregister(component_id_);
}

scoped_refptr<base::SequencedTaskRunner> HnsComponent::GetTaskRunner() {
  return delegate_->GetTaskRunner();
}

void HnsComponent::AddObserver(ComponentObserver* observer) {
  DCHECK(delegate_);
  delegate_->AddObserver(observer);
}

void HnsComponent::RemoveObserver(ComponentObserver* observer) {
  DCHECK(delegate_);
  delegate_->RemoveObserver(observer);
}

void HnsComponent::OnComponentReadyInternal(
    const std::string& component_id,
    const base::FilePath& install_dir,
    const std::string& manifest) {
  VLOG(2) << "component ready: " << manifest;
  OnComponentReady(component_id, install_dir, manifest);
}

void HnsComponent::OnComponentReady(
    const std::string& component_id,
    const base::FilePath& install_dir,
    const std::string& manifest) {}

// static
void HnsComponent::OnComponentRegistered(
    Delegate* delegate,
    const std::string& component_id) {
  VLOG(2) << "component registered: " << component_id;
  delegate->OnDemandUpdate(component_id);
}

HnsComponent::Delegate* HnsComponent::delegate() {
  return delegate_;
}

}  // namespace hns_component_updater
