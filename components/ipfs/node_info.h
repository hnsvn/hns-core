/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_IPFS_NODE_INFO_H_
#define HNS_COMPONENTS_IPFS_NODE_INFO_H_

#include <string>
#include <vector>

namespace ipfs {

struct NodeInfo {
  NodeInfo();
  ~NodeInfo();

  std::string id;
  std::string version;
};

}  // namespace ipfs

#endif  // HNS_COMPONENTS_IPFS_NODE_INFO_H_