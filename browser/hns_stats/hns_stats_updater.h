/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_STATS_HNS_STATS_UPDATER_H_
#define HNS_BROWSER_HNS_STATS_HNS_STATS_UPDATER_H_

#include <memory>
#include <string>

#include "base/functional/callback.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "hns/components/hns_stats/browser/hns_stats_updater_util.h"
#include "chrome/browser/profiles/profile_manager_observer.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "url/gurl.h"

class HnsStatsUpdaterBrowserTest;
class PrefChangeRegistrar;
class PrefRegistrySimple;
class PrefService;
class ProfileManager;

namespace base {
class OneShotTimer;
class RepeatingTimer;
}  // namespace base

namespace net {
class HttpResponseHeaders;
}

namespace network {
class SimpleURLLoader;
}

namespace misc_metrics {
class GeneralBrowserUsage;
}

namespace hns_stats {

extern const char kP3AMonthlyPingHistogramName[];
extern const char kP3ADailyPingHistogramName[];

class HnsStatsUpdaterParams;

class HnsStatsUpdater : public ProfileManagerObserver {
 public:
  HnsStatsUpdater(PrefService* pref_service, ProfileManager* profile_manager);
  HnsStatsUpdater(const HnsStatsUpdater&) = delete;
  HnsStatsUpdater& operator=(const HnsStatsUpdater&) = delete;
  ~HnsStatsUpdater() override;

  void Start();
  void Stop();
  bool MaybeDoThresholdPing(int score);

  using StatsUpdatedCallback = base::RepeatingCallback<void(const GURL& url)>;

  static void SetStatsUpdatedCallbackForTesting(
      StatsUpdatedCallback* stats_updated_callback);
  static void SetStatsThresholdCallbackForTesting(
      StatsUpdatedCallback* stats_threshold_callback);
  void SetURLLoaderFactoryForTesting(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);
  void SetUsageServerForTesting(const std::string& usage_server);

 private:
  void RecordP3APing();
  GURL BuildStatsEndpoint(const std::string& path);
  void OnThresholdLoaderComplete(scoped_refptr<net::HttpResponseHeaders>);
  // Invoked from SimpleURLLoader after download is complete.
  void OnSimpleLoaderComplete(
      std::unique_ptr<hns_stats::HnsStatsUpdaterParams>
          stats_updater_params,
      scoped_refptr<net::HttpResponseHeaders> headers);

  // Invoked when server ping timer fires.
  void OnServerPingTimerFired();

  // Invoked after browser has initialized with referral server.
  void OnReferralInitialization();

  // Invoked after hns ads initializes
  void OnDetectUncertainFuture(const bool is_uncertain_future);

  void DetectUncertainFuture();
  void StartServerPingStartupTimer();
  void QueueServerPing();
  void SendUserTriggeredPing();
  void SendServerPing();

  bool IsAdsEnabled();
  bool IsReferralInitialized();
  bool HasDoneThresholdPing();
  void DisableThresholdPing();

  // ProfileManagerObserver:
  void OnProfileAdded(Profile* profile) override;

  network::mojom::URLLoaderFactory* GetURLLoaderFactory();

  friend class ::HnsStatsUpdaterBrowserTest;

  int threshold_score_ = 0;
  ProcessArch arch_ = ProcessArch::kArchSkip;
  bool stats_startup_complete_ = false;
  raw_ptr<PrefService> pref_service_ = nullptr;
  raw_ptr<ProfileManager> profile_manager_ = nullptr;
  std::string usage_server_;
  std::unique_ptr<network::SimpleURLLoader> simple_url_loader_;
  std::unique_ptr<base::OneShotTimer> server_ping_startup_timer_;
  std::unique_ptr<base::RepeatingTimer> server_ping_periodic_timer_;
  std::unique_ptr<PrefChangeRegistrar> pref_change_registrar_;
  base::RepeatingClosure stats_preconditions_barrier_;

  scoped_refptr<network::SharedURLLoaderFactory> testing_url_loader_factory_;

  std::unique_ptr<misc_metrics::GeneralBrowserUsage> general_browser_usage_p3a_;
  base::WeakPtrFactory<HnsStatsUpdater> weak_ptr_factory_{this};
};

// Registers the preferences used by HnsStatsUpdater
void RegisterLocalStatePrefs(PrefRegistrySimple* registry);

}  // namespace hns_stats

#endif  // HNS_BROWSER_HNS_STATS_HNS_STATS_UPDATER_H_
