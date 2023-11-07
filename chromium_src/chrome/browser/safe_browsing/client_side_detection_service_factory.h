/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_SERVICE_FACTORY_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_SERVICE_FACTORY_H_

#define ClientSideDetectionServiceFactory \
  ClientSideDetectionServiceFactory_ChromiumImpl
#include "src/chrome/browser/safe_browsing/client_side_detection_service_factory.h"  // IWYU pragma: export
#undef ClientSideDetectionServiceFactory

namespace safe_browsing {

class ClientSideDetectionServiceFactory {
 public:
  static ClientSideDetectionService* GetForProfile(Profile* profile);

  // Get the singleton instance.
  static ClientSideDetectionServiceFactory* GetInstance();
};

}  // namespace safe_browsing

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_SERVICE_FACTORY_H_
