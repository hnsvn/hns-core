/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_OMNIBOX_HNS_OMNIBOX_CLIENT_IMPL_H_
#define HNS_BROWSER_UI_OMNIBOX_HNS_OMNIBOX_CLIENT_IMPL_H_

#include "base/memory/raw_ptr.h"
#include "hns/browser/autocomplete/hns_autocomplete_scheme_classifier.h"
#include "chrome/browser/ui/omnibox/chrome_omnibox_client.h"

class PrefRegistrySimple;
class Profile;

class HnsOmniboxClientImpl : public ChromeOmniboxClient {
 public:
  HnsOmniboxClientImpl(LocationBar* location_bar,
                         Browser* browser,
                         Profile* profile);
  HnsOmniboxClientImpl(const HnsOmniboxClientImpl&) = delete;
  HnsOmniboxClientImpl& operator=(const HnsOmniboxClientImpl&) = delete;
  ~HnsOmniboxClientImpl() override;

  static void RegisterProfilePrefs(PrefRegistrySimple* prefs);

  const AutocompleteSchemeClassifier& GetSchemeClassifier() const override;
  bool IsAutocompleteEnabled() const override;

  void OnInputAccepted(const AutocompleteMatch& match) override;
  void OnURLOpenedFromOmnibox(OmniboxLog* log) override;

 private:
  raw_ptr<Profile> profile_ = nullptr;
  HnsAutocompleteSchemeClassifier scheme_classifier_;
};

#endif  // HNS_BROWSER_UI_OMNIBOX_HNS_OMNIBOX_CLIENT_IMPL_H_
