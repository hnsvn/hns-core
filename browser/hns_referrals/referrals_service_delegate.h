/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_REFERRALS_REFERRALS_SERVICE_DELEGATE_H_
#define HNS_BROWSER_HNS_REFERRALS_REFERRALS_SERVICE_DELEGATE_H_

#include "hns/components/hns_referrals/browser/hns_referrals_service.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_manager_observer.h"

class ReferralsServiceDelegate : public hns::HnsReferralsService::Delegate,
                                 public ProfileManagerObserver {
 public:
  explicit ReferralsServiceDelegate(hns::HnsReferralsService* service);
  ~ReferralsServiceDelegate() override;

  // hns::HnsReferralsService::Delegate:
  void OnInitialized() override;
  base::FilePath GetUserDataDirectory() override;
  network::mojom::URLLoaderFactory* GetURLLoaderFactory() override;
#if !BUILDFLAG(IS_ANDROID)
  base::OnceCallback<base::Time()> GetFirstRunSentinelCreationTimeCallback()
      override;
#endif

  // ProfileManagerObserver:
  void OnProfileAdded(Profile* profile) override;

 private:
  raw_ptr<hns::HnsReferralsService> service_;  // owner

  base::ScopedObservation<ProfileManager, ProfileManagerObserver>
      profile_manager_observation_{this};
};

#endif  // HNS_BROWSER_HNS_REFERRALS_REFERRALS_SERVICE_DELEGATE_H_
