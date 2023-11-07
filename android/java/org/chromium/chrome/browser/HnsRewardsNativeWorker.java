/**
 * Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser;

import android.os.Handler;

import androidx.annotation.Nullable;

import org.json.JSONException;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.hns_rewards.mojom.PublisherStatus;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.components.embedder_support.util.UrlConstants;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class HnsRewardsNativeWorker {
    /**
     * Allows to monitor a front tab publisher changes.
     */
    public interface PublisherObserver { void onFrontTabPublisherChanged(boolean verified); }

    // Rewards notifications
    // Taken from components/hns_rewards/browser/rewards_notification_service.h
    public static final int REWARDS_NOTIFICATION_INVALID = 0;
    public static final int REWARDS_NOTIFICATION_AUTO_CONTRIBUTE = 1;
    public static final int REWARDS_NOTIFICATION_GRANT = 2;
    public static final int REWARDS_NOTIFICATION_GRANT_ADS = 3;
    public static final int REWARDS_NOTIFICATION_FAILED_CONTRIBUTION = 4;
    public static final int REWARDS_NOTIFICATION_IMPENDING_CONTRIBUTION = 5;
    public static final int REWARDS_NOTIFICATION_TIPS_PROCESSED = 8;
    public static final int REWARDS_NOTIFICATION_ADS_ONBOARDING = 9;
    public static final int REWARDS_NOTIFICATION_VERIFIED_PUBLISHER = 10;
    public static final int REWARDS_NOTIFICATION_PENDING_NOT_ENOUGH_FUNDS = 11;
    public static final int REWARDS_NOTIFICATION_GENERAL = 12;

    public static final int OK = 0;
    public static final int FAILED = 1;
    public static final int BAT_NOT_ALLOWED = 25;
    public static final int SAFETYNET_ATTESTATION_FAILED = 27;

    private static final int REWARDS_UNKNOWN = 0;
    private static final int REWARDS_DISABLED = 1;
    private static final int REWARDS_ENABLED = 2;
    private static int rewardsStatus = REWARDS_UNKNOWN;
    private String frontTabUrl;
    private static final Handler mHandler = new Handler();

    private List<HnsRewardsObserver> mObservers;
    private List<PublisherObserver> mFrontTabPublisherObservers;
    private long mNativeHnsRewardsNativeWorker;

    private static HnsRewardsNativeWorker instance;
    private static final Object lock = new Object();
    private boolean grantClaimInProcess;  // flag: wallet is being created

    public static  HnsRewardsNativeWorker getInstance() {
        synchronized(lock) {
          if(instance == null) {
              instance = new HnsRewardsNativeWorker();
              instance.Init();
          }
        }
        return instance;
    }

    private HnsRewardsNativeWorker() {
        mObservers = new ArrayList<HnsRewardsObserver>();
        mFrontTabPublisherObservers = new ArrayList<PublisherObserver>();
    }

    private void Init() {
      if (mNativeHnsRewardsNativeWorker == 0) {
          HnsRewardsNativeWorkerJni.get().init(HnsRewardsNativeWorker.this);
      }
    }

    @Override
    protected void finalize() {
        Destroy();
    }

    private void Destroy() {
        if (mNativeHnsRewardsNativeWorker != 0) {
            HnsRewardsNativeWorkerJni.get().destroy(mNativeHnsRewardsNativeWorker);
            mNativeHnsRewardsNativeWorker = 0;
        }
    }

    public void AddObserver(HnsRewardsObserver observer) {
        synchronized(lock) {
            mObservers.add(observer);
        }
    }

    public void RemoveObserver(HnsRewardsObserver observer) {
        synchronized(lock) {
            mObservers.remove(observer);
        }
    }

    public void AddPublisherObserver(PublisherObserver observer) {
        synchronized (lock) {
            mFrontTabPublisherObservers.add(observer);
        }
    }

    public void RemovePublisherObserver(PublisherObserver observer) {
        synchronized (lock) {
            mFrontTabPublisherObservers.remove(observer);
        }
    }

    public void OnNotifyFrontTabUrlChanged(int tabId, String url) {
        boolean chromeUrl = url.startsWith(UrlConstants.CHROME_SCHEME);
        boolean newUrl = (frontTabUrl == null || !frontTabUrl.equals(url));
        if (chromeUrl) {
            // Don't query 'GetPublisherInfo' and post response now.
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    NotifyPublisherObservers(false);
                }
            });
        } else if (newUrl) {
            GetPublisherInfo(tabId, url);
        }

        frontTabUrl = url;
    }

    private void NotifyPublisherObservers(boolean verified) {
        for (PublisherObserver observer : mFrontTabPublisherObservers) {
            observer.onFrontTabPublisherChanged(verified);
        }
    }

    public void TriggerOnNotifyFrontTabUrlChanged() {
        // Clear frontTabUrl so that all observers are updated.
        frontTabUrl = "";
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                Tab tab = HnsRewardsHelper.currentActiveChromeTabbedActivityTab();
                if (tab != null && !tab.isIncognito()) {
                    OnNotifyFrontTabUrlChanged(tab.getId(), tab.getUrl().getSpec());
                }
            }
        });
    }

    public boolean IsGrantClaimInProcess() {
        synchronized(lock) {
          return grantClaimInProcess;
        }
    }

    public boolean IsSupported() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().isSupported(mNativeHnsRewardsNativeWorker);
        }
    }

    public boolean IsSupportedSkipRegionCheck() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().isSupportedSkipRegionCheck(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public boolean isRewardsEnabled() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().isRewardsEnabled(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public void CreateRewardsWallet(String countryCode) {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().createRewardsWallet(
                    mNativeHnsRewardsNativeWorker, countryCode);
        }
    }

    public void GetRewardsParameters() {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().getRewardsParameters(mNativeHnsRewardsNativeWorker);
        }
    }

    public double getVbatDeadline() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().getVbatDeadline(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public void getUserType() {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().getUserType(mNativeHnsRewardsNativeWorker);
        }
    }

    public boolean isGrandfatheredUser() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().isGrandfatheredUser(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public void fetchBalance() {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().fetchBalance(mNativeHnsRewardsNativeWorker);
        }
    }

    @Nullable
    public HnsRewardsBalance GetWalletBalance() {
        synchronized(lock) {
            String json = HnsRewardsNativeWorkerJni.get().getWalletBalance(
                    mNativeHnsRewardsNativeWorker);
            HnsRewardsBalance balance = null;
            try{
                balance = new HnsRewardsBalance (json);
            }
            catch (JSONException e) {
                balance = null;
            }
            return balance;
        }
    }

    public String getExternalWalletType() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().getExternalWalletType(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public boolean canConnectAccount() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().canConnectAccount(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public double[] GetTipChoices() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().GetTipChoices(mNativeHnsRewardsNativeWorker);
        }
    }

    public double GetWalletRate() {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getWalletRate(mNativeHnsRewardsNativeWorker);
        }
    }

    public void GetPublisherInfo(int tabId, String host) {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().getPublisherInfo(
                    mNativeHnsRewardsNativeWorker, tabId, host);
        }
    }

    public String GetPublisherURL(int tabId) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getPublisherURL(
                    mNativeHnsRewardsNativeWorker, tabId);
        }
    }

    public String getCaptchaSolutionURL(String paymentId, String captchaId) {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().getCaptchaSolutionURL(
                    mNativeHnsRewardsNativeWorker, paymentId, captchaId);
        }
    }

    public String getAttestationURL() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().getAttestationURL(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public String getAttestationURLWithPaymentId(String paymentId) {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().getAttestationURLWithPaymentId(
                    mNativeHnsRewardsNativeWorker, paymentId);
        }
    }

    public String GetPublisherFavIconURL(int tabId) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getPublisherFavIconURL(
                    mNativeHnsRewardsNativeWorker, tabId);
        }
    }

    public String GetPublisherName(int tabId) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getPublisherName(
                    mNativeHnsRewardsNativeWorker, tabId);
        }
    }

    public String GetPublisherId(int tabId) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getPublisherId(
                    mNativeHnsRewardsNativeWorker, tabId);
        }
    }

    public int GetPublisherPercent(int tabId) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getPublisherPercent(
                    mNativeHnsRewardsNativeWorker, tabId);
        }
    }

    public boolean GetPublisherExcluded(int tabId) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getPublisherExcluded(
                    mNativeHnsRewardsNativeWorker, tabId);
        }
    }

    public int GetPublisherStatus(int tabId) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getPublisherStatus(
                    mNativeHnsRewardsNativeWorker, tabId);
        }
    }

    public void IncludeInAutoContribution(int tabId, boolean exclude) {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().includeInAutoContribution(
                    mNativeHnsRewardsNativeWorker, tabId, exclude);
        }
    }

    public void RemovePublisherFromMap(int tabId) {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().removePublisherFromMap(
                    mNativeHnsRewardsNativeWorker, tabId);
        }
    }

    public void GetCurrentBalanceReport() {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().getCurrentBalanceReport(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public void Donate(String publisher_key, double amount, boolean recurring) {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().donate(
                    mNativeHnsRewardsNativeWorker, publisher_key, amount, recurring);
        }
    }

    public void GetAllNotifications() {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                synchronized (lock) {
                    HnsRewardsNativeWorkerJni.get().getAllNotifications(
                            mNativeHnsRewardsNativeWorker);
                }
            }
        });
    }

    public void DeleteNotification(String notification_id) {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().deleteNotification(
                    mNativeHnsRewardsNativeWorker, notification_id);
        }
    }

    public void GetGrant(String promotionId) {
        synchronized(lock) {
            if (grantClaimInProcess) {
                return;
            }
            grantClaimInProcess = true;
            HnsRewardsNativeWorkerJni.get().getGrant(
                    mNativeHnsRewardsNativeWorker, promotionId);
        }
    }

    public String[] GetCurrentGrant(int position) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getCurrentGrant(
                    mNativeHnsRewardsNativeWorker, position);
        }
    }

    public void GetRecurringDonations() {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().getRecurringDonations(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public boolean IsCurrentPublisherInRecurrentDonations(String publisher) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().isCurrentPublisherInRecurrentDonations(
                    mNativeHnsRewardsNativeWorker, publisher);
        }
    }

    public double GetPublisherRecurrentDonationAmount(String publisher) {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().getPublisherRecurrentDonationAmount(
                    mNativeHnsRewardsNativeWorker, publisher);
        }
    }

    public void GetAutoContributeProperties() {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().getAutoContributeProperties(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public boolean IsAutoContributeEnabled() {
        synchronized(lock) {
            return HnsRewardsNativeWorkerJni.get().isAutoContributeEnabled(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public void GetReconcileStamp() {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().getReconcileStamp(mNativeHnsRewardsNativeWorker);
        }
    }

    public void RemoveRecurring(String publisher) {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().removeRecurring(
                    mNativeHnsRewardsNativeWorker, publisher);
        }
    }

    public void ResetTheWholeState() {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().resetTheWholeState(mNativeHnsRewardsNativeWorker);
        }
    }

    public void FetchGrants() {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().fetchGrants(mNativeHnsRewardsNativeWorker);
        }
    }

    public int GetAdsPerHour() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().getAdsPerHour(mNativeHnsRewardsNativeWorker);
        }
    }

    public void SetAdsPerHour(int value) {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().setAdsPerHour(mNativeHnsRewardsNativeWorker, value);
        }
    }

    public void GetExternalWallet() {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().getExternalWallet(mNativeHnsRewardsNativeWorker);
        }
    }

    public String getCountryCode() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().getCountryCode(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public void getAvailableCountries() {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().getAvailableCountries(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public void GetPublisherBanner(String publisher_key) {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().GetPublisherBanner(
                    mNativeHnsRewardsNativeWorker, publisher_key);
        }
    }

    public void getPublishersVisitedCount() {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().getPublishersVisitedCount(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    @CalledByNative
    public void onGetPublishersVisitedCount(int count) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onGetPublishersVisitedCount(count);
        }
    }

    public void DisconnectWallet() {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().disconnectWallet(mNativeHnsRewardsNativeWorker);
        }
    }

    public void getAdsAccountStatement() {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().getAdsAccountStatement(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    @CalledByNative
    public void onCreateRewardsWallet(String result) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onCreateRewardsWallet(result);
        }
    }

    public void RefreshPublisher(String publisherKey) {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().refreshPublisher(
                    mNativeHnsRewardsNativeWorker, publisherKey);
        }
    }

    public void SetAutoContributeEnabled(boolean isSetAutoContributeEnabled) {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().setAutoContributeEnabled(
                    mNativeHnsRewardsNativeWorker, isSetAutoContributeEnabled);
        }
    }

    public void SetAutoContributionAmount(double amount) {
        synchronized(lock) {
            HnsRewardsNativeWorkerJni.get().setAutoContributionAmount(
                    mNativeHnsRewardsNativeWorker, amount);
        }
    }

    public void getAutoContributionAmount() {
        synchronized (lock) {
            HnsRewardsNativeWorkerJni.get().getAutoContributionAmount(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    public String getPayoutStatus() {
        synchronized (lock) {
            return HnsRewardsNativeWorkerJni.get().getPayoutStatus(
                    mNativeHnsRewardsNativeWorker);
        }
    }

    @CalledByNative
    public void OnRefreshPublisher(int status, String publisherKey) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnRefreshPublisher(status, publisherKey);
        }
    }

    @CalledByNative
    public void OnRewardsParameters() {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnRewardsParameters();
        }
    }

    @CalledByNative
    public void onBalance(boolean success) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onBalance(success);
        }
    }

    @CalledByNative
    public void OnGetCurrentBalanceReport(double[] report) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnGetCurrentBalanceReport(report);
        }
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeHnsRewardsNativeWorker == 0;
        mNativeHnsRewardsNativeWorker = nativePtr;
    }

    @CalledByNative
    public void OnPublisherInfo(int tabId) {
        int pubStatus = GetPublisherStatus(tabId);
        boolean verified = pubStatus != PublisherStatus.NOT_VERIFIED;
        NotifyPublisherObservers(verified);

        // Notify HnsRewardsObserver (panel).
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnPublisherInfo(tabId);
        }
    }

    @CalledByNative
    public void OnNotificationAdded(String id, int type, long timestamp,
            String[] args) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnNotificationAdded(id, type, timestamp, args);
        }
    }

    @CalledByNative
    public void OnNotificationsCount(int count) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnNotificationsCount(count);
        }
    }

    @CalledByNative
    public void OnGetLatestNotification(String id, int type, long timestamp,
            String[] args) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnGetLatestNotification(id, type, timestamp, args);
        }
    }

    @CalledByNative
    public void OnNotificationDeleted(String id) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnNotificationDeleted(id);
        }
    }

    @CalledByNative
    public void OnGetAutoContributeProperties() {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnGetAutoContributeProperties();
        }
    }

    @CalledByNative
    public void OnGetAutoContributionAmount(double amount) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onGetAutoContributionAmount(amount);
        }
    }

    @CalledByNative
    public void OnGetReconcileStamp(long timestamp) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnGetReconcileStamp(timestamp);
        }
    }

    @CalledByNative
    public void OnRecurringDonationUpdated() {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnRecurringDonationUpdated();
        }
    }

    @CalledByNative
    public void OnGrantFinish(int result) {
        grantClaimInProcess = false;
        for(HnsRewardsObserver observer : mObservers) {
            observer.OnGrantFinish(result);
        }
    }

    @CalledByNative
    public void onCompleteReset(boolean success) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onCompleteReset(success);
        }
    }

    @CalledByNative
    public void OnResetTheWholeState(boolean success) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnResetTheWholeState(success);
        }
    }

    @CalledByNative
    public void OnGetExternalWallet(String external_wallet) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnGetExternalWallet(external_wallet);
        }
    }

    @CalledByNative
    public void onGetAvailableCountries(String[] countries) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onGetAvailableCountries(countries);
        }
    }

    @CalledByNative
    public void OnGetAdsAccountStatement(boolean success, double nextPaymentDate,
            int adsReceivedThisMonth, double minEarningsThisMonth, double maxEarningsThisMonth,
            double minEarningsLastMonth, double maxEarningsLastMonth) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnGetAdsAccountStatement(success, nextPaymentDate, adsReceivedThisMonth,
                    minEarningsThisMonth, maxEarningsThisMonth, minEarningsLastMonth,
                    maxEarningsLastMonth);
        }
    }

    @CalledByNative
    public void OnExternalWalletConnected() {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnExternalWalletConnected();
        }
    }

    @CalledByNative
    public void OnExternalWalletLoggedOut() {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnExternalWalletLoggedOut();
        }
    }

    @CalledByNative
    public void OnExternalWalletReconnected() {
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnExternalWalletReconnected();
        }
    }

    @CalledByNative
    public void OnClaimPromotion(int error_code) {
        grantClaimInProcess = false;
        for (HnsRewardsObserver observer : mObservers) {
            observer.OnClaimPromotion(error_code);
        }
    }

    @CalledByNative
    public void onSendContribution(boolean result) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onSendContribution(result);
        }
    }

    @CalledByNative
    public void onUnblindedTokensReady() {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onUnblindedTokensReady();
        }
    }

    @CalledByNative
    public void onReconcileComplete(int resultCode, int rewardsType, double amount) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onReconcileComplete(resultCode, rewardsType, amount);
        }
    }

    @CalledByNative
    public void onPublisherBanner(String jsonBannerInfo) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onPublisherBanner(jsonBannerInfo);
        }
    }

    @CalledByNative
    public void onGetUserType(int userType) {
        for (HnsRewardsObserver observer : mObservers) {
            observer.onGetUserType(userType);
        }
    }

    @NativeMethods
    interface Natives {
        void init(HnsRewardsNativeWorker caller);
        void destroy(long nativeHnsRewardsNativeWorker);
        boolean isSupported(long nativeHnsRewardsNativeWorker);
        boolean isSupportedSkipRegionCheck(long nativeHnsRewardsNativeWorker);
        boolean isRewardsEnabled(long nativeHnsRewardsNativeWorker);
        String getWalletBalance(long nativeHnsRewardsNativeWorker);
        String getExternalWalletType(long nativeHnsRewardsNativeWorker);
        void GetPublisherBanner(long nativeHnsRewardsNativeWorker, String publisher_key);
        void getPublishersVisitedCount(long nativeHnsRewardsNativeWorker);
        boolean canConnectAccount(long nativeHnsRewardsNativeWorker);
        double[] GetTipChoices(long nativeHnsRewardsNativeWorker);
        double getWalletRate(long nativeHnsRewardsNativeWorker);
        void getPublisherInfo(long nativeHnsRewardsNativeWorker, int tabId, String host);
        String getPublisherURL(long nativeHnsRewardsNativeWorker, int tabId);
        String getCaptchaSolutionURL(
                long nativeHnsRewardsNativeWorker, String paymentId, String captchaId);
        String getAttestationURL(long nativeHnsRewardsNativeWorker);
        String getAttestationURLWithPaymentId(
                long nativeHnsRewardsNativeWorker, String paymentId);
        String getPublisherFavIconURL(long nativeHnsRewardsNativeWorker, int tabId);
        String getPublisherName(long nativeHnsRewardsNativeWorker, int tabId);
        String getPublisherId(long nativeHnsRewardsNativeWorker, int tabId);
        int getPublisherPercent(long nativeHnsRewardsNativeWorker, int tabId);
        boolean getPublisherExcluded(long nativeHnsRewardsNativeWorker, int tabId);
        int getPublisherStatus(long nativeHnsRewardsNativeWorker, int tabId);
        void includeInAutoContribution(
                long nativeHnsRewardsNativeWorker, int tabId, boolean exclude);
        void removePublisherFromMap(long nativeHnsRewardsNativeWorker, int tabId);
        void getCurrentBalanceReport(long nativeHnsRewardsNativeWorker);
        void donate(long nativeHnsRewardsNativeWorker, String publisher_key, double amount,
                boolean recurring);
        void getAllNotifications(long nativeHnsRewardsNativeWorker);
        void deleteNotification(long nativeHnsRewardsNativeWorker, String notification_id);
        void getGrant(long nativeHnsRewardsNativeWorker, String promotionId);
        String[] getCurrentGrant(long nativeHnsRewardsNativeWorker, int position);
        void getRecurringDonations(long nativeHnsRewardsNativeWorker);
        boolean isCurrentPublisherInRecurrentDonations(
                long nativeHnsRewardsNativeWorker, String publisher);
        void getAutoContributeProperties(long nativeHnsRewardsNativeWorker);
        boolean isAutoContributeEnabled(long nativeHnsRewardsNativeWorker);
        void getReconcileStamp(long nativeHnsRewardsNativeWorker);
        double getPublisherRecurrentDonationAmount(
                long nativeHnsRewardsNativeWorker, String publisher);
        void removeRecurring(long nativeHnsRewardsNativeWorker, String publisher);
        void resetTheWholeState(long nativeHnsRewardsNativeWorker);
        void fetchGrants(long nativeHnsRewardsNativeWorker);
        int getAdsPerHour(long nativeHnsRewardsNativeWorker);
        void setAdsPerHour(long nativeHnsRewardsNativeWorker, int value);
        void getExternalWallet(long nativeHnsRewardsNativeWorker);
        String getCountryCode(long nativeHnsRewardsNativeWorker);
        void getAvailableCountries(long nativeHnsRewardsNativeWorker);
        void disconnectWallet(long nativeHnsRewardsNativeWorker);
        void refreshPublisher(long nativeHnsRewardsNativeWorker, String publisherKey);
        void createRewardsWallet(long nativeHnsRewardsNativeWorker, String countryCode);
        void getRewardsParameters(long nativeHnsRewardsNativeWorker);
        double getVbatDeadline(long nativeHnsRewardsNativeWorker);
        void getUserType(long nativeHnsRewardsNativeWorker);
        boolean isGrandfatheredUser(long nativeHnsRewardsNativeWorker);
        void fetchBalance(long nativeHnsRewardsNativeWorker);
        void setAutoContributeEnabled(
                long nativeHnsRewardsNativeWorker, boolean isSetAutoContributeEnabled);
        void setAutoContributionAmount(long nativeHnsRewardsNativeWorker, double amount);
        void getAutoContributionAmount(long nativeHnsRewardsNativeWorker);
        void getAdsAccountStatement(long nativeHnsRewardsNativeWorker);
        String getPayoutStatus(long nativeHnsRewardsNativeWorker);
    }
}
