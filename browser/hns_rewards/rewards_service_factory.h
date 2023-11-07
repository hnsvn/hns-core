/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_REWARDS_REWARDS_SERVICE_FACTORY_H_
#define HNS_BROWSER_HNS_REWARDS_REWARDS_SERVICE_FACTORY_H_

#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile_manager_observer.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_rewards {
class RewardsService;

// Singleton that owns all RewardsService and associates them with Profiles.
class RewardsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  RewardsServiceFactory(const RewardsServiceFactory&) = delete;
  RewardsServiceFactory& operator=(const RewardsServiceFactory&) = delete;

  static hns_rewards::RewardsService* GetForProfile(Profile* profile);

  static RewardsServiceFactory* GetInstance();

  static void SetServiceForTesting(RewardsService* service);

 private:
  friend base::NoDestructor<RewardsServiceFactory>;

  RewardsServiceFactory();
  ~RewardsServiceFactory() override = default;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  bool ServiceIsNULLWhileTesting() const override;
};

}  // namespace hns_rewards

#endif  // HNS_BROWSER_HNS_REWARDS_REWARDS_SERVICE_FACTORY_H_
