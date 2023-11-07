/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_ADS_NOTIFICATION_AD_H_
#define HNS_BROWSER_UI_HNS_ADS_NOTIFICATION_AD_H_

#include <string>
#include <utility>

#include "base/memory/ref_counted.h"
#include "hns/browser/ui/hns_ads/notification_ad_delegate.h"

namespace hns_ads {

class NotificationAd {
 public:
  // Create a new notification ad with an |id|, |title| text and |body| text.
  // |delegate| will influence the behavior of this notification ad and receives
  // events on its behalf. The delegate may be omitted
  NotificationAd(const std::string& id,
                 const std::u16string& title,
                 const std::u16string& body,
                 scoped_refptr<NotificationAdDelegate> delegate);

  // Creates a copy of the |other| notification ad. The delegate, if any, will
  // be identical for both notification ad instances. The |id| of the ad
  // notification will be replaced by the given value
  NotificationAd(const std::string& id, const NotificationAd&);

  // Creates a copy of the |other| notification ad. The delegate will be
  // replaced by |delegate|
  NotificationAd(scoped_refptr<NotificationAdDelegate> delegate,
                 const NotificationAd&);

  // Creates a copy of the |other| notification ad. The delegate, if any, will
  // be identical for both notification ad instances
  NotificationAd(const NotificationAd&);

  NotificationAd& operator=(const NotificationAd&);

  virtual ~NotificationAd();

  const std::string& id() const { return id_; }

  const std::u16string& title() const { return title_; }
  void set_title(const std::u16string& title) { title_ = title; }

  const std::u16string& body() const { return body_; }
  void set_body(const std::u16string& body) { body_ = body; }

  std::u16string accessible_name() const;

  NotificationAdDelegate* delegate() const { return delegate_.get(); }

  void set_delegate(scoped_refptr<NotificationAdDelegate> delegate) {
    CHECK(!delegate_);
    delegate_ = std::move(delegate);
  }

 protected:
  std::string id_;

  std::u16string title_;
  std::u16string body_;

 private:
  // A proxy object that allows access back to the JavaScript object that
  // represents the notification, for firing events
  scoped_refptr<NotificationAdDelegate> delegate_;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_UI_HNS_ADS_NOTIFICATION_AD_H_
