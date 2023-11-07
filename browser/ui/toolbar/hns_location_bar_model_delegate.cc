/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/toolbar/hns_location_bar_model_delegate.h"

#include "base/strings/utf_string_conversions.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/components/constants/url_constants.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/ipfs/ipfs_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#endif

HnsLocationBarModelDelegate::HnsLocationBarModelDelegate(Browser* browser)
    : BrowserLocationBarModelDelegate(browser) {}

HnsLocationBarModelDelegate::~HnsLocationBarModelDelegate() = default;

// static
void HnsLocationBarModelDelegate::FormattedStringFromURL(
    const GURL& url,
    std::u16string* new_formatted_url) {
  if (url.SchemeIs("chrome")) {
    base::ReplaceFirstSubstringAfterOffset(new_formatted_url, 0, u"chrome://",
                                           u"hns://");
  }

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
  if (url.SchemeIs(kChromeExtensionScheme) &&
      url.host() == ethereum_remote_client_extension_id) {
    base::ReplaceFirstSubstringAfterOffset(
        new_formatted_url, 0,
        base::UTF8ToUTF16(ethereum_remote_client_base_url), u"hns://wallet");
    base::ReplaceFirstSubstringAfterOffset(
        new_formatted_url, 0,
        base::UTF8ToUTF16(ethereum_remote_client_phishing_url),
        u"hns://wallet");
    base::ReplaceFirstSubstringAfterOffset(
        new_formatted_url, 0,
        base::UTF8ToUTF16(ethereum_remote_client_ens_redirect_url),
        u"hns://wallet");
  }
#endif
}

std::u16string
HnsLocationBarModelDelegate::FormattedStringWithEquivalentMeaning(
    const GURL& url,
    const std::u16string& formatted_url) const {
  std::u16string new_formatted_url =
      BrowserLocationBarModelDelegate::FormattedStringWithEquivalentMeaning(
          url, formatted_url);
  HnsLocationBarModelDelegate::FormattedStringFromURL(url,
                                                        &new_formatted_url);
  return new_formatted_url;
}
