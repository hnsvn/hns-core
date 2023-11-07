/**
 * Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class HnsVpnNativeWorker {
    private long mNativeHnsVpnNativeWorker;
    private static final Object mLock = new Object();
    private static HnsVpnNativeWorker mInstance;

    private List<HnsVpnObserver> mObservers;

    public static HnsVpnNativeWorker getInstance() {
        synchronized (mLock) {
            if (mInstance == null) {
                mInstance = new HnsVpnNativeWorker();
                mInstance.init();
            }
        }
        return mInstance;
    }

    private HnsVpnNativeWorker() {
        mObservers = new ArrayList<HnsVpnObserver>();
    }

    private void init() {
        if (mNativeHnsVpnNativeWorker == 0) {
            HnsVpnNativeWorkerJni.get().init(this);
        }
    }

    @Override
    protected void finalize() {
        destroy();
    }

    private void destroy() {
        if (mNativeHnsVpnNativeWorker != 0) {
            HnsVpnNativeWorkerJni.get().destroy(mNativeHnsVpnNativeWorker, this);
            mNativeHnsVpnNativeWorker = 0;
        }
    }

    public void addObserver(HnsVpnObserver observer) {
        synchronized (mLock) {
            mObservers.add(observer);
        }
    }

    public void removeObserver(HnsVpnObserver observer) {
        synchronized (mLock) {
            mObservers.remove(observer);
        }
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeHnsVpnNativeWorker == 0;
        mNativeHnsVpnNativeWorker = nativePtr;
    }

    @CalledByNative
    public void onGetAllServerRegions(String jsonServerRegions, boolean isSuccess) {
        for (HnsVpnObserver observer : mObservers) {
            observer.onGetAllServerRegions(jsonServerRegions, isSuccess);
        }
    }

    @CalledByNative
    public void onGetTimezonesForRegions(String jsonTimezones, boolean isSuccess) {
        for (HnsVpnObserver observer : mObservers) {
            observer.onGetTimezonesForRegions(jsonTimezones, isSuccess);
        }
    }

    @CalledByNative
    public void onGetHostnamesForRegion(String jsonHostnames, boolean isSuccess) {
        for (HnsVpnObserver observer : mObservers) {
            observer.onGetHostnamesForRegion(jsonHostnames, isSuccess);
        }
    }

    @CalledByNative
    public void onGetWireguardProfileCredentials(
            String jsonWireguardProfileCredentials, boolean isSuccess) {
        for (HnsVpnObserver observer : mObservers) {
            observer.onGetWireguardProfileCredentials(jsonWireguardProfileCredentials, isSuccess);
        }
    }

    @CalledByNative
    public void onVerifyCredentials(String jsonVerifyCredentials, boolean isSuccess) {
        for (HnsVpnObserver observer : mObservers) {
            observer.onVerifyCredentials(jsonVerifyCredentials, isSuccess);
        }
    }

    @CalledByNative
    public void onInvalidateCredentials(String jsonInvalidateCredentials, boolean isSuccess) {
        for (HnsVpnObserver observer : mObservers) {
            observer.onInvalidateCredentials(jsonInvalidateCredentials, isSuccess);
        }
    }

    @CalledByNative
    public void onGetSubscriberCredential(String subscriberCredential, boolean isSuccess) {
        for (HnsVpnObserver observer : mObservers) {
            observer.onGetSubscriberCredential(subscriberCredential, isSuccess);
        }
    }

    @CalledByNative
    public void onVerifyPurchaseToken(String jsonResponse, boolean isSuccess) {
        for (HnsVpnObserver observer : mObservers) {
            observer.onVerifyPurchaseToken(jsonResponse, isSuccess);
        }
    }

    public void getAllServerRegions() {
        HnsVpnNativeWorkerJni.get().getAllServerRegions(mNativeHnsVpnNativeWorker);
    }

    public void getTimezonesForRegions() {
        HnsVpnNativeWorkerJni.get().getTimezonesForRegions(mNativeHnsVpnNativeWorker);
    }

    public void getHostnamesForRegion(String region) {
        HnsVpnNativeWorkerJni.get().getHostnamesForRegion(mNativeHnsVpnNativeWorker, region);
    }

    public void getWireguardProfileCredentials(
            String subscriberCredential, String publicKey, String hostname) {
        HnsVpnNativeWorkerJni.get().getWireguardProfileCredentials(
                mNativeHnsVpnNativeWorker, subscriberCredential, publicKey, hostname);
    }

    public void verifyCredentials(
            String hostname, String clientId, String subscriberCredential, String apiAuthToken) {
        HnsVpnNativeWorkerJni.get().verifyCredentials(mNativeHnsVpnNativeWorker, hostname,
                clientId, subscriberCredential, apiAuthToken);
    }

    public void invalidateCredentials(
            String hostname, String clientId, String subscriberCredential, String apiAuthToken) {
        HnsVpnNativeWorkerJni.get().invalidateCredentials(mNativeHnsVpnNativeWorker, hostname,
                clientId, subscriberCredential, apiAuthToken);
    }

    public void getSubscriberCredential(String productType, String productId,
            String validationMethod, String purchaseToken, String packageName) {
        HnsVpnNativeWorkerJni.get().getSubscriberCredential(mNativeHnsVpnNativeWorker,
                productType, productId, validationMethod, purchaseToken, packageName);
    }

    public void verifyPurchaseToken(
            String purchaseToken, String productId, String productType, String packageName) {
        HnsVpnNativeWorkerJni.get().verifyPurchaseToken(
                mNativeHnsVpnNativeWorker, purchaseToken, productId, productType, packageName);
    }

    // Desktop purchase methods
    public void reloadPurchasedState() {
        HnsVpnNativeWorkerJni.get().reloadPurchasedState(mNativeHnsVpnNativeWorker);
    }

    public boolean isPurchasedUser() {
        return HnsVpnNativeWorkerJni.get().isPurchasedUser(mNativeHnsVpnNativeWorker);
    }

    public void getSubscriberCredentialV12() {
        HnsVpnNativeWorkerJni.get().getSubscriberCredentialV12(mNativeHnsVpnNativeWorker);
    }

    public void reportBackgroundP3A(long sessionStartTimeMs, long sessionEndTimeMs) {
        HnsVpnNativeWorkerJni.get().reportBackgroundP3A(
                mNativeHnsVpnNativeWorker, sessionStartTimeMs, sessionEndTimeMs);
    }

    public void reportForegroundP3A() {
        HnsVpnNativeWorkerJni.get().reportForegroundP3A(mNativeHnsVpnNativeWorker);
    }

    @NativeMethods
    interface Natives {
        void init(HnsVpnNativeWorker caller);
        void destroy(long nativeHnsVpnNativeWorker, HnsVpnNativeWorker caller);
        void getAllServerRegions(long nativeHnsVpnNativeWorker);
        void getTimezonesForRegions(long nativeHnsVpnNativeWorker);
        void getHostnamesForRegion(long nativeHnsVpnNativeWorker, String region);
        void getWireguardProfileCredentials(long nativeHnsVpnNativeWorker,
                String subscriberCredential, String publicKey, String hostname);
        void verifyCredentials(long nativeHnsVpnNativeWorker, String hostname, String clientId,
                String subscriberCredential, String apiAuthToken);
        void invalidateCredentials(long nativeHnsVpnNativeWorker, String hostname,
                String clientId, String subscriberCredential, String apiAuthToken);
        void getSubscriberCredential(long nativeHnsVpnNativeWorker, String productType,
                String productId, String validationMethod, String purchaseToken,
                String packageName);
        void verifyPurchaseToken(long nativeHnsVpnNativeWorker, String purchaseToken,
                String productId, String productType, String packageName);
        void reloadPurchasedState(long nativeHnsVpnNativeWorker);
        boolean isPurchasedUser(long nativeHnsVpnNativeWorker);
        void getSubscriberCredentialV12(long nativeHnsVpnNativeWorker);
        void reportBackgroundP3A(
                long nativeHnsVpnNativeWorker, long sessionStartTimeMs, long sessionEndTimeMs);
        void reportForegroundP3A(long nativeHnsVpnNativeWorker);
    }
}
