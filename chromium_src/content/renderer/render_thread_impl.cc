/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define REGISTER_HNS_SCHEMES_DISPLAY_ISOLATED_AND_NO_JS                \
  WebString hns_scheme(WebString::FromASCII(kHnsUIScheme));          \
  WebSecurityPolicy::RegisterURLSchemeAsDisplayIsolated(hns_scheme);   \
  WebSecurityPolicy::RegisterURLSchemeAsNotAllowingJavascriptURLs(       \
      hns_scheme);                                                     \
                                                                         \
  WebString ipfs_scheme(WebString::FromASCII("ipfs"));                   \
  WebSecurityPolicy::RegisterURLSchemeAsSupportingFetchAPI(ipfs_scheme); \
                                                                         \
  WebString ipns_scheme(WebString::FromASCII("ipns"));                   \
  WebSecurityPolicy::RegisterURLSchemeAsSupportingFetchAPI(ipns_scheme);

#include "src/content/renderer/render_thread_impl.cc"
