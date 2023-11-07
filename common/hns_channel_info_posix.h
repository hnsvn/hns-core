/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMMON_HNS_CHANNEL_INFO_POSIX_H_
#define HNS_COMMON_HNS_CHANNEL_INFO_POSIX_H_

#include <string>

#include "components/version_info/version_info.h"

namespace hns {

version_info::Channel GetChannelImpl(std::string* modifier_out,
                                     std::string* data_dir_suffix_out);

}

#endif  // HNS_COMMON_HNS_CHANNEL_INFO_POSIX_H_

