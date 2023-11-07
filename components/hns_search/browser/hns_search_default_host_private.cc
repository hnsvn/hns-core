// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/components/hns_search/browser/hns_search_default_host_private.h"

#include <utility>

namespace hns_search {

HnsSearchDefaultHostPrivate::~HnsSearchDefaultHostPrivate() = default;

void HnsSearchDefaultHostPrivate::SetCanAlwaysSetDefault() {}

void HnsSearchDefaultHostPrivate::GetCanSetDefaultSearchProvider(
    GetCanSetDefaultSearchProviderCallback callback) {
  std::move(callback).Run(false);
}

void HnsSearchDefaultHostPrivate::SetIsDefaultSearchProvider() {}

}  // namespace hns_search
