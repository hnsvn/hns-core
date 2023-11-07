// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_LOCALHOST_FILTERS_PROVIDER_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_LOCALHOST_FILTERS_PROVIDER_H_

#include <string>

#include "base/functional/callback.h"
#include "base/memory/raw_ptr.h"
#include "base/sequence_checker.h"
#include "hns/components/hns_component_updater/browser/dat_file_util.h"
#include "hns/components/hns_shields/browser/ad_block_filters_provider.h"

using hns_component_updater::DATFileDataBuffer;

namespace hns_shields {

class AdBlockLocalhostFiltersProvider : public AdBlockFiltersProvider {
 public:
  AdBlockLocalhostFiltersProvider();
  ~AdBlockLocalhostFiltersProvider() override;
  AdBlockLocalhostFiltersProvider(const AdBlockLocalhostFiltersProvider&) =
      delete;
  AdBlockLocalhostFiltersProvider& operator=(
      const AdBlockLocalhostFiltersProvider&) = delete;

  std::string GetLocalhostFilters();
  bool UpdateLocalhostFilters(const std::string& localhost_filters);

  void LoadDATBuffer(
      base::OnceCallback<void(bool deserialize,
                              const DATFileDataBuffer& dat_buf)>) override;

  // AdBlockFiltersProvider
  void AddObserver(AdBlockFiltersProvider::Observer* observer);

  std::string GetNameForDebugging() override;

 private:
  SEQUENCE_CHECKER(sequence_checker_);
};

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_LOCALHOST_FILTERS_PROVIDER_H_
