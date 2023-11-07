/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/profile_resetter/hns_profile_resetter.h"

#include "hns/browser/search_engines/search_engine_provider_util.h"

HnsProfileResetter::~HnsProfileResetter() = default;

void HnsProfileResetter::ResetDefaultSearchEngine() {
  ProfileResetter::ResetDefaultSearchEngine();

  if (template_url_service_->loaded()) {
    hns::ResetDefaultPrivateSearchProvider(profile_);
  }
}
