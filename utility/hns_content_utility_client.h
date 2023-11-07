/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_UTILITY_HNS_CONTENT_UTILITY_CLIENT_H_
#define HNS_UTILITY_HNS_CONTENT_UTILITY_CLIENT_H_

#include <string>

#include "chrome/utility/chrome_content_utility_client.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

class HnsContentUtilityClient : public ChromeContentUtilityClient {
 public:
  HnsContentUtilityClient();
  HnsContentUtilityClient(const HnsContentUtilityClient&) = delete;
  HnsContentUtilityClient& operator=(const HnsContentUtilityClient&) =
      delete;
  ~HnsContentUtilityClient() override;

  // ChromeContentUtilityClient overrides:
  void RegisterMainThreadServices(mojo::ServiceFactory& services) override;
};

#endif  // HNS_UTILITY_HNS_CONTENT_UTILITY_CLIENT_H_
