// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_IOS_BROWSER_APPLICATION_CONTEXT_HNS_APPLICATION_CONTEXT_IMPL_H_
#define HNS_IOS_BROWSER_APPLICATION_CONTEXT_HNS_APPLICATION_CONTEXT_IMPL_H_

#include <memory>
#include <string>

#include "hns/components/hns_component_updater/browser/hns_component.h"
#include "hns/components/url_sanitizer/browser/url_sanitizer_component_installer.h"
#include "ios/chrome/browser/application_context/model/application_context_impl.h"

namespace base {
class CommandLine;
class SequencedTaskRunner;
}  // namespace base

/// This extends the behaviors of the ApplicationContext
class HnsApplicationContextImpl : public ApplicationContextImpl {
 public:
  // Out-of-line constructor declaration
  HnsApplicationContextImpl(
      base::SequencedTaskRunner* local_state_task_runner,
      const base::CommandLine& command_line,
      const std::string& locale,
      const std::string& country);

  hns::URLSanitizerComponentInstaller* url_sanitizer_component_installer();

  // Disable copy constructor and assignment operator
  HnsApplicationContextImpl(const HnsApplicationContextImpl&) = delete;
  HnsApplicationContextImpl& operator=(const HnsApplicationContextImpl&) =
      delete;

  // Start any services that we may need later
  void StartHnsServices();

  // Out-of-line destructor declaration
  ~HnsApplicationContextImpl() override;

 private:
  hns_component_updater::HnsComponent::Delegate*
  hns_component_updater_delegate();
  hns_component_updater::LocalDataFilesService* local_data_files_service();

  std::unique_ptr<hns_component_updater::HnsComponent::Delegate>
      hns_component_updater_delegate_;
  std::unique_ptr<hns_component_updater::LocalDataFilesService>
      local_data_files_service_;
  std::unique_ptr<hns::URLSanitizerComponentInstaller>
      url_sanitizer_component_installer_;
};

#endif  // HNS_IOS_BROWSER_APPLICATION_CONTEXT_HNS_APPLICATION_CONTEXT_IMPL_H_
