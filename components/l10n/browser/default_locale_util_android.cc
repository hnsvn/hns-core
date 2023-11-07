/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/l10n/browser/default_locale_util.h"

#include "base/android/locale_utils.h"

namespace hns_l10n {

absl::optional<std::string> MaybeGetDefaultLocaleString() {
  return base::android::GetDefaultLocaleString();
}

}  // namespace hns_l10n
