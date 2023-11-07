/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_COMPONENT_INSTALLER_H_
#define HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_COMPONENT_INSTALLER_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/functional/callback.h"
#include "base/values.h"
#include "hns/components/hns_component_updater/browser/hns_component.h"
#include "components/component_updater/component_installer.h"
#include "components/update_client/update_client.h"

using hns_component_updater::HnsComponent;

namespace hns {

class HnsComponentInstallerPolicy
    : public component_updater::ComponentInstallerPolicy {
 public:
  explicit HnsComponentInstallerPolicy(
      const std::string& name,
      const std::string& base64_public_key,
      HnsComponent::ReadyCallback ready_callback);

  HnsComponentInstallerPolicy(const HnsComponentInstallerPolicy&) = delete;
  HnsComponentInstallerPolicy& operator=(
      const HnsComponentInstallerPolicy&) = delete;

  ~HnsComponentInstallerPolicy() override;

 private:
  // The following methods override ComponentInstallerPolicy
  bool VerifyInstallation(const base::Value::Dict& manifest,
                          const base::FilePath& install_dir) const override;
  bool SupportsGroupPolicyEnabledComponentUpdates() const override;
  bool RequiresNetworkEncryption() const override;
  update_client::CrxInstaller::Result OnCustomInstall(
      const base::Value::Dict& manifest,
      const base::FilePath& install_dir) override;
  void OnCustomUninstall() override;
  void ComponentReady(const base::Version& version,
                      const base::FilePath& install_dir,
                      base::Value::Dict manifest) override;
  base::FilePath GetRelativeInstallDir() const override;
  void GetHash(std::vector<uint8_t>* hash) const override;
  std::string GetName() const override;
  update_client::InstallerAttributes GetInstallerAttributes() const override;

  std::string name_;
  std::string base64_public_key_;
  std::string public_key_;
  HnsComponent::ReadyCallback ready_callback_;
};

void RegisterComponent(component_updater::ComponentUpdateService* cus,
                       const std::string& name,
                       const std::string& base64_public_key,
                       base::OnceClosure registered_callback,
                       HnsComponent::ReadyCallback ready_callback);

}  // namespace hns

#endif  // HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_COMPONENT_INSTALLER_H_
