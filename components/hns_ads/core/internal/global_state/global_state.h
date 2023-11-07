/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_GLOBAL_STATE_GLOBAL_STATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_GLOBAL_STATE_GLOBAL_STATE_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "base/sequence_checker.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

namespace hns_ads {

class AdsClient;
class BrowserManager;
class ClientStateManager;
class ConfirmationStateManager;
class DatabaseManager;
class DiagnosticManager;
class GlobalStateHolder;
class HistoryManager;
class NotificationAdManager;
class PredictorsManager;
class TabManager;
class UserActivityManager;

class GlobalState final {
 public:
  explicit GlobalState(AdsClient* ads_client);

  GlobalState(const GlobalState& other) = delete;
  GlobalState& operator=(const GlobalState& other) = delete;

  GlobalState(GlobalState&& other) noexcept = delete;
  GlobalState& operator=(GlobalState&& other) noexcept = delete;

  ~GlobalState();

  static GlobalState* GetInstance();

  static bool HasInstance();

  AdsClient* GetAdsClient();

  BrowserManager& GetBrowserManager();
  ClientStateManager& GetClientStateManager();
  ConfirmationStateManager& GetConfirmationStateManager();
  DatabaseManager& GetDatabaseManager();
  DiagnosticManager& GetDiagnosticManager();
  HistoryManager& GetHistoryManager();
  NotificationAdManager& GetNotificationAdManager();
  PredictorsManager& GetPredictorsManager();
  TabManager& GetTabManager();
  UserActivityManager& GetUserActivityManager();

  mojom::SysInfo& SysInfo();

  mojom::BuildChannelInfo& BuildChannel();

  mojom::Flags& Flags();

 private:
  SEQUENCE_CHECKER(sequence_checker_);

  const raw_ptr<AdsClient> ads_client_ = nullptr;

  const std::unique_ptr<GlobalStateHolder> global_state_holder_;

  std::unique_ptr<BrowserManager> browser_manager_;
  std::unique_ptr<ClientStateManager> client_state_manager_;
  std::unique_ptr<ConfirmationStateManager> confirmation_state_manager_;
  std::unique_ptr<DatabaseManager> database_manager_;
  std::unique_ptr<DiagnosticManager> diagnostic_manager_;
  std::unique_ptr<HistoryManager> history_manager_;
  std::unique_ptr<NotificationAdManager> notification_ad_manager_;
  std::unique_ptr<PredictorsManager> predictors_manager_;
  std::unique_ptr<TabManager> tab_manager_;
  std::unique_ptr<UserActivityManager> user_activity_manager_;

  mojom::SysInfo sys_info_;
  mojom::BuildChannelInfo build_channel_;
  mojom::Flags flags_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_GLOBAL_STATE_GLOBAL_STATE_H_
