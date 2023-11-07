/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_PROMOTION_PROVIDER_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_PROMOTION_PROVIDER_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "components/omnibox/browser/autocomplete_provider.h"

class AutocompleteProviderClient;
class PrefService;
class TemplateURLService;

class PromotionProvider : public AutocompleteProvider {
 public:
  explicit PromotionProvider(AutocompleteProviderClient* client);
  PromotionProvider(const PromotionProvider&) = delete;
  PromotionProvider& operator=(const PromotionProvider&) = delete;

  // AutocompleteProvider overrides:
  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~PromotionProvider() override;

  void AddMatchForHnsSearchPromotion(const std::u16string& input);

  raw_ptr<PrefService> prefs_ = nullptr;
  raw_ptr<TemplateURLService> template_url_service_ = nullptr;
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_PROMOTION_PROVIDER_H_
