/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_AUTOCOMPLETE_HNS_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_
#define HNS_BROWSER_AUTOCOMPLETE_HNS_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_

#include <string>

#include "hns/components/hns_webtorrent/browser/buildflags/buildflags.h"
#include "chrome/browser/autocomplete/chrome_autocomplete_scheme_classifier.h"

#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
#include "base/memory/raw_ptr.h"
#endif

class HnsAutocompleteSchemeClassifier
    : public ChromeAutocompleteSchemeClassifier {
 public:
  explicit HnsAutocompleteSchemeClassifier(Profile* profile);
  HnsAutocompleteSchemeClassifier(const HnsAutocompleteSchemeClassifier&) =
      delete;
  HnsAutocompleteSchemeClassifier& operator=(
      const HnsAutocompleteSchemeClassifier&) = delete;
  ~HnsAutocompleteSchemeClassifier() override;

  metrics::OmniboxInputType GetInputTypeForScheme(
      const std::string& scheme) const override;

 private:
#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
  raw_ptr<Profile> profile_ = nullptr;
#endif
};

#endif  // HNS_BROWSER_AUTOCOMPLETE_HNS_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_

