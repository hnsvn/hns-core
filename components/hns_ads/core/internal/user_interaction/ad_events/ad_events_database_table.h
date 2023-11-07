/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENTS_DATABASE_TABLE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENTS_DATABASE_TABLE_H_

#include <string>

#include "base/functional/callback.h"
#include "hns/components/hns_ads/core/internal/database/database_table_interface.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "hns/components/hns_ads/core/public/client/ads_client_callback.h"

namespace hns_ads::database::table {

using GetAdEventsCallback =
    base::OnceCallback<void(bool success, const AdEventList& ad_events)>;

class AdEvents final : public TableInterface {
 public:
  void LogEvent(const AdEventInfo& ad_event, ResultCallback callback);

  void GetAll(GetAdEventsCallback callback) const;

  void GetForType(mojom::AdType ad_type, GetAdEventsCallback callback) const;

  void PurgeExpired(ResultCallback callback) const;
  void PurgeOrphaned(mojom::AdType ad_type, ResultCallback callback) const;

  std::string GetTableName() const override;

  void Create(mojom::DBTransactionInfo* transaction) override;
  void Migrate(mojom::DBTransactionInfo* transaction, int to_version) override;

 private:
  void InsertOrUpdate(mojom::DBTransactionInfo* transaction,
                      const AdEventList& ad_event);

  std::string BuildInsertOrUpdateSql(mojom::DBCommandInfo* command,
                                     const AdEventList& ad_events) const;
};

}  // namespace hns_ads::database::table

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENTS_DATABASE_TABLE_H_
