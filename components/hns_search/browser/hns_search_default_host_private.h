// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_DEFAULT_HOST_PRIVATE_H_
#define HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_DEFAULT_HOST_PRIVATE_H_

#include "hns/components/hns_search/common/hns_search_default.mojom.h"

class TemplateURLService;
class PrefService;

namespace hns_search {

class HnsSearchDefaultHostPrivate final
    : public hns_search::mojom::HnsSearchDefault {
 public:
  HnsSearchDefaultHostPrivate(const HnsSearchDefaultHostPrivate&) = delete;
  HnsSearchDefaultHostPrivate& operator=(
      const HnsSearchDefaultHostPrivate&) = delete;

  HnsSearchDefaultHostPrivate() = default;
  ~HnsSearchDefaultHostPrivate() override;

  // hns_search::mojom::HnsSearchDefault:
  void SetCanAlwaysSetDefault() override;
  void GetCanSetDefaultSearchProvider(
      GetCanSetDefaultSearchProviderCallback callback) override;
  void SetIsDefaultSearchProvider() override;
};

}  // namespace hns_search

#endif  // HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_DEFAULT_HOST_PRIVATE_H_
