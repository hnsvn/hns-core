// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_HNS_NEWS_HNS_NEWS_CONTROLLER_FACTORY_H_
#define HNS_BROWSER_HNS_NEWS_HNS_NEWS_CONTROLLER_FACTORY_H_

#include "hns/components/hns_news/common/hns_news.mojom.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"
#include "content/public/browser/browser_context.h"
#include "mojo/public/cpp/bindings/pending_remote.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace content {
class BrowserContext;
}

namespace hns_news {

class HnsNewsController;

class HnsNewsControllerFactory : public BrowserContextKeyedServiceFactory {
 public:
  static HnsNewsController* GetForContext(content::BrowserContext* context);
  static mojo::PendingRemote<mojom::HnsNewsController> GetRemoteService(
      content::BrowserContext* context);
  static HnsNewsController* GetControllerForContext(
      content::BrowserContext* context);
  static HnsNewsControllerFactory* GetInstance();

  HnsNewsControllerFactory(const HnsNewsControllerFactory&) = delete;
  HnsNewsControllerFactory& operator=(const HnsNewsControllerFactory&) =
      delete;

 private:
  friend base::NoDestructor<HnsNewsControllerFactory>;

  HnsNewsControllerFactory();
  ~HnsNewsControllerFactory() override;

  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
};

}  // namespace hns_news

#endif  // HNS_BROWSER_HNS_NEWS_HNS_NEWS_CONTROLLER_FACTORY_H_
