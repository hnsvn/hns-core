/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/hns_extension_management.h"

#define HNS_EXTENSION_MANAGEMENT_FACTORY_BUILD_SERVICE_INSTANCE_FOR \
  return std::make_unique<HnsExtensionManagement>(                  \
      Profile::FromBrowserContext(context));

#include "src/chrome/browser/extensions/extension_management.cc"
#undef HNS_EXTENSION_MANAGEMENT_FACTORY_BUILD_SERVICE_INSTANCE_FOR
