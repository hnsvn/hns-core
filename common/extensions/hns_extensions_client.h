// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMMON_EXTENSIONS_HNS_EXTENSIONS_CLIENT_H_
#define HNS_COMMON_EXTENSIONS_HNS_EXTENSIONS_CLIENT_H_

#include "chrome/common/extensions/chrome_extensions_client.h"

namespace extensions {

class HnsExtensionsClient : public ChromeExtensionsClient {
 public:
  HnsExtensionsClient();
  HnsExtensionsClient(const HnsExtensionsClient&) = delete;
  HnsExtensionsClient& operator=(const HnsExtensionsClient&) = delete;

  void InitializeWebStoreUrls(base::CommandLine* command_line) override;
  const GURL& GetWebstoreUpdateURL() const override;

 private:
  GURL webstore_update_url_;
};

}  // namespace extensions

#endif  // HNS_COMMON_EXTENSIONS_HNS_EXTENSIONS_CLIENT_H_
