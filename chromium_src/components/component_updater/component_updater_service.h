/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_H_

class IPFSDOMHandler;

namespace chrome {
namespace android {
class HnsComponentUpdaterAndroid;
}
}  // namespace chrome

#define HNS_COMPONENT_UPDATER_SERVICE_H_ \
  friend class ::IPFSDOMHandler;           \
  friend class ::chrome::android::HnsComponentUpdaterAndroid;

#define HNS_COMPONENT_UPDATER_SERVICE_H_ON_DEMAND_UPDATER \
 private:                                                   \
  friend void HnsOnDemandUpdate(const std::string&);      \
                                                            \
 public:
#include "src/components/component_updater/component_updater_service.h"  // IWYU pragma: export

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_H_
