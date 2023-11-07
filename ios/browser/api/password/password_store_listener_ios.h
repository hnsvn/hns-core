/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import <Foundation/Foundation.h>

#ifndef HNS_IOS_BROWSER_API_PASSWORD_PASSWORD_STORE_LISTENER_IOS_H_
#define HNS_IOS_BROWSER_API_PASSWORD_PASSWORD_STORE_LISTENER_IOS_H_

#include <vector>

#include "hns/ios/browser/api/password/hns_password_observer.h"

#include "components/password_manager/core/browser/password_form.h"
#include "components/password_manager/core/browser/password_store_interface.h"

@interface PasswordStoreListenerImpl : NSObject <PasswordStoreListener>
- (instancetype)init:(id<PasswordStoreObserver>)observer
       passwordStore:
           (scoped_refptr<password_manager::PasswordStoreInterface>)store;
@end

namespace hns {
namespace ios {

class PasswordStoreListenerIOS
    : public password_manager::PasswordStoreInterface::Observer {
 public:
  explicit PasswordStoreListenerIOS(
      id<PasswordStoreObserver> observer,
      scoped_refptr<password_manager::PasswordStoreInterface> store);
  ~PasswordStoreListenerIOS() override;

 private:
  // Called when the contents of the password store change.
  void OnLoginsChanged(
      password_manager::PasswordStoreInterface* store,
      const password_manager::PasswordStoreChangeList& changes) override;
  void OnLoginsRetained(password_manager::PasswordStoreInterface* store,
                        const std::vector<password_manager::PasswordForm>&
                            retained_passwords) override;

  id<PasswordStoreObserver> observer_;
  scoped_refptr<password_manager::PasswordStoreInterface> store_;
};

}  // namespace ios
}  // namespace hns

#endif  // HNS_IOS_BROWSER_API_PASSWORD_PASSWORD_STORE_LISTENER_IOS_H_
