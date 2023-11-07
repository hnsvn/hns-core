/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <sstream>

#define HNS_ZYGOTE_HOST_IMPL_LINUX_ZYGOTE_HOST_IMPL_INIT             \
  << "No usable sandbox! You probably need to enable user namespaces " \
     "in your kernel. See https://hns.com/linux for more info.";     \
  std::stringstream ss;                                                \
  ss

#include "src/content/browser/zygote_host/zygote_host_impl_linux.cc"
#undef HNS_ZYGOTE_HOST_IMPL_LINUX_ZYGOTE_HOST_IMPL_INIT
