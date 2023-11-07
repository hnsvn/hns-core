/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/browser/ads_p2a.h"

#include <cstdint>

#include "base/containers/contains.h"
#include "base/logging.h"
#include "base/metrics/histogram_functions.h"
#include "base/strings/strcat.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/time_period_storage/weekly_storage.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace hns_ads {

namespace {

constexpr const char* kAllowedEvents[] = {
    "Hns.P2A.TotalAdOpportunities",
    "Hns.P2A.AdOpportunitiesPerSegment.architecture",
    "Hns.P2A.AdOpportunitiesPerSegment.artsentertainment",
    "Hns.P2A.AdOpportunitiesPerSegment.automotive",
    "Hns.P2A.AdOpportunitiesPerSegment.business",
    "Hns.P2A.AdOpportunitiesPerSegment.careers",
    "Hns.P2A.AdOpportunitiesPerSegment.cellphones",
    "Hns.P2A.AdOpportunitiesPerSegment.crypto",
    "Hns.P2A.AdOpportunitiesPerSegment.education",
    "Hns.P2A.AdOpportunitiesPerSegment.familyparenting",
    "Hns.P2A.AdOpportunitiesPerSegment.fashion",
    "Hns.P2A.AdOpportunitiesPerSegment.folklore",
    "Hns.P2A.AdOpportunitiesPerSegment.fooddrink",
    "Hns.P2A.AdOpportunitiesPerSegment.gaming",
    "Hns.P2A.AdOpportunitiesPerSegment.healthfitness",
    "Hns.P2A.AdOpportunitiesPerSegment.history",
    "Hns.P2A.AdOpportunitiesPerSegment.hobbiesinterests",
    "Hns.P2A.AdOpportunitiesPerSegment.home",
    "Hns.P2A.AdOpportunitiesPerSegment.law",
    "Hns.P2A.AdOpportunitiesPerSegment.military",
    "Hns.P2A.AdOpportunitiesPerSegment.other",
    "Hns.P2A.AdOpportunitiesPerSegment.personalfinance",
    "Hns.P2A.AdOpportunitiesPerSegment.pets",
    "Hns.P2A.AdOpportunitiesPerSegment.realestate",
    "Hns.P2A.AdOpportunitiesPerSegment.science",
    "Hns.P2A.AdOpportunitiesPerSegment.sports",
    "Hns.P2A.AdOpportunitiesPerSegment.technologycomputing",
    "Hns.P2A.AdOpportunitiesPerSegment.travel",
    "Hns.P2A.AdOpportunitiesPerSegment.weather",
    "Hns.P2A.AdOpportunitiesPerSegment.untargeted"};

constexpr size_t kIntervalBuckets[] = {0, 5, 10, 20, 50, 100, 250, 500};

void EmitP2AHistogramName(const std::string& name, uint16_t sum) {
  CHECK(base::Contains(kAllowedEvents, name));

  const size_t* const iter =
      std::lower_bound(kIntervalBuckets, std::end(kIntervalBuckets), sum);
  const size_t bucket = iter - kIntervalBuckets;

  base::UmaHistogramExactLinear(name, static_cast<int>(bucket),
                                std::size(kIntervalBuckets) + 1);
}

}  // namespace

void RegisterP2APrefs(PrefRegistrySimple* registry) {
  for (const char* const event : kAllowedEvents) {
    const std::string pref_path =
        base::StrCat({prefs::kP2AStoragePrefNamePrefix, event});
    registry->RegisterListPref(pref_path);
  }
}

void RecordInWeeklyStorageAndEmitP2AHistogramName(PrefService* prefs,
                                                  const std::string& name) {
  CHECK(prefs);

  if (!base::Contains(kAllowedEvents, name)) {
    return;
  }

  const std::string pref_path =
      base::StrCat({prefs::kP2AStoragePrefNamePrefix, name});
  if (!prefs->FindPreference(pref_path)) {
    return;
  }

  WeeklyStorage storage(prefs, pref_path.c_str());
  storage.AddDelta(1);

  EmitP2AHistogramName(name, storage.GetWeeklySum());
}

void SuspendP2AHistograms() {
  // Record "special value" to prevent sending this week's data to P2A server.
  // Matches INT_MAX - 1 for |kSuspendedMetricValue| in |hns_p3a_service.cc|.
  for (const char* const event : kAllowedEvents) {
    base::UmaHistogramExactLinear(event, INT_MAX,
                                  std::size(kIntervalBuckets) + 1);
  }

  VLOG(1) << "P2A histograms suspended";
}

}  // namespace hns_ads
