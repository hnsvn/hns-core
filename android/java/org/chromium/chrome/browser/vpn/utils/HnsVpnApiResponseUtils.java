/**
 * Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn.utils;

import android.app.Activity;
import android.text.TextUtils;
import android.util.Pair;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.chrome.browser.vpn.HnsVpnNativeWorker;
import org.chromium.chrome.browser.vpn.billing.InAppPurchaseWrapper;
import org.chromium.chrome.browser.vpn.billing.PurchaseModel;
import org.chromium.chrome.browser.vpn.models.HnsVpnPrefModel;

import java.util.TimeZone;

public class HnsVpnApiResponseUtils {
    public static void queryPurchaseFailed(Activity activity) {
        HnsVpnPrefUtils.setProductId("");
        HnsVpnPrefUtils.setPurchaseExpiry(0L);
        HnsVpnPrefUtils.setSubscriptionPurchase(false);
        HnsVpnPrefUtils.setPaymentState(0);
        if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(activity)) {
            HnsVpnProfileUtils.getInstance().stopVpn(activity);
        }
        HnsVpnUtils.showToast(
                activity.getResources().getString(R.string.purchase_token_verification_failed));
    }

    public static void handleOnGetSubscriberCredential(Activity activity, boolean isSuccess) {
        if (isSuccess) {
            if (!HnsVpnNativeWorker.getInstance().isPurchasedUser()) {
                MutableLiveData<PurchaseModel> _activePurchases = new MutableLiveData();
                LiveData<PurchaseModel> activePurchases = _activePurchases;
                InAppPurchaseWrapper.getInstance().queryPurchases(_activePurchases);
                LiveDataUtil.observeOnce(activePurchases, activePurchaseModel -> {
                    InAppPurchaseWrapper.getInstance().processPurchases(
                            activity, activePurchaseModel.getPurchase());
                });
            }
            HnsVpnNativeWorker.getInstance().getTimezonesForRegions();
        } else {
            HnsVpnUtils.showToast(
                    activity.getResources().getString(R.string.vpn_profile_creation_failed));
            HnsVpnUtils.dismissProgressDialog();
        }
    }

    public static void handleOnGetTimezonesForRegions(Activity activity,
            HnsVpnPrefModel hnsVpnPrefModel, String jsonTimezones, boolean isSuccess) {
        if (isSuccess) {
            String region = HnsVpnUtils.getRegionForTimeZone(
                    jsonTimezones, TimeZone.getDefault().getID());
            if (TextUtils.isEmpty(region)) {
                HnsVpnUtils.showToast(String.format(
                        activity.getResources().getString(R.string.couldnt_get_matching_timezone),
                        TimeZone.getDefault().getID()));
                return;
            }
            if (!TextUtils.isEmpty(HnsVpnUtils.selectedServerRegion)
                    && HnsVpnUtils.selectedServerRegion != null) {
                region = HnsVpnUtils.selectedServerRegion.equals(
                                 HnsVpnPrefUtils.PREF_HNS_VPN_AUTOMATIC)
                        ? region
                        : HnsVpnUtils.selectedServerRegion;
                HnsVpnUtils.selectedServerRegion = null;
            } else {
                String serverRegion = HnsVpnPrefUtils.getServerRegion();
                region = serverRegion.equals(HnsVpnPrefUtils.PREF_HNS_VPN_AUTOMATIC)
                        ? region
                        : serverRegion;
            }

            HnsVpnNativeWorker.getInstance().getHostnamesForRegion(region);
            hnsVpnPrefModel.setServerRegion(region);
        } else {
            HnsVpnUtils.showToast(
                    activity.getResources().getString(R.string.vpn_profile_creation_failed));
            HnsVpnUtils.dismissProgressDialog();
        }
    }

    public static Pair<String, String> handleOnGetHostnamesForRegion(Activity activity,
            HnsVpnPrefModel hnsVpnPrefModel, String jsonHostNames, boolean isSuccess) {
        Pair<String, String> host = new Pair<String, String>("", "");
        if (isSuccess && hnsVpnPrefModel != null) {
            host = HnsVpnUtils.getHostnameForRegion(jsonHostNames);
            HnsVpnNativeWorker.getInstance().getWireguardProfileCredentials(
                    hnsVpnPrefModel.getSubscriberCredential(),
                    hnsVpnPrefModel.getClientPublicKey(), host.first);
        } else {
            HnsVpnUtils.showToast(
                    activity.getResources().getString(R.string.vpn_profile_creation_failed));
            HnsVpnUtils.dismissProgressDialog();
        }
        return host;
    }
}
