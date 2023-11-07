/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_IPFS_IPFS_JSON_PARSER_H_
#define HNS_COMPONENTS_IPFS_IPFS_JSON_PARSER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "base/values.h"
#include "hns/components/ipfs/addresses_config.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/ipfs/import/imported_data.h"
#include "hns/components/ipfs/node_info.h"
#include "hns/components/ipfs/repo_stats.h"

#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
#include "hns/components/ipfs/pin/ipfs_pin_rpc_types.h"
#endif  // BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)

class IPFSJSONParser {
 public:
  static bool GetPeersFromJSON(const base::Value& json_value,
                               std::vector<std::string>* peers);
  static bool GetAddressesConfigFromJSON(const base::Value& json_value,
                                         ipfs::AddressesConfig* config);
  static bool GetRepoStatsFromJSON(const base::Value& json_value,
                                   ipfs::RepoStats* config);
  static bool GetNodeInfoFromJSON(const base::Value& json_value,
                                  ipfs::NodeInfo* info);
  static bool GetGarbageCollectionFromJSON(const base::Value& json_value,
                                           std::string* error);
  static bool GetImportResponseFromJSON(const std::string& json,
                                        ipfs::ImportedData* data);
  static bool GetParseKeysFromJSON(
      const base::Value& json_value,
      std::unordered_map<std::string, std::string>* keys);
  static bool GetParseSingleKeyFromJSON(const std::string& json,
                                        std::string* name,
                                        std::string* value);
  static bool GetParseSingleKeyFromJSON(const base::Value& json_value,
                                        std::string* name,
                                        std::string* value);
  static bool GetPeersFromConfigJSON(const std::string& json,
                                     std::vector<std::string>* peers);
  static std::string PutNewPeerToConfigJSON(const std::string& json,
                                            const std::string& peer);
  static std::string RemovePeerFromConfigJSON(const std::string& json,
                                              const std::string& peer_id,
                                              const std::string& address);
#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
  // Local pins
  static absl::optional<ipfs::AddPinResult> GetAddPinsResultFromJSON(
      const base::Value& value);
  static absl::optional<ipfs::GetPinsResult> GetGetPinsResultFromJSON(
      const base::Value& value);
  static absl::optional<ipfs::RemovePinResult> GetRemovePinsResultFromJSON(
      const base::Value& value);
#endif  // BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
};

#endif  // HNS_COMPONENTS_IPFS_IPFS_JSON_PARSER_H_
