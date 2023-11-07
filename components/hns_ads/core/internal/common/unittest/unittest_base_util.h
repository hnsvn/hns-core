/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_BASE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_BASE_UTIL_H_

#include "base/files/scoped_temp_dir.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_mock.h"
#include "hns/components/hns_ads/core/public/database/database.h"

namespace hns_ads {

void MockFlags();

void MockShowNotificationAd(AdsClientMock& mock);
void MockCloseNotificationAd(AdsClientMock& mock);

void MockRecordAdEventForId(const AdsClientMock& mock);
void MockGetAdEventHistory(const AdsClientMock& mock);
void MockResetAdEventHistoryForId(const AdsClientMock& mock);

void MockSave(AdsClientMock& mock);
void MockLoad(AdsClientMock& mock, const base::ScopedTempDir& temp_dir);
void MockLoadFileResource(AdsClientMock& mock,
                          const base::ScopedTempDir& temp_dir);
void MockLoadDataResource(AdsClientMock& mock);

void MockRunDBTransaction(AdsClientMock& mock, Database& database);

void MockGetBooleanPref(const AdsClientMock& mock);
void MockGetIntegerPref(const AdsClientMock& mock);
void MockGetDoublePref(const AdsClientMock& mock);
void MockGetStringPref(const AdsClientMock& mock);
void MockGetInt64Pref(const AdsClientMock& mock);
void MockGetUint64Pref(const AdsClientMock& mock);
void MockGetTimePref(const AdsClientMock& mock);
void MockGetDictPref(const AdsClientMock& mock);
void MockGetListPref(const AdsClientMock& mock);
void MockClearPref(AdsClientMock& mock);
void MockHasPrefPath(const AdsClientMock& mock);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_BASE_UTIL_H_
