/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_H_

#include <memory>
#include <string>
#include <vector>

#include "base/functional/callback_forward.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "base/scoped_observation.h"
#include "base/sequence_checker.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "hns/components/hns_vpn/browser/api/hns_vpn_api_request.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_os_connection_api.h"
#include "hns/components/hns_vpn/common/hns_vpn_data_types.h"
#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#include "hns/components/skus/browser/skus_utils.h"
#include "hns/components/skus/common/skus_sdk.mojom.h"
#include "build/build_config.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/prefs/pref_change_registrar.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote_set.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"

namespace network {
class SharedURLLoaderFactory;
}  // namespace network

class PrefService;

#if !BUILDFLAG(IS_ANDROID)
class HnsAppMenuBrowserTest;
class HnsAppMenuModelBrowserTest;
class HnsBrowserCommandControllerTest;
#endif  // !BUILDFLAG(IS_ANDROID)

namespace hns_vpn {

constexpr char kNewUserReturningHistogramName[] = "Hns.VPN.NewUserReturning";
constexpr char kDaysInMonthUsedHistogramName[] = "Hns.VPN.DaysInMonthUsed";
constexpr char kLastUsageTimeHistogramName[] = "Hns.VPN.LastUsageTime";

// This class is used by desktop and android.
// However, it includes desktop specific impls and it's hidden
// by IS_ANDROID ifdef.
class HnsVpnService :
#if !BUILDFLAG(IS_ANDROID)
    public HnsVPNOSConnectionAPI::Observer,
#endif
    public mojom::ServiceHandler,
    public KeyedService {
 public:
  HnsVpnService(
      HnsVPNOSConnectionAPI* connection_api,
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs,
      PrefService* profile_prefs,
      base::RepeatingCallback<mojo::PendingRemote<skus::mojom::SkusService>()>
          skus_service_getter);
  ~HnsVpnService() override;

  HnsVpnService(const HnsVpnService&) = delete;
  HnsVpnService& operator=(const HnsVpnService&) = delete;

  std::string GetCurrentEnvironment() const;
  bool is_purchased_user() const {
    return GetPurchasedInfoSync().state == mojom::PurchasedState::PURCHASED;
  }
  void BindInterface(mojo::PendingReceiver<mojom::ServiceHandler> receiver);
  void ReloadPurchasedState();
  bool IsHnsVPNEnabled() const;
#if !BUILDFLAG(IS_ANDROID)
  void ToggleConnection();
  mojom::ConnectionState GetConnectionState() const;
  bool IsConnected() const;

  // mojom::vpn::ServiceHandler
  void GetConnectionState(GetConnectionStateCallback callback) override;
  void Connect() override;
  void Disconnect() override;
  void GetAllRegions(GetAllRegionsCallback callback) override;
  void GetSelectedRegion(GetSelectedRegionCallback callback) override;
  void SetSelectedRegion(mojom::RegionPtr region) override;
  void GetProductUrls(GetProductUrlsCallback callback) override;
  void CreateSupportTicket(const std::string& email,
                           const std::string& subject,
                           const std::string& body,
                           CreateSupportTicketCallback callback) override;
  void GetSupportData(GetSupportDataCallback callback) override;
  void ResetConnectionState() override;
#else
  // mojom::vpn::ServiceHandler
  void GetPurchaseToken(GetPurchaseTokenCallback callback) override;
#endif  // !BUILDFLAG(IS_ANDROID)

  using ResponseCallback =
      base::OnceCallback<void(const std::string&, bool success)>;

  // mojom::vpn::ServiceHandler
  void AddObserver(
      mojo::PendingRemote<mojom::ServiceObserver> observer) override;
  void GetPurchasedState(GetPurchasedStateCallback callback) override;
  void LoadPurchasedState(const std::string& domain) override;

  void GetAllServerRegions(ResponseCallback callback);
  void GetTimezonesForRegions(ResponseCallback callback);
  void GetHostnamesForRegion(ResponseCallback callback,
                             const std::string& region);
  void GetProfileCredentials(ResponseCallback callback,
                             const std::string& subscriber_credential,
                             const std::string& hostname);
  void GetWireguardProfileCredentials(ResponseCallback callback,
                                      const std::string& subscriber_credential,
                                      const std::string& public_key,
                                      const std::string& hostname);
  void VerifyCredentials(ResponseCallback callback,
                         const std::string& hostname,
                         const std::string& client_id,
                         const std::string& subscriber_credential,
                         const std::string& api_auth_token);
  void InvalidateCredentials(ResponseCallback callback,
                             const std::string& hostname,
                             const std::string& client_id,
                             const std::string& subscriber_credential,
                             const std::string& api_auth_token);
  void GetSubscriberCredential(ResponseCallback callback,
                               const std::string& product_type,
                               const std::string& product_id,
                               const std::string& validation_method,
                               const std::string& purchase_token,
                               const std::string& bundle_id);
  void VerifyPurchaseToken(ResponseCallback callback,
                           const std::string& purchase_token,
                           const std::string& product_id,
                           const std::string& product_type,
                           const std::string& bundle_id);
  void GetSubscriberCredentialV12(ResponseCallback callback);

  // new_usage should be set to true if a new VPN connection was just
  // established.
  void RecordP3A(bool new_usage);
#if BUILDFLAG(IS_ANDROID)
  void RecordAndroidBackgroundP3A(int64_t session_start_time_ms,
                                  int64_t session_end_time_ms);
#endif

 private:
  friend class HnsVPNServiceTest;

#if !BUILDFLAG(IS_ANDROID)
  friend class ::HnsAppMenuBrowserTest;
  friend class ::HnsAppMenuModelBrowserTest;
  friend class ::HnsBrowserCommandControllerTest;

  // HnsVPNOSConnectionAPI::Observer overrides:
  void OnConnectionStateChanged(mojom::ConnectionState state) override;
  void OnRegionDataReady(bool success) override;
  void OnSelectedRegionChanged(const std::string& region_name) override;

  void OnCreateSupportTicket(CreateSupportTicketCallback callback,
                             const std::string& ticket,
                             bool success);

  void OnPreferenceChanged(const std::string& pref_name);

  void UpdatePurchasedStateForSessionExpired(const std::string& env);
#endif  // !BUILDFLAG(IS_ANDROID)

  // KeyedService overrides:
  void Shutdown() override;

  void InitP3A();
  void OnP3AInterval();

  mojom::PurchasedInfo GetPurchasedInfoSync() const;
  void SetPurchasedState(
      const std::string& env,
      mojom::PurchasedState state,
      const absl::optional<std::string>& description = absl::nullopt);
  void SetCurrentEnvironment(const std::string& env);
  void EnsureMojoConnected();
  void OnMojoConnectionError();
  void RequestCredentialSummary(const std::string& domain);
  void OnCredentialSummary(const std::string& domain,
                           const std::string& summary_string);
  void OnPrepareCredentialsPresentation(
      const std::string& domain,
      const std::string& credential_as_cookie);
  void OnGetSubscriberCredentialV12(const base::Time& expiration_time,
                                    const std::string& subscriber_credential,
                                    bool success);
  void ScheduleSubscriberCredentialRefresh();
  void RefreshSubscriberCredential();

  // Check initial purchased/connected state.
  void CheckInitialState();

#if !BUILDFLAG(IS_ANDROID)
  base::ScopedObservation<HnsVPNOSConnectionAPI,
                          HnsVPNOSConnectionAPI::Observer>
      observed_{this};
  bool wait_region_data_ready_ = false;
  raw_ptr<HnsVPNOSConnectionAPI> connection_api_ = nullptr;

  PrefChangeRegistrar policy_pref_change_registrar_;
#endif  // !BUILDFLAG(IS_ANDROID)

  SEQUENCE_CHECKER(sequence_checker_);

  raw_ptr<PrefService> local_prefs_ = nullptr;
  raw_ptr<PrefService> profile_prefs_ = nullptr;
  mojo::ReceiverSet<mojom::ServiceHandler> receivers_;
  base::RepeatingCallback<mojo::PendingRemote<skus::mojom::SkusService>()>
      skus_service_getter_;
  mojo::Remote<skus::mojom::SkusService> skus_service_;
  absl::optional<mojom::PurchasedInfo> purchased_state_;
  mojo::RemoteSet<mojom::ServiceObserver> observers_;
  std::unique_ptr<HnsVpnAPIRequest> api_request_;
  base::RepeatingTimer p3a_timer_;
  base::OneShotTimer subs_cred_refresh_timer_;
  base::WeakPtrFactory<HnsVpnService> weak_ptr_factory_{this};
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_H_
