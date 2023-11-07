/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_OMNIBOX_CLIENT_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_OMNIBOX_CLIENT_H_

#include "components/omnibox/browser/omnibox_client.h"

class HnsOmniboxClient : public OmniboxClient {
 public:
  virtual bool IsAutocompleteEnabled() const;

 protected:
  ~HnsOmniboxClient() override {}
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_OMNIBOX_CLIENT_H_
