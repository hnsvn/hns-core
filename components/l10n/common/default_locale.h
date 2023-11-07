/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_L10N_COMMON_DEFAULT_LOCALE_H_
#define HNS_COMPONENTS_L10N_COMMON_DEFAULT_LOCALE_H_

#include <string>

namespace hns_l10n {

const std::string& DefaultLocaleString();
std::string& MutableDefaultLocaleStringForTesting();

}  // namespace hns_l10n

#endif  // HNS_COMPONENTS_L10N_COMMON_DEFAULT_LOCALE_H_
