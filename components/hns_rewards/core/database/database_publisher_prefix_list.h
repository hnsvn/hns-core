/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_PUBLISHER_PREFIX_LIST_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_PUBLISHER_PREFIX_LIST_H_

#include <string>

#include "hns/components/hns_rewards/core/database/database_table.h"
#include "hns/components/hns_rewards/core/publisher/prefix_list_reader.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_rewards::internal {
namespace database {

using SearchPublisherPrefixListCallback = std::function<void(bool)>;

class DatabasePublisherPrefixList : public DatabaseTable {
 public:
  explicit DatabasePublisherPrefixList(RewardsEngineImpl& engine);
  ~DatabasePublisherPrefixList() override;

  void Reset(publisher::PrefixListReader reader, LegacyResultCallback callback);

  void Search(const std::string& publisher_key,
              SearchPublisherPrefixListCallback callback);

 private:
  void InsertNext(publisher::PrefixIterator begin,
                  LegacyResultCallback callback);

  void OnSearch(SearchPublisherPrefixListCallback callback,
                mojom::DBCommandResponsePtr response);

  void OnInsertNext(LegacyResultCallback callback,
                    publisher::PrefixIterator iter,
                    mojom::DBCommandResponsePtr response);

  absl::optional<publisher::PrefixListReader> reader_;
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_PUBLISHER_PREFIX_LIST_H_
