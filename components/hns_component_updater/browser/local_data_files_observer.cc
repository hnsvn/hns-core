/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_component_updater/browser/local_data_files_observer.h"

#include "hns/components/hns_component_updater/browser/local_data_files_service.h"

namespace hns_component_updater {

LocalDataFilesObserver::LocalDataFilesObserver(
    LocalDataFilesService* local_data_files_service)
    : local_data_files_service_(local_data_files_service) {
  local_data_files_observer_.Observe(local_data_files_service);
}

LocalDataFilesObserver::~LocalDataFilesObserver() = default;

void LocalDataFilesObserver::OnLocalDataFilesServiceDestroyed() {
  local_data_files_observer_.Reset();
  local_data_files_service_ = nullptr;
}

LocalDataFilesService* LocalDataFilesObserver::local_data_files_service() {
  return local_data_files_service_;
}

}  // namespace hns_component_updater

