// Copyright 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/notifications/ads_notification_handler.h"

#define HNS_ADD_HNS_ADS_NOTIFICATION_HANDLER \
  AddNotificationHandler(                        \
      NotificationHandler::Type::HNS_ADS,      \
      std::make_unique<hns_ads::AdsNotificationHandler>(*profile));
#include "src/chrome/browser/notifications/notification_display_service_impl.cc"
#undef HNS_ADD_HNS_ADS_NOTIFICATION_HANDLER
