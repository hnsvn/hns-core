/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_NTP_TILES_HNS_POPULAR_SITES_IMPL_H_
#define HNS_COMPONENTS_NTP_TILES_HNS_POPULAR_SITES_IMPL_H_

#include <map>

#include "components/ntp_tiles/popular_sites_impl.h"

namespace ntp_tiles {

class HnsPopularSitesImpl : public PopularSitesImpl {
 public:
  using PopularSitesImpl::PopularSitesImpl;

  HnsPopularSitesImpl(const HnsPopularSitesImpl&) = delete;
  HnsPopularSitesImpl& operator=(const HnsPopularSitesImpl&) = delete;

  // PopularSitesImpl overrides:
  const std::map<SectionType, SitesVector>& sections() const override;
};

}  // namespace ntp_tiles

#endif  // HNS_COMPONENTS_NTP_TILES_HNS_POPULAR_SITES_IMPL_H_
