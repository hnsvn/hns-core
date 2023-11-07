/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/url/request_builder/host/hosts/anonymous_url_host.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

namespace hns_ads {

namespace {

constexpr char kProductionHost[] = "https://anonymous.ads.hns.com";
constexpr char kStagingHost[] = "https://anonymous.ads.hnssoftware.com";

}  // namespace

std::string AnonymousUrlHost::Get() const {
  const mojom::EnvironmentType environment_type =
      GlobalState::GetInstance()->Flags().environment_type;
  CHECK(mojom::IsKnownEnumValue(environment_type));

  switch (environment_type) {
    case mojom::EnvironmentType::kProduction: {
      return kProductionHost;
    }

    case mojom::EnvironmentType::kStaging: {
      return kStagingHost;
    }
  }
}

}  // namespace hns_ads
