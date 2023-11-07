// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <string>

#include "hns/components/hns_news/browser/network.h"

namespace hns_news {

net::NetworkTrafficAnnotationTag GetNetworkTrafficAnnotationTag() {
  return net::DefineNetworkTrafficAnnotation("hns_news_controller", R"(
      semantics {
        sender: "Hns News Controller"
        description:
          "This controller is used to fetch hns news feeds and publisher lists."
        trigger:
          "Triggered by uses of the Hns News feature."
        data:
          "Article JSON"
        destination: WEBSITE
      }
      policy {
        cookies_allowed: NO
        setting:
          "You can enable or disable this feature on the New Tab Page customization."
        policy_exception_justification:
          "Not implemented."
      }
    )");
}

}  // namespace hns_news
