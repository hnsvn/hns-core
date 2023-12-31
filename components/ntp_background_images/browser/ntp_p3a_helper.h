/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_NTP_P3A_HELPER_H_
#define HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_NTP_P3A_HELPER_H_

#include <string>

#include "url/gurl.h"

namespace ntp_background_images {

class NTPP3AHelper {
 public:
  virtual ~NTPP3AHelper() {}

  virtual void RecordView(const std::string& creative_instance_id) = 0;

  virtual void RecordClickAndMaybeLand(
      const std::string& creative_instance_id) = 0;

  virtual void SetLastTabURL(const GURL& url) = 0;
};

}  // namespace ntp_background_images

#endif  // HNS_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_NTP_P3A_HELPER_H_
