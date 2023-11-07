/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_PLAYLIST_COMMON_FEATURES_H_
#define HNS_COMPONENTS_PLAYLIST_COMMON_FEATURES_H_

#include "base/feature_list.h"

namespace playlist::features {

BASE_DECLARE_FEATURE(kPlaylist);

BASE_DECLARE_FEATURE(kPlaylistFakeUA);

}  // namespace playlist::features

#endif  // HNS_COMPONENTS_PLAYLIST_COMMON_FEATURES_H_
