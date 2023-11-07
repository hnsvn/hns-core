/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_NET_PROXY_CONFIG_MONITOR_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_NET_PROXY_CONFIG_MONITOR_H_

#define HNS_PROXY_CONFIG_MONITOR_H \
  net::ProxyConfigService* GetProxyConfigServiceForTesting() \
    { return proxy_config_service_.get(); }
#include "src/chrome/browser/net/proxy_config_monitor.h"  // IWYU pragma: export
#undef HNS_PROXY_CONFIG_MONITOR_H

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_NET_PROXY_CONFIG_MONITOR_H_
