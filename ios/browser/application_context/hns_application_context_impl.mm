// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/ios/browser/application_context/hns_application_context_impl.h"

#include <string>

#import "base/command_line.h"
#import "base/task/sequenced_task_runner.h"
#include "hns/components/hns_component_updater/browser/hns_component.h"
#include "hns/components/hns_component_updater/browser/hns_component_updater_delegate.h"
#include "hns/components/hns_component_updater/browser/local_data_files_service.h"
#include "hns/components/url_sanitizer/browser/url_sanitizer_component_installer.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"

HnsApplicationContextImpl::HnsApplicationContextImpl(
    base::SequencedTaskRunner* local_state_task_runner,
    const base::CommandLine& command_line,
    const std::string& locale,
    const std::string& country)
    : ApplicationContextImpl(local_state_task_runner,
                             command_line,
                             locale,
                             country) {}

inline HnsApplicationContextImpl::~HnsApplicationContextImpl() = default;

hns_component_updater::HnsComponent::Delegate*
HnsApplicationContextImpl::hns_component_updater_delegate() {
  if (!hns_component_updater_delegate_) {
    hns_component_updater_delegate_ =
        std::make_unique<hns::HnsComponentUpdaterDelegate>(
            GetComponentUpdateService(), GetLocalState(),
            GetApplicationLocale());
  }

  return hns_component_updater_delegate_.get();
}

hns_component_updater::LocalDataFilesService*
HnsApplicationContextImpl::local_data_files_service() {
  if (!local_data_files_service_) {
    local_data_files_service_ =
        hns_component_updater::LocalDataFilesServiceFactory(
            hns_component_updater_delegate());
  }
  return local_data_files_service_.get();
}

hns::URLSanitizerComponentInstaller*
HnsApplicationContextImpl::url_sanitizer_component_installer() {
  if (!url_sanitizer_component_installer_) {
    url_sanitizer_component_installer_ =
        std::make_unique<hns::URLSanitizerComponentInstaller>(
            local_data_files_service());
  }
  return url_sanitizer_component_installer_.get();
}

void HnsApplicationContextImpl::StartHnsServices() {
  // We need to Initialize the component installer
  // before calling Start on the local_data_files_service
  url_sanitizer_component_installer();

  // Start the local data file service
  local_data_files_service()->Start();
}
