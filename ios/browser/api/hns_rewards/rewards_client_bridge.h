/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_REWARDS_REWARDS_CLIENT_BRIDGE_H_
#define HNS_IOS_BROWSER_API_HNS_REWARDS_REWARDS_CLIENT_BRIDGE_H_

#import <Foundation/Foundation.h>
#include <string>
#include <vector>
#include "hns/components/hns_rewards/common/mojom/rewards_engine.mojom.h"

NS_ASSUME_NONNULL_BEGIN

@protocol RewardsClientBridge
@required

- (void)loadLegacyState:
    (hns_rewards::mojom::RewardsEngineClient::LoadLegacyStateCallback)
        callback;
- (void)loadPublisherState:
    (hns_rewards::mojom::RewardsEngineClient::LoadPublisherStateCallback)
        callback;
- (void)onReconcileComplete:(hns_rewards::mojom::Result)result
               contribution:
                   (hns_rewards::mojom::ContributionInfoPtr)contribution;
- (void)onPanelPublisherInfo:(hns_rewards::mojom::Result)result
               publisherInfo:
                   (hns_rewards::mojom::PublisherInfoPtr)publisherInfo
                    windowId:(uint64_t)windowId;
- (void)
    fetchFavIcon:(const std::string&)url
      faviconKey:(const std::string&)faviconKey
        callback:
            (hns_rewards::mojom::RewardsEngineClient::FetchFavIconCallback)
                callback;
- (void)loadUrl:(hns_rewards::mojom::UrlRequestPtr)request
       callback:
           (hns_rewards::mojom::RewardsEngineClient::LoadURLCallback)callback;
- (void)publisherListNormalized:
    (std::vector<hns_rewards::mojom::PublisherInfoPtr>)list;
- (void)onPublisherRegistryUpdated;
- (void)onPublisherUpdated:(const std::string&)publisherId;
- (void)
    booleanState:(const std::string&)name
        callback:
            (hns_rewards::mojom::RewardsEngineClient::GetBooleanStateCallback)
                callback;
- (void)setBooleanState:(const std::string&)name
                  value:(bool)value
               callback:(hns_rewards::mojom::RewardsEngineClient::
                             SetBooleanStateCallback)callback;
- (void)
    integerState:(const std::string&)name
        callback:
            (hns_rewards::mojom::RewardsEngineClient::GetIntegerStateCallback)
                callback;
- (void)setIntegerState:(const std::string&)name
                  value:(int32_t)value
               callback:(hns_rewards::mojom::RewardsEngineClient::
                             SetIntegerStateCallback)callback;
- (void)
    doubleState:(const std::string&)name
       callback:
           (hns_rewards::mojom::RewardsEngineClient::GetDoubleStateCallback)
               callback;
- (void)setDoubleState:(const std::string&)name
                 value:(double)value
              callback:(hns_rewards::mojom::RewardsEngineClient::
                            SetDoubleStateCallback)callback;
- (void)
    stringState:(const std::string&)name
       callback:
           (hns_rewards::mojom::RewardsEngineClient::GetStringStateCallback)
               callback;
- (void)setStringState:(const std::string&)name
                 value:(const std::string&)value
              callback:(hns_rewards::mojom::RewardsEngineClient::
                            SetStringStateCallback)callback;
- (void)int64State:(const std::string&)name
          callback:
              (hns_rewards::mojom::RewardsEngineClient::GetInt64StateCallback)
                  callback;
- (void)
    setInt64State:(const std::string&)name
            value:(int64_t)value
         callback:
             (hns_rewards::mojom::RewardsEngineClient::SetInt64StateCallback)
                 callback;
- (void)
    uint64State:(const std::string&)name
       callback:
           (hns_rewards::mojom::RewardsEngineClient::GetUint64StateCallback)
               callback;
- (void)setUint64State:(const std::string&)name
                 value:(uint64_t)value
              callback:(hns_rewards::mojom::RewardsEngineClient::
                            SetUint64StateCallback)callback;
- (void)valueState:(const std::string&)name
          callback:
              (hns_rewards::mojom::RewardsEngineClient::GetValueStateCallback)
                  callback;
- (void)
    setValueState:(const std::string&)name
            value:(base::Value)value
         callback:
             (hns_rewards::mojom::RewardsEngineClient::SetValueStateCallback)
                 callback;
- (void)timeState:(const std::string&)name
         callback:
             (hns_rewards::mojom::RewardsEngineClient::GetTimeStateCallback)
                 callback;
- (void)
    setTimeState:(const std::string&)name
           value:(base::Time)value
        callback:
            (hns_rewards::mojom::RewardsEngineClient::SetTimeStateCallback)
                callback;
- (void)clearState:(const std::string&)name
          callback:
              (hns_rewards::mojom::RewardsEngineClient::ClearStateCallback)
                  callback;
- (void)getClientCountryCode:
    (hns_rewards::mojom::RewardsEngineClient::GetClientCountryCodeCallback)
        callback;
- (void)isAutoContributeSupportedForClient:
    (hns_rewards::mojom::RewardsEngineClient::
         IsAutoContributeSupportedForClientCallback)callback;
- (void)legacyWallet:
    (hns_rewards::mojom::RewardsEngineClient::GetLegacyWalletCallback)
        callback;
- (void)showNotification:(const std::string&)type
                    args:(std::vector<std::string>)args
                callback:(hns_rewards::mojom::RewardsEngineClient::
                              ShowNotificationCallback)callback;
- (void)clientInfo:
    (hns_rewards::mojom::RewardsEngineClient::GetClientInfoCallback)callback;
- (void)unblindedTokensReady;
- (void)reconcileStampReset;
- (void)runDbTransaction:(hns_rewards::mojom::DBTransactionPtr)transaction
                callback:(hns_rewards::mojom::RewardsEngineClient::
                              RunDBTransactionCallback)callback;
- (void)log:(const std::string&)file
            line:(int32_t)line
    verboseLevel:(int32_t)verboseLevel
         message:(const std::string&)message;
- (void)clearAllNotifications;
- (void)externalWalletConnected;
- (void)externalWalletLoggedOut;
- (void)externalWalletReconnected;
- (void)externalWalletDisconnected;
- (void)deleteLog:
    (hns_rewards::mojom::RewardsEngineClient::DeleteLogCallback)callback;
- (void)
    encryptString:(const std::string&)value
         callback:
             (hns_rewards::mojom::RewardsEngineClient::EncryptStringCallback)
                 callback;
- (void)
    decryptString:(const std::string&)value
         callback:
             (hns_rewards::mojom::RewardsEngineClient::DecryptStringCallback)
                 callback;

@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_HNS_REWARDS_REWARDS_CLIENT_BRIDGE_H_
