/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_
#define HNS_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_

#include <memory>

#include "base/scoped_observation.h"
#include "hns/browser/search_engines/private_window_search_engine_provider_service_base.h"
#include "components/search_engines/template_url_service.h"
#include "components/search_engines/template_url_service_observer.h"

class PrefService;
struct TemplateURLData;

// The purpose of this service for tor is making user changed search engine
// provider persist across the sessions.
// Also, HnsProfileManager::SetNonPersonalProfilePrefs() overrides for it.
class TorWindowSearchEngineProviderService
    : public PrivateWindowSearchEngineProviderServiceBase,
      public TemplateURLServiceObserver {
 public:
  explicit TorWindowSearchEngineProviderService(Profile* otr_profile);
  ~TorWindowSearchEngineProviderService() override;

  TorWindowSearchEngineProviderService(
      const TorWindowSearchEngineProviderService&) = delete;
  TorWindowSearchEngineProviderService& operator=(
      const TorWindowSearchEngineProviderService&) = delete;

  // PrivateWindowSearchEngineProviderServiceBase overrides:
  void Initialize() override;
  void Shutdown() override;

  // TemplateURLServiceObserver overrides:
  void OnTemplateURLServiceChanged() override;

 private:
  void ConfigureSearchEngineProvider();

  std::unique_ptr<TemplateURL> default_template_url_for_tor_;
  base::ScopedObservation<TemplateURLService, TemplateURLServiceObserver>
      observation_{this};
};

#endif  // HNS_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_
