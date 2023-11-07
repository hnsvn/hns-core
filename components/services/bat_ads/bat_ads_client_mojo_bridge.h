/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SERVICES_BAT_ADS_BAT_ADS_CLIENT_MOJO_BRIDGE_H_
#define HNS_COMPONENTS_SERVICES_BAT_ADS_BAT_ADS_CLIENT_MOJO_BRIDGE_H_

#include <cstdint>
#include <string>
#include <vector>

#include "base/values.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "hns/components/hns_ads/core/public/client/ads_client.h"
#include "hns/components/hns_federated/public/interfaces/hns_federated.mojom-forward.h"
#include "hns/components/services/bat_ads/bat_ads_client_notifier_impl.h"
#include "hns/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/pending_associated_remote.h"

namespace base {
class Time;
}  // namespace base

namespace hns_ads {
class AdsClientNotifierObserver;
struct NotificationAdInfo;
}  // namespace hns_ads

namespace bat_ads {

class BatAdsClientMojoBridge : public hns_ads::AdsClient {
 public:
  explicit BatAdsClientMojoBridge(
      mojo::PendingAssociatedRemote<mojom::BatAdsClient> client_info,
      mojo::PendingReceiver<mojom::BatAdsClientNotifier> client_notifier);

  BatAdsClientMojoBridge(const BatAdsClientMojoBridge&) = delete;
  BatAdsClientMojoBridge& operator=(const BatAdsClientMojoBridge&) = delete;

  BatAdsClientMojoBridge(BatAdsClientMojoBridge&& other) noexcept = delete;
  BatAdsClientMojoBridge& operator=(BatAdsClientMojoBridge&& other) noexcept =
      delete;

  ~BatAdsClientMojoBridge() override;

  // AdsClient:
  void AddObserver(hns_ads::AdsClientNotifierObserver* observer) override;
  void RemoveObserver(hns_ads::AdsClientNotifierObserver* observer) override;
  void NotifyPendingObservers() override;

  bool IsNetworkConnectionAvailable() const override;

  bool IsBrowserActive() const override;
  bool IsBrowserInFullScreenMode() const override;

  bool CanShowNotificationAds() override;
  bool CanShowNotificationAdsWhileBrowserIsBackgrounded() const override;
  void ShowNotificationAd(const hns_ads::NotificationAdInfo& ad) override;
  void CloseNotificationAd(const std::string& placement_id) override;

  void ShowReminder(hns_ads::mojom::ReminderType type) override;

  void UpdateAdRewards() override;

  void RecordAdEventForId(const std::string& id,
                          const std::string& ad_type,
                          const std::string& confirmation_type,
                          base::Time time) const override;
  std::vector<base::Time> GetAdEventHistory(
      const std::string& ad_type,
      const std::string& confirmation_type) const override;
  void ResetAdEventHistoryForId(const std::string& id) const override;

  void GetBrowsingHistory(
      int max_count,
      int days_ago,
      hns_ads::GetBrowsingHistoryCallback callback) override;

  void UrlRequest(hns_ads::mojom::UrlRequestInfoPtr url_request,
                  hns_ads::UrlRequestCallback callback) override;

  void Save(const std::string& name,
            const std::string& value,
            hns_ads::SaveCallback callback) override;
  void Load(const std::string& name, hns_ads::LoadCallback callback) override;
  void LoadFileResource(const std::string& id,
                        int version,
                        hns_ads::LoadFileCallback callback) override;
  std::string LoadDataResource(const std::string& name) override;

  void GetScheduledCaptcha(
      const std::string& payment_id,
      hns_ads::GetScheduledCaptchaCallback callback) override;
  void ShowScheduledCaptchaNotification(const std::string& payment_id,
                                        const std::string& captcha_id) override;

  void RunDBTransaction(hns_ads::mojom::DBTransactionInfoPtr transaction,
                        hns_ads::RunDBTransactionCallback callback) override;

  void RecordP2AEvents(base::Value::List events) override;

  void AddTrainingSample(std::vector<hns_federated::mojom::CovariateInfoPtr>
                             training_sample) override;

  bool GetBooleanPref(const std::string& path) const override;
  void SetBooleanPref(const std::string& path, bool value) override;
  int GetIntegerPref(const std::string& path) const override;
  void SetIntegerPref(const std::string& path, int value) override;
  double GetDoublePref(const std::string& path) const override;
  void SetDoublePref(const std::string& path, double value) override;
  std::string GetStringPref(const std::string& path) const override;
  void SetStringPref(const std::string& path,
                     const std::string& value) override;
  int64_t GetInt64Pref(const std::string& path) const override;
  void SetInt64Pref(const std::string& path, int64_t value) override;
  uint64_t GetUint64Pref(const std::string& path) const override;
  void SetUint64Pref(const std::string& path, uint64_t value) override;
  base::Time GetTimePref(const std::string& path) const override;
  void SetTimePref(const std::string& path, base::Time value) override;
  absl::optional<base::Value::Dict> GetDictPref(
      const std::string& path) const override;
  void SetDictPref(const std::string& path, base::Value::Dict value) override;
  absl::optional<base::Value::List> GetListPref(
      const std::string& path) const override;
  void SetListPref(const std::string& path, base::Value::List value) override;
  void ClearPref(const std::string& path) override;
  bool HasPrefPath(const std::string& path) const override;

  void Log(const char* file,
           int line,
           int verbose_level,
           const std::string& message) override;

 private:
  mojo::AssociatedRemote<mojom::BatAdsClient> bat_ads_client_;
  BatAdsClientNotifierImpl notifier_impl_;
};

}  // namespace bat_ads

#endif  // HNS_COMPONENTS_SERVICES_BAT_ADS_BAT_ADS_CLIENT_MOJO_BRIDGE_H_
