/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_TEST_FILTERS_PROVIDER_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_TEST_FILTERS_PROVIDER_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/functional/callback.h"
#include "hns/components/hns_component_updater/browser/dat_file_util.h"
#include "hns/components/hns_shields/browser/ad_block_filters_provider.h"
#include "hns/components/hns_shields/browser/ad_block_resource_provider.h"

using hns_component_updater::DATFileDataBuffer;

namespace hns_shields {

class TestFiltersProvider : public AdBlockFiltersProvider,
                            public AdBlockResourceProvider {
 public:
  TestFiltersProvider(const std::string& rules, const std::string& resources);
  TestFiltersProvider(const base::FilePath& dat_location,
                      const std::string& resources);
  ~TestFiltersProvider() override;

  void LoadDATBuffer(
      base::OnceCallback<void(bool deserialize,
                              const DATFileDataBuffer& dat_buf)> cb) override;

  void LoadResources(
      base::OnceCallback<void(const std::string& resources_json)> cb) override;

  std::string GetNameForDebugging() override;

 private:
  DATFileDataBuffer dat_buffer_;
  std::string rules_;
  std::string resources_;
};

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_TEST_FILTERS_PROVIDER_H_
