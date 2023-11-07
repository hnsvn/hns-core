/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_BOOKMARK_BOOKMARK_PREFS_SERVICE_FACTORY_H_
#define HNS_BROWSER_UI_BOOKMARK_BOOKMARK_PREFS_SERVICE_FACTORY_H_

#include "base/gtest_prod_util.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

class BookmarkPrefsService;

class BookmarkPrefsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  BookmarkPrefsServiceFactory(const BookmarkPrefsServiceFactory&) = delete;
  BookmarkPrefsServiceFactory& operator=(const BookmarkPrefsServiceFactory&) =
      delete;

  static BookmarkPrefsService* GetForBrowserContext(
      content::BrowserContext* context);

  static BookmarkPrefsServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<BookmarkPrefsServiceFactory>;
  FRIEND_TEST_ALL_PREFIXES(BookmarkStateUnittest, SetState);

  BookmarkPrefsServiceFactory();
  ~BookmarkPrefsServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;
  void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) override;
};

#endif  // HNS_BROWSER_UI_BOOKMARK_BOOKMARK_PREFS_SERVICE_FACTORY_H_
