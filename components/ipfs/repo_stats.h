/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_IPFS_REPO_STATS_H_
#define HNS_COMPONENTS_IPFS_REPO_STATS_H_

#include <string>
#include <vector>

namespace ipfs {

struct RepoStats {
  RepoStats();
  ~RepoStats();

  uint64_t objects = 0;
  uint64_t size = 0;
  uint64_t storage_max = 0;

  std::string path;
  std::string version;
};

}  // namespace ipfs

#endif  // HNS_COMPONENTS_IPFS_REPO_STATS_H_
