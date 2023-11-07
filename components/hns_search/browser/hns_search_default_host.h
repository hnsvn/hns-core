// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_DEFAULT_HOST_H_
#define HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_DEFAULT_HOST_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_search/common/hns_search_default.mojom.h"

class TemplateURLService;
class TemplateURL;
class PrefService;
class PrefRegistrySimple;

namespace hns_search {

class HnsSearchDefaultHost final
    : public hns_search::mojom::HnsSearchDefault {
 public:
  static void RegisterProfilePrefs(PrefRegistrySimple* registry);

  HnsSearchDefaultHost(const HnsSearchDefaultHost&) = delete;
  HnsSearchDefaultHost& operator=(const HnsSearchDefaultHost&) = delete;

  HnsSearchDefaultHost(const std::string& host,
                         TemplateURLService* template_url_service,
                         PrefService* prefs);
  ~HnsSearchDefaultHost() override;

  // hns_search::mojom::HnsSearchDefault:
  void SetCanAlwaysSetDefault() override;
  void GetCanSetDefaultSearchProvider(
      GetCanSetDefaultSearchProviderCallback callback) override;
  void SetIsDefaultSearchProvider() override;

 private:
  bool CanSetDefaultSearchProvider(TemplateURL* provider, bool is_historic);
  uint64_t GetMaxDailyCanAskCount();
  uint64_t GetMaxTotalCanAskCount();

  bool can_always_set_default_ = false;
  bool can_set_default_ = false;
  const std::string host_;
  raw_ptr<TemplateURLService> template_url_service_ = nullptr;
  raw_ptr<PrefService> prefs_ = nullptr;
};

}  // namespace hns_search

#endif  // HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_DEFAULT_HOST_H_
