/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/component_updater/configurator_impl.h"

namespace component_updater {

namespace {

class HnsConfiguratorImpl : public ConfiguratorImpl {
 public:
  using ConfiguratorImpl::ConfiguratorImpl;
  bool EnabledCupSigning() const;
};

bool HnsConfiguratorImpl::EnabledCupSigning() const {
  return false;
}

}  // namespace

}  // namespace component_updater

#define ConfiguratorImpl HnsConfiguratorImpl
#include "src/ios/chrome/browser/component_updater/ios_component_updater_configurator.mm"
#undef ConfiguratorImpl
