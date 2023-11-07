/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/page_info/chrome_page_info_delegate.h"

#include "hns/components/content_settings/core/browser/hns_content_settings_utils.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/url_constants.h"

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/ipfs_constants.h"
#include "hns/components/ipfs/ipfs_utils.h"

namespace chrome {
std::string GetIPFSLearnMoreURL(const GURL& url) {
  if (ipfs::IsIPFSScheme(url))
    return ipfs::kIPFSLearnMoreURL;
  return std::string(chrome::kPageInfoHelpCenterURL);
}
}  // namespace chrome

#define kPageInfoHelpCenterURL GetIPFSLearnMoreURL(web_contents_->GetURL())
#endif  // BUILDFLAG(ENABLE_IPFS)

#include "src/chrome/browser/ui/page_info/chrome_page_info_delegate.cc"
#if BUILDFLAG(ENABLE_IPFS)
#undef kPageInfoHelpCenterURL
#endif  // BUILDFLAG(ENABLE_IPFS)

bool ChromePageInfoDelegate::HnsShouldShowPermission(
    ContentSettingsType type) {
  if ((content_settings::IsShieldsContentSettingsType(type) ||
       type == ContentSettingsType::GEOLOCATION) &&
      GetProfile()->IsTor()) {
    return false;
  }
  return true;
}
