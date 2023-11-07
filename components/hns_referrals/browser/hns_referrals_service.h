/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REFERRALS_BROWSER_HNS_REFERRALS_SERVICE_H_
#define HNS_COMPONENTS_HNS_REFERRALS_BROWSER_HNS_REFERRALS_SERVICE_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/no_destructor.h"
#include "base/scoped_observation.h"
#include "base/task/sequenced_task_runner.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "build/build_config.h"
#include "url/gurl.h"

#if BUILDFLAG(IS_ANDROID)
#include "hns/components/hns_referrals/browser/android_hns_referrer.h"
#include "hns/components/safetynet/safetynet_check.h"
#endif

class PrefRegistrySimple;
class PrefService;

namespace network {
namespace mojom {
class URLLoaderFactory;
}  // namespace mojom
class SimpleURLLoader;
}  // namespace network

namespace hns {

class HnsReferralsHeaders {
 public:
  static HnsReferralsHeaders* GetInstance() {
    static base::NoDestructor<HnsReferralsHeaders> instance;
    return instance.get();
  }

  template <typename Iter>
  bool GetMatchingReferralHeaders(
      const Iter& referral_headers_list,
      const base::Value::Dict** request_headers_dict,
      const GURL& url);

  bool GetMatchingReferralHeaders(
      const base::Value::Dict** request_headers_dict,
      const GURL& url);

  HnsReferralsHeaders(const HnsReferralsHeaders&) = delete;
  HnsReferralsHeaders& operator=(const HnsReferralsHeaders&) = delete;
  ~HnsReferralsHeaders() = delete;

 private:
  friend class base::NoDestructor<HnsReferralsHeaders>;
  HnsReferralsHeaders();

  std::vector<base::Value::Dict> referral_headers_;
};

class HnsReferralsService {
 public:
  class Delegate {
   public:
    Delegate() = default;
    virtual ~Delegate() = default;

    virtual void OnInitialized() = 0;
    virtual base::FilePath GetUserDataDirectory() = 0;
    virtual network::mojom::URLLoaderFactory* GetURLLoaderFactory() = 0;

#if !BUILDFLAG(IS_ANDROID)
    virtual base::OnceCallback<base::Time()>
    GetFirstRunSentinelCreationTimeCallback() = 0;
#endif
  };

  HnsReferralsService(PrefService* pref_service,
                        const std::string& platform,
                        const std::string& api_key);
  HnsReferralsService(const HnsReferralsService&) = delete;
  HnsReferralsService& operator=(const HnsReferralsService&) = delete;
  ~HnsReferralsService();

  void set_delegate(std::unique_ptr<Delegate> delegate) {
    delegate_ = std::move(delegate);
  }

  void Start();
  void Stop();

  using ReferralInitializedCallback =
      base::RepeatingCallback<void(const std::string& download_id)>;

  static void SetReferralInitializedCallbackForTesting(
      ReferralInitializedCallback* referral_initialized_callback);

  static void SetPromoFilePathForTesting(const base::FilePath& path);

  static bool IsDefaultReferralCode(const std::string& code);

 private:
  void GetFirstRunTime();
  void SetFirstRunTime(const base::Time& first_run_timestamp);
  void PerformFinalizationChecks();
  base::FilePath GetPromoCodeFileName() const;
  void MaybeCheckForReferralFinalization();
  void MaybeDeletePromoCodePref() const;
  void InitReferral();
  std::string BuildReferralInitPayload() const;
  std::string BuildReferralFinalizationCheckPayload() const;
  void CheckForReferralFinalization();

  // Invoked from RepeatingTimer when finalization checks timer
  // fires.
  void OnFinalizationChecksTimerFired();

  // Invoked from SimpleURLLoader after referral init load
  // completes.
  void OnReferralInitLoadComplete(std::unique_ptr<std::string> response_body);

  // Invoked from SimpleURLLoader after referral finalization check
  // load completes.
  void OnReferralFinalizationCheckLoadComplete(
      std::unique_ptr<std::string> response_body);

  // Invoked after reading contents of promo code file.
  void OnReadPromoCodeComplete(const std::string& promo_code);

#if BUILDFLAG(IS_ANDROID)
  void GetSafetynetStatusResult(const bool token_received,
                                const std::string& result_string,
                                const bool attestation_passed);
  safetynet_check::SafetyNetCheckRunner safetynet_check_runner_;

  void InitAndroidReferrer();
  void OnAndroidHnsReferrerReady();
  android_hns_referrer::HnsReferrer android_hns_referrer_;
#endif

  bool initialized_ = false;
  base::Time first_run_timestamp_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  std::unique_ptr<network::SimpleURLLoader> referral_init_loader_;
  std::unique_ptr<network::SimpleURLLoader> referral_finalization_check_loader_;
  std::unique_ptr<base::OneShotTimer> initialization_timer_;
  std::unique_ptr<base::RepeatingTimer> finalization_checks_timer_;
  ReferralInitializedCallback referral_initialized_callback_;
  std::unique_ptr<Delegate> delegate_;
  raw_ptr<PrefService> pref_service_ = nullptr;
  const std::string api_key_;
  const std::string platform_;
  std::string promo_code_;

  base::WeakPtrFactory<HnsReferralsService> weak_factory_{this};
};

// Registers the preferences used by HnsReferralsService
void RegisterPrefsForHnsReferralsService(PrefRegistrySimple* registry);

}  // namespace hns

#endif  // HNS_COMPONENTS_HNS_REFERRALS_BROWSER_HNS_REFERRALS_SERVICE_H_
