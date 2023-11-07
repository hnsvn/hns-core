// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_PLAYLIST_BROWSER_PREF_NAMES_H_
#define HNS_COMPONENTS_PLAYLIST_BROWSER_PREF_NAMES_H_

namespace playlist {

// Set of playlists. Each playlist has ids of its items
// so that playlists can share same item efficiently
// Currently, List type preference always has to be updated entirely but there
// are many cases where we only need update small part of it.
// Thus, in order to update playlists efficiently, this pref is in Dictionary
//
// e.g. {
//        "list1": {name: "playlist1", items: [id1, id2, id3]},
//        "list2": {name: "playlist2", items: [id1, id4, id5] }
//      }                                       ^ same item
constexpr char kPlaylistsPref[] = "hns.playlist.lists";

// Stores playlist item key-value pairs in a dict. Each item has unique key and
// it's metadata(such as, title, media file path and etc..).
constexpr char kPlaylistItemsPref[] = "hns.playlist.items";

// Boolean pref indicates that we should cache media file when adding items.
constexpr char kPlaylistCacheByDefault[] = "hns.playlist.cache";

// A string indicates to which playlist items should be added by default.
constexpr char kPlaylistDefaultSaveTargetListID[] =
    "hns.playlist.default_save_target_list_id";

// Timestamp indicating first usage of playlist
constexpr char kPlaylistFirstUsageTime[] = "hns.playlist.first_usage_time";

// Timestamp indicating last usage of playlist
constexpr char kPlaylistLastUsageTime[] = "hns.playlist.last_usage_time";

// Boolean indicating if playlist was used on second day
// on first week of usage
constexpr char kPlaylistUsedSecondDay[] = "hns.playlist.used_second_day";

// Weekly storage event list counting usages of playlist
constexpr char kPlaylistUsageWeeklyStorage[] =
    "hns.playlist.usage_weekly_storage";

}  // namespace playlist

#endif  // HNS_COMPONENTS_PLAYLIST_BROWSER_PREF_NAMES_H_
