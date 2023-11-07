/**
 * Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn.activities;

import android.content.Intent;
import android.util.Pair;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.wireguard.android.backend.GoBackend;
import com.wireguard.crypto.KeyPair;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.init.AsyncInitializationActivity;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.chrome.browser.vpn.HnsVpnNativeWorker;
import org.chromium.chrome.browser.vpn.HnsVpnObserver;
import org.chromium.chrome.browser.vpn.billing.InAppPurchaseWrapper;
import org.chromium.chrome.browser.vpn.billing.PurchaseModel;
import org.chromium.chrome.browser.vpn.models.HnsVpnPrefModel;
import org.chromium.chrome.browser.vpn.models.HnsVpnWireguardProfileCredentials;
import org.chromium.chrome.browser.vpn.utils.HnsVpnApiResponseUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnProfileUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnUtils;
import org.chromium.chrome.browser.vpn.wireguard.WireguardConfigUtils;

public abstract class HnsVpnParentActivity
        extends AsyncInitializationActivity implements HnsVpnObserver {
    private static final String TAG = "HnsVPN";
    public boolean mIsVerification;
    protected HnsVpnPrefModel mHnsVpnPrefModel;

    abstract void showRestoreMenu(boolean shouldShowRestore);
    abstract void updateProfileView();

    // Pass @{code ActivityResultRegistry} reference explicitly to avoid crash
    // https://github.com/hnsvn/hns-browser/issues/31882
    ActivityResultLauncher<Intent> mIntentActivityResultLauncher = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(), getActivityResultRegistry(),
            result -> {
                HnsVpnUtils.dismissProgressDialog();
                if (result.getResultCode() == RESULT_OK) {
                    HnsVpnProfileUtils.getInstance().startVpn(HnsVpnParentActivity.this);
                    HnsVpnUtils.showVpnConfirmDialog(this);
                } else if (result.getResultCode() == RESULT_CANCELED) {
                    if (HnsVpnProfileUtils.getInstance().isVPNRunning(this)) {
                        HnsVpnUtils.showVpnAlwaysOnErrorDialog(this);
                    } else {
                        updateProfileView();
                    }
                    HnsVpnUtils.showToast(
                            getResources().getString(R.string.permission_was_cancelled));
                }
            });

    @Override
    public void finishNativeInitialization() {
        super.finishNativeInitialization();
    }

    @Override
    protected void onDestroy() {
        HnsVpnUtils.dismissProgressDialog();
        super.onDestroy();
    }

    protected void verifySubscription() {
        mHnsVpnPrefModel = new HnsVpnPrefModel();
        MutableLiveData<PurchaseModel> _activePurchases = new MutableLiveData();
        LiveData<PurchaseModel> activePurchases = _activePurchases;
        InAppPurchaseWrapper.getInstance().queryPurchases(_activePurchases);
        LiveDataUtil.observeOnce(activePurchases, activePurchaseModel -> {
            if (activePurchaseModel != null) {
                mHnsVpnPrefModel.setPurchaseToken(activePurchaseModel.getPurchaseToken());
                mHnsVpnPrefModel.setProductId(activePurchaseModel.getProductId());
                HnsVpnNativeWorker.getInstance().verifyPurchaseToken(
                        mHnsVpnPrefModel.getPurchaseToken(), mHnsVpnPrefModel.getProductId(),
                        HnsVpnUtils.SUBSCRIPTION_PARAM_TEXT, getPackageName());
            } else {
                if (!mIsVerification) {
                    HnsVpnApiResponseUtils.queryPurchaseFailed(HnsVpnParentActivity.this);
                } else {
                    showRestoreMenu(false);
                }
                HnsVpnUtils.dismissProgressDialog();
            }
        });
    }

    @Override
    public void onVerifyPurchaseToken(String jsonResponse, boolean isSuccess) {
        if (isSuccess && mHnsVpnPrefModel != null) {
            Long purchaseExpiry = HnsVpnUtils.getPurchaseExpiryDate(jsonResponse);
            int paymentState = HnsVpnUtils.getPaymentState(jsonResponse);
            if (purchaseExpiry > 0 && purchaseExpiry >= System.currentTimeMillis()) {
                HnsVpnPrefUtils.setPurchaseToken(mHnsVpnPrefModel.getPurchaseToken());
                HnsVpnPrefUtils.setProductId(mHnsVpnPrefModel.getProductId());
                HnsVpnPrefUtils.setPurchaseExpiry(purchaseExpiry);
                HnsVpnPrefUtils.setSubscriptionPurchase(true);
                HnsVpnPrefUtils.setPaymentState(paymentState);
                if (!mIsVerification || HnsVpnPrefUtils.isResetConfiguration()) {
                    HnsVpnNativeWorker.getInstance().getSubscriberCredential(
                            HnsVpnUtils.SUBSCRIPTION_PARAM_TEXT,
                            mHnsVpnPrefModel.getProductId(), HnsVpnUtils.IAP_ANDROID_PARAM_TEXT,
                            mHnsVpnPrefModel.getPurchaseToken(), getPackageName());
                } else {
                    mIsVerification = false;
                    showRestoreMenu(true);
                    HnsVpnUtils.showToast(getResources().getString(R.string.already_subscribed));
                    HnsVpnUtils.dismissProgressDialog();
                }
            } else {
                HnsVpnApiResponseUtils.queryPurchaseFailed(HnsVpnParentActivity.this);
                if (mIsVerification) {
                    mIsVerification = false;
                    showRestoreMenu(false);
                    HnsVpnUtils.dismissProgressDialog();
                } else {
                    HnsVpnUtils.openHnsVpnPlansActivity(HnsVpnParentActivity.this);
                }
            }
        } else {
            HnsVpnUtils.dismissProgressDialog();
        }
    };

    @Override
    public void onGetSubscriberCredential(String subscriberCredential, boolean isSuccess) {
        mHnsVpnPrefModel.setSubscriberCredential(subscriberCredential);
        HnsVpnApiResponseUtils.handleOnGetSubscriberCredential(
                HnsVpnParentActivity.this, isSuccess);
    };

    @Override
    public void onGetTimezonesForRegions(String jsonTimezones, boolean isSuccess) {
        HnsVpnApiResponseUtils.handleOnGetTimezonesForRegions(
                HnsVpnParentActivity.this, mHnsVpnPrefModel, jsonTimezones, isSuccess);
    }

    @Override
    public void onGetHostnamesForRegion(String jsonHostNames, boolean isSuccess) {
        KeyPair keyPair = new KeyPair();
        mHnsVpnPrefModel.setClientPrivateKey(keyPair.getPrivateKey().toBase64());
        mHnsVpnPrefModel.setClientPublicKey(keyPair.getPublicKey().toBase64());
        Pair<String, String> host = HnsVpnApiResponseUtils.handleOnGetHostnamesForRegion(
                HnsVpnParentActivity.this, mHnsVpnPrefModel, jsonHostNames, isSuccess);
        mHnsVpnPrefModel.setHostname(host.first);
        mHnsVpnPrefModel.setHostnameDisplay(host.second);
    }

    @Override
    public void onGetWireguardProfileCredentials(
            String jsonWireguardProfileCredentials, boolean isSuccess) {
        if (isSuccess && mHnsVpnPrefModel != null) {
            HnsVpnWireguardProfileCredentials hnsVpnWireguardProfileCredentials =
                    HnsVpnUtils.getWireguardProfileCredentials(jsonWireguardProfileCredentials);
            if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(this)) {
                HnsVpnProfileUtils.getInstance().stopVpn(this);
            }
            mHnsVpnPrefModel.setClientId(hnsVpnWireguardProfileCredentials.getClientId());
            mHnsVpnPrefModel.setApiAuthToken(
                    hnsVpnWireguardProfileCredentials.getApiAuthToken());
            HnsVpnPrefUtils.setPrefModel(mHnsVpnPrefModel);

            checkForVpn(hnsVpnWireguardProfileCredentials, mHnsVpnPrefModel);
        } else {
            HnsVpnUtils.showToast(getResources().getString(R.string.vpn_profile_creation_failed));
            HnsVpnUtils.dismissProgressDialog();
        }
    }

    private void checkForVpn(
            HnsVpnWireguardProfileCredentials hnsVpnWireguardProfileCredentials,
            HnsVpnPrefModel hnsVpnPrefModel) {
        new Thread() {
            @Override
            public void run() {
                try {
                    if (!WireguardConfigUtils.isConfigExist(getApplicationContext())) {
                        WireguardConfigUtils.createConfig(getApplicationContext(),
                                hnsVpnWireguardProfileCredentials.getMappedIpv4Address(),
                                hnsVpnPrefModel.getHostname(),
                                hnsVpnPrefModel.getClientPrivateKey(),
                                hnsVpnWireguardProfileCredentials.getServerPublicKey());
                    }

                    Intent intent = GoBackend.VpnService.prepare(HnsVpnParentActivity.this);
                    if (intent != null) {
                        mIntentActivityResultLauncher.launch(intent);
                        return;
                    }
                    HnsVpnUtils.dismissProgressDialog();
                    HnsVpnProfileUtils.getInstance().startVpn(HnsVpnParentActivity.this);
                    finish();
                } catch (Exception e) {
                    HnsVpnUtils.dismissProgressDialog();
                    Log.e(TAG, e.getMessage());
                }
            }
        }.start();
    }
}
