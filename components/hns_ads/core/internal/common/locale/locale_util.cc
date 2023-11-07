/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/locale/locale_util.h"

#include "hns/components/l10n/common/locale_util.h"

namespace hns_ads {

const std::string& GetLocale() {
  return hns_l10n::GetDefaultLocaleString();
}

}  // namespace hns_ads
