/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/autocomplete/hns_autocomplete_scheme_classifier.h"

#include <string>

#include "base/strings/string_util.h"
#include "hns/components/constants/url_constants.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"

#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
#include "hns/components/hns_webtorrent/browser/webtorrent_util.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/ipfs_constants.h"
#endif

HnsAutocompleteSchemeClassifier::HnsAutocompleteSchemeClassifier(
    Profile* profile)
    : ChromeAutocompleteSchemeClassifier(profile) {
#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
  profile_ = profile;
#endif
}

HnsAutocompleteSchemeClassifier::~HnsAutocompleteSchemeClassifier() =
    default;

// Without this override, typing in hns:// URLs will search Google
metrics::OmniboxInputType
HnsAutocompleteSchemeClassifier::GetInputTypeForScheme(
    const std::string& scheme) const {
  if (scheme.empty()) {
    return metrics::OmniboxInputType::EMPTY;
  }
  if (base::IsStringASCII(scheme) &&
      base::EqualsCaseInsensitiveASCII(scheme, kHnsUIScheme)) {
    return metrics::OmniboxInputType::URL;
  }

#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
  if (base::IsStringASCII(scheme) &&
      webtorrent::IsWebtorrentPrefEnabled(profile_) &&
      base::EqualsCaseInsensitiveASCII(scheme, kMagnetScheme)) {
    return metrics::OmniboxInputType::URL;
  }
#endif

#if BUILDFLAG(ENABLE_IPFS)
  if (base::IsStringASCII(scheme) &&
      (base::EqualsCaseInsensitiveASCII(scheme, ipfs::kIPFSScheme) ||
       base::EqualsCaseInsensitiveASCII(scheme, ipfs::kIPNSScheme))) {
    return metrics::OmniboxInputType::URL;
  }
#endif


  return ChromeAutocompleteSchemeClassifier::GetInputTypeForScheme(scheme);
}
