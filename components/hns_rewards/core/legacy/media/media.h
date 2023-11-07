/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_MEDIA_MEDIA_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_MEDIA_MEDIA_H_

#include <string>

#include "base/containers/flat_map.h"
#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/legacy/media/github.h"
#include "hns/components/hns_rewards/core/legacy/media/youtube.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {

class RewardsEngineImpl;

class Media {
 public:
  explicit Media(RewardsEngineImpl& engine);

  ~Media();

  static std::string GetLinkType(const std::string& url,
                                 const std::string& first_party_url,
                                 const std::string& referrer);

  void ProcessMedia(const base::flat_map<std::string, std::string>& parts,
                    const std::string& type,
                    mojom::VisitDataPtr visit_data);

  void GetMediaActivityFromUrl(uint64_t windowId,
                               mojom::VisitDataPtr visit_data,
                               const std::string& type,
                               const std::string& publisher_blob);

  void SaveMediaInfo(const std::string& type,
                     const base::flat_map<std::string, std::string>& data,
                     PublisherInfoCallback callback);

  static std::string GetShareURL(
      const std::string& type,
      const base::flat_map<std::string, std::string>& args);

 private:
  void OnMediaActivityError(mojom::VisitDataPtr visit_data,
                            const std::string& type,
                            uint64_t windowId);

  const raw_ref<RewardsEngineImpl> engine_;
  YouTube media_youtube_;
  GitHub media_github_;
};

}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_MEDIA_MEDIA_H_
