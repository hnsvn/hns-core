/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/l10n/common/test/scoped_default_locale.h"

#include "hns/components/l10n/common/default_locale.h"

namespace hns_l10n::test {

ScopedDefaultLocale::ScopedDefaultLocale(const std::string& locale) {
  last_locale_ = MutableDefaultLocaleStringForTesting();
  MutableDefaultLocaleStringForTesting() = locale;
}

ScopedDefaultLocale::~ScopedDefaultLocale() {
  MutableDefaultLocaleStringForTesting() = last_locale_;
}

}  // namespace hns_l10n::test
