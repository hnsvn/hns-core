/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_AD_TYPE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_AD_TYPE_H_

#include <ostream>
#include <string>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

class AdType final {
 public:
  enum Value {
    kUndefined,
    kNotificationAd,
    kNewTabPageAd,
    kPromotedContentAd,
    kInlineContentAd,
    kSearchResultAd
  };

  AdType();

  // Allow implicit conversion of the enum value to this wrapper
  constexpr AdType(const Value& value)  // NOLINT (runtime/explicit)
      : value_(value) {}

  explicit AdType(const std::string& value);
  explicit AdType(mojom::AdType value);

  Value value() const;
  std::string ToString() const;

 private:
  Value value_ = kUndefined;
};

bool operator==(const AdType&, const AdType&);
bool operator!=(const AdType&, const AdType&);

std::ostream& operator<<(std::ostream& os, const AdType& type);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_AD_TYPE_H_
