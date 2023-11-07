/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_COMPONENT_UPDATER_DELEGATE_H_
#define HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_COMPONENT_UPDATER_DELEGATE_H_

#include <string>

#include "base/functional/callback.h"
#include "base/memory/raw_ref.h"
#include "hns/components/hns_component_updater/browser/hns_component.h"

using hns_component_updater::HnsComponent;

namespace base {
class SequencedTaskRunner;
}

namespace component_updater {
class ComponentUpdateService;
}

class PrefService;

namespace hns {

class HnsComponentUpdaterDelegate : public HnsComponent::Delegate {
 public:
  HnsComponentUpdaterDelegate(
      component_updater::ComponentUpdateService* updater,
      PrefService* local_state,
      const std::string& locale);
  HnsComponentUpdaterDelegate(const HnsComponentUpdaterDelegate&) = delete;
  HnsComponentUpdaterDelegate& operator=(
      const HnsComponentUpdaterDelegate&) = delete;
  ~HnsComponentUpdaterDelegate() override;

  using ComponentObserver = update_client::UpdateClient::Observer;
  // hns_component_updater::HnsComponent::Delegate implementation
  void Register(const std::string& component_name,
                const std::string& component_base64_public_key,
                base::OnceClosure registered_callback,
                HnsComponent::ReadyCallback ready_callback) override;
  bool Unregister(const std::string& component_id) override;
  void OnDemandUpdate(const std::string& component_id) override;

  void AddObserver(ComponentObserver* observer) override;
  void RemoveObserver(ComponentObserver* observer) override;

  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner() override;

  const std::string& locale() const override;
  PrefService* local_state() override;

 private:
  const raw_ref<component_updater::ComponentUpdateService> component_updater_;
  const raw_ref<PrefService> local_state_;
  std::string locale_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
};

}  // namespace hns

#endif  // HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_COMPONENT_UPDATER_DELEGATE_H_
