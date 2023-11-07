// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_SPEEDREADER_COMMON_URL_READABLE_HINTS_H_
#define HNS_COMPONENTS_SPEEDREADER_COMMON_URL_READABLE_HINTS_H_

class GURL;

namespace speedreader {

// Helper function for testing URLs against precompiled regexes.
bool IsURLLooksReadable(const GURL& url);

}  // namespace speedreader

#endif  // HNS_COMPONENTS_SPEEDREADER_COMMON_URL_READABLE_HINTS_H_
