/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_IPFS_IPFS_SERVICE_FACTORY_H_
#define HNS_BROWSER_IPFS_IPFS_SERVICE_FACTORY_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

class Profile;
namespace ipfs {
class IpfsService;

class IpfsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  IpfsServiceFactory(const IpfsServiceFactory&) = delete;
  IpfsServiceFactory& operator=(const IpfsServiceFactory&) = delete;

  static IpfsService* GetForContext(content::BrowserContext* context);
  static IpfsServiceFactory* GetInstance();
  static bool IsIpfsEnabled(content::BrowserContext* context);

 private:
  friend base::NoDestructor<IpfsServiceFactory>;

  IpfsServiceFactory();
  ~IpfsServiceFactory() override;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
};

}  // namespace ipfs

#endif  // HNS_BROWSER_IPFS_IPFS_SERVICE_FACTORY_H_
