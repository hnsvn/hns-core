/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Pair;

import androidx.appcompat.app.AlertDialog;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.preference.Preference;
import androidx.preference.PreferenceCategory;

import com.wireguard.android.backend.GoBackend;
import com.wireguard.crypto.KeyPair;

import org.chromium.base.HnsFeatureList;
import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.chrome.browser.InternetConnection;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.chrome.browser.vpn.HnsVpnNativeWorker;
import org.chromium.chrome.browser.vpn.HnsVpnObserver;
import org.chromium.chrome.browser.vpn.billing.InAppPurchaseWrapper;
import org.chromium.chrome.browser.vpn.billing.PurchaseModel;
import org.chromium.chrome.browser.vpn.models.HnsVpnPrefModel;
import org.chromium.chrome.browser.vpn.models.HnsVpnServerRegion;
import org.chromium.chrome.browser.vpn.models.HnsVpnWireguardProfileCredentials;
import org.chromium.chrome.browser.vpn.utils.HnsVpnApiResponseUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnProfileUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnUtils;
import org.chromium.chrome.browser.vpn.wireguard.WireguardConfigUtils;
import org.chromium.components.browser_ui.settings.ChromeBasePreference;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.ui.widget.Toast;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Locale;

public class HnsVpnPreferences extends HnsPreferenceFragment implements HnsVpnObserver {
    private static final String TAG = "HnsVPN";
    public static final String PREF_VPN_SWITCH = "vpn_switch";
    public static final String PREF_SUBSCRIPTION_MANAGE = "subscription_manage";
    public static final String PREF_LINK_SUBSCRIPTION = "link_subscription";
    public static final String PREF_SUBSCRIPTION_STATUS = "subscription_status";
    public static final String PREF_SUBSCRIPTION_EXPIRES = "subscription_expires";
    public static final String PREF_SERVER_HOST = "server_host";
    public static final String PREF_SERVER_CHANGE_LOCATION = "server_change_location";
    public static final String PREF_SUPPORT_TECHNICAL = "support_technical";
    public static final String PREF_SUPPORT_VPN = "support_vpn";
    public static final String PREF_SERVER_RESET_CONFIGURATION = "server_reset_configuration";
    private static final String PREF_SPLIT_TUNNELING = "split_tunneling";
    private static final String PREF_HNS_VPN_SUBSCRIPTION_SECTION =
            "hns_vpn_subscription_section";

    private static final int INVALIDATE_CREDENTIAL_TIMER_COUNT = 5000;

    private static final String VPN_SUPPORT_PAGE =
            "https://support.hns.com/hc/en-us/articles/4410838268429";
    private static final String MANAGE_SUBSCRIPTION_PAGE =
            "https://play.google.com/store/account/subscriptions";

    private static final String DATE_FORMAT = "dd/MM/yyyy";

    private ChromeSwitchPreference mVpnSwitch;
    private ChromeBasePreference mSubscriptionStatus;
    private ChromeBasePreference mSubscriptionExpires;
    private ChromeBasePreference mServerHost;
    private ChromeBasePreference mLinkSubscriptionPreference;
    private HnsVpnPrefModel mHnsVpnPrefModel;

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        getActivity().setTitle(R.string.hns_firewall_vpn);
        SettingsUtils.addPreferencesFromResource(this, R.xml.hns_vpn_preferences);

        mVpnSwitch = (ChromeSwitchPreference) findPreference(PREF_VPN_SWITCH);
        mVpnSwitch.setChecked(
                HnsVpnProfileUtils.getInstance().isHnsVPNConnected(getActivity()));
        mVpnSwitch.setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
            @Override
            public boolean onPreferenceClick(Preference preference) {
                if (mVpnSwitch != null) {
                    mVpnSwitch.setChecked(
                            HnsVpnProfileUtils.getInstance().isHnsVPNConnected(getActivity()));
                }
                if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(getActivity())) {
                    HnsVpnUtils.showProgressDialog(
                            getActivity(), getResources().getString(R.string.vpn_disconnect_text));
                    HnsVpnProfileUtils.getInstance().stopVpn(getActivity());
                } else {
                    if (HnsVpnNativeWorker.getInstance().isPurchasedUser()) {
                        HnsVpnPrefUtils.setSubscriptionPurchase(true);
                        if (WireguardConfigUtils.isConfigExist(getActivity())) {
                            HnsVpnProfileUtils.getInstance().startVpn(getActivity());
                        } else {
                            HnsVpnUtils.openHnsVpnProfileActivity(getActivity());
                        }
                    } else {
                        HnsVpnUtils.showProgressDialog(
                                getActivity(), getResources().getString(R.string.vpn_connect_text));
                        if (HnsVpnPrefUtils.isSubscriptionPurchase()) {
                            verifyPurchase(true);
                        } else {
                            HnsVpnUtils.openHnsVpnPlansActivity(getActivity());
                            HnsVpnUtils.dismissProgressDialog();
                        }
                    }
                }
                return false;
            }
        });

        mSubscriptionStatus = (ChromeBasePreference) findPreference(PREF_SUBSCRIPTION_STATUS);
        mSubscriptionExpires = (ChromeBasePreference) findPreference(PREF_SUBSCRIPTION_EXPIRES);

        mServerHost = (ChromeBasePreference) findPreference(PREF_SERVER_HOST);

        findPreference(PREF_SUPPORT_TECHNICAL)
                .setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        HnsVpnUtils.openHnsVpnSupportActivity(getActivity());
                        return true;
                    }
                });

        findPreference(PREF_SUPPORT_VPN)
                .setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        CustomTabActivity.showInfoPage(getActivity(), VPN_SUPPORT_PAGE);
                        return true;
                    }
                });

        findPreference(PREF_SUBSCRIPTION_MANAGE)
                .setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        Intent browserIntent =
                                new Intent(Intent.ACTION_VIEW, Uri.parse(MANAGE_SUBSCRIPTION_PAGE));
                        getActivity().startActivity(browserIntent);
                        return true;
                    }
                });

        findPreference(PREF_SERVER_RESET_CONFIGURATION)
                .setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        showConfirmDialog();
                        return true;
                    }
                });

        findPreference(PREF_SPLIT_TUNNELING)
                .setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        HnsVpnUtils.openSplitTunnelActivity(getActivity());
                        return true;
                    }
                });
        mLinkSubscriptionPreference = new ChromeBasePreference(getActivity());
        mLinkSubscriptionPreference.setTitle(
                getResources().getString(R.string.link_subscription_title));
        mLinkSubscriptionPreference.setSummary(
                getResources().getString(R.string.link_subscription_text));
        mLinkSubscriptionPreference.setKey(PREF_LINK_SUBSCRIPTION);
        mLinkSubscriptionPreference.setVisible(
                ChromeFeatureList.isEnabled(HnsFeatureList.HNS_VPN_LINK_SUBSCRIPTION_ANDROID_UI)
                && HnsVpnPrefUtils.isSubscriptionPurchase());
        mLinkSubscriptionPreference.setOnPreferenceClickListener(
                new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        Intent intent = new Intent(getActivity(), ChromeTabbedActivity.class);
                        intent.putExtra(HnsActivity.OPEN_URL, HnsVpnUtils.getHnsAccountUrl());
                        intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
                        intent.setAction(Intent.ACTION_VIEW);
                        getActivity().finish();
                        startActivity(intent);
                        return true;
                    }
                });
        PreferenceCategory preferenceCategory =
                (PreferenceCategory) findPreference(PREF_HNS_VPN_SUBSCRIPTION_SECTION);
        preferenceCategory.addPreference(mLinkSubscriptionPreference);
        preferenceCategory.setVisible(!HnsVpnNativeWorker.getInstance().isPurchasedUser());
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context != null) {
            ConnectivityManager connectivityManager =
                    (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkRequest networkRequest =
                    new NetworkRequest.Builder()
                            .addTransportType(NetworkCapabilities.TRANSPORT_VPN)
                            .removeCapability(NetworkCapabilities.NET_CAPABILITY_NOT_VPN)
                            .build();
            connectivityManager.registerNetworkCallback(networkRequest, mNetworkCallback);
        }
        HnsVpnNativeWorker.getInstance().getAllServerRegions();
        if (!InternetConnection.isNetworkAvailable(getActivity())) {
            Toast.makeText(getActivity(), R.string.no_internet, Toast.LENGTH_SHORT).show();
            getActivity().finish();
        }
    }

    @Override
    public void onGetAllServerRegions(String jsonResponse, boolean isSuccess) {
        if (isSuccess) {
            HnsVpnPrefUtils.setServerRegions(jsonResponse);
            new Handler().post(() -> updateSummaries());
        } else {
            Toast.makeText(getActivity(), R.string.fail_to_get_server_locations, Toast.LENGTH_LONG)
                    .show();
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        if (HnsVpnUtils.mIsServerLocationChanged) {
            HnsVpnUtils.showProgressDialog(
                    getActivity(), getResources().getString(R.string.vpn_connect_text));
            if (HnsVpnNativeWorker.getInstance().isPurchasedUser()) {
                mHnsVpnPrefModel = new HnsVpnPrefModel();
                HnsVpnNativeWorker.getInstance().getSubscriberCredentialV12();
            } else {
                verifyPurchase(false);
            }
        } else if (HnsVpnUtils.mUpdateProfileAfterSplitTunnel) {
            HnsVpnUtils.mUpdateProfileAfterSplitTunnel = false;
            HnsVpnUtils.showProgressDialog(
                    getActivity(), getResources().getString(R.string.updating_vpn_profile));
            HnsVpnUtils.updateProfileConfiguration(getActivity());
            new Handler().post(() -> updateSummaries());
        } else {
            HnsVpnUtils.dismissProgressDialog();
        }
        if (mLinkSubscriptionPreference != null) {
            mLinkSubscriptionPreference.setVisible(
                    ChromeFeatureList.isEnabled(
                            HnsFeatureList.HNS_VPN_LINK_SUBSCRIPTION_ANDROID_UI)
                    && HnsVpnPrefUtils.isSubscriptionPurchase());
        }
    }

    private void updateSummary(String preferenceString, String summary) {
        Preference p = findPreference(preferenceString);
        p.setSummary(summary);
    }

    private void updateSummaries() {
        if (getActivity() == null) {
            return;
        }
        List<HnsVpnServerRegion> vpnServerRegions =
                HnsVpnUtils.getServerLocations(HnsVpnPrefUtils.getServerRegions());
        String serverLocation = "";
        for (HnsVpnServerRegion vpnServerRegion : vpnServerRegions) {
            if (HnsVpnPrefUtils.getServerRegion().equals(
                        HnsVpnPrefUtils.PREF_HNS_VPN_AUTOMATIC)) {
                serverLocation = getActivity().getResources().getString(R.string.automatic);
            }
            if (vpnServerRegion.getName().equals(HnsVpnPrefUtils.getServerRegion())) {
                serverLocation = vpnServerRegion.getNamePretty();
                break;
            }
        }
        updateSummary(PREF_SERVER_CHANGE_LOCATION, serverLocation);
        updateSummary(PREF_SERVER_HOST, HnsVpnPrefUtils.getHostnameDisplay());
        if (!HnsVpnPrefUtils.getProductId().isEmpty()) {
            String subscriptionStatus = String.format(
                    InAppPurchaseWrapper.getInstance().isMonthlySubscription(
                            HnsVpnPrefUtils.getProductId())
                            ? getActivity().getResources().getString(R.string.monthly_subscription)
                            : getActivity().getResources().getString(R.string.yearly_subscription),
                    (HnsVpnPrefUtils.isTrialSubscription()
                                    ? getActivity().getResources().getString(R.string.trial)
                                    : ""));
            updateSummary(PREF_SUBSCRIPTION_STATUS, subscriptionStatus);
        }

        if (HnsVpnPrefUtils.getPurchaseExpiry() > 0) {
            long expiresInMillis = HnsVpnPrefUtils.getPurchaseExpiry();
            SimpleDateFormat formatter = new SimpleDateFormat(DATE_FORMAT, Locale.getDefault());
            updateSummary(PREF_SUBSCRIPTION_EXPIRES, formatter.format(new Date(expiresInMillis)));
        }
        if (mVpnSwitch != null) {
            mVpnSwitch.setChecked(
                    HnsVpnProfileUtils.getInstance().isHnsVPNConnected(getActivity()));
        }
        new Thread() {
            @Override
            public void run() {
                if (getActivity() != null) {
                    getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            findPreference(PREF_SERVER_CHANGE_LOCATION)
                                    .setEnabled(WireguardConfigUtils.isConfigExist(getActivity()));
                            findPreference(PREF_SPLIT_TUNNELING)
                                    .setEnabled(WireguardConfigUtils.isConfigExist(getActivity()));
                        }
                    });
                }
            }
        }.start();
        HnsVpnUtils.dismissProgressDialog();
    }

    private final ConnectivityManager.NetworkCallback mNetworkCallback =
            new ConnectivityManager.NetworkCallback() {
                @Override
                public void onAvailable(Network network) {
                    HnsVpnUtils.dismissProgressDialog();
                    if (getActivity() != null) {
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                new Handler().post(() -> updateSummaries());
                            }
                        });
                    }
                }

                @Override
                public void onLost(Network network) {
                    HnsVpnUtils.dismissProgressDialog();
                    if (getActivity() != null) {
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                new Handler().post(() -> updateSummaries());
                            }
                        });
                    }
                }
            };

    private void verifyPurchase(boolean isVerification) {
        MutableLiveData<PurchaseModel> _activePurchases = new MutableLiveData();
        LiveData<PurchaseModel> activePurchases = _activePurchases;
        InAppPurchaseWrapper.getInstance().queryPurchases(_activePurchases);
        LiveDataUtil.observeOnce(activePurchases, activePurchaseModel -> {
            mHnsVpnPrefModel = new HnsVpnPrefModel();
            if (activePurchaseModel != null) {
                mHnsVpnPrefModel.setPurchaseToken(activePurchaseModel.getPurchaseToken());
                mHnsVpnPrefModel.setProductId(activePurchaseModel.getProductId());
                if (HnsVpnPrefUtils.isResetConfiguration()) {
                    HnsVpnUtils.dismissProgressDialog();
                    HnsVpnUtils.openHnsVpnProfileActivity(getActivity());
                    return;
                }
                if (!isVerification) {
                    HnsVpnNativeWorker.getInstance().getSubscriberCredential(
                            HnsVpnUtils.SUBSCRIPTION_PARAM_TEXT,
                            mHnsVpnPrefModel.getProductId(), HnsVpnUtils.IAP_ANDROID_PARAM_TEXT,
                            mHnsVpnPrefModel.getPurchaseToken(), getActivity().getPackageName());
                } else {
                    HnsVpnNativeWorker.getInstance().verifyPurchaseToken(
                            mHnsVpnPrefModel.getPurchaseToken(),
                            mHnsVpnPrefModel.getProductId(),
                            HnsVpnUtils.SUBSCRIPTION_PARAM_TEXT, getActivity().getPackageName());
                }
            } else {
                HnsVpnApiResponseUtils.queryPurchaseFailed(getActivity());
                HnsVpnUtils.mIsServerLocationChanged = false;
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
                if (mSubscriptionStatus != null) {
                    String subscriptionStatus = String.format(
                            InAppPurchaseWrapper.getInstance().isMonthlySubscription(
                                    HnsVpnPrefUtils.getProductId())
                                    ? getActivity().getResources().getString(
                                            R.string.monthly_subscription)
                                    : getActivity().getResources().getString(
                                            R.string.yearly_subscription),
                            (HnsVpnPrefUtils.isTrialSubscription()
                                            ? getActivity().getResources().getString(R.string.trial)
                                            : ""));
                    mSubscriptionStatus.setSummary(subscriptionStatus);
                }

                if (mSubscriptionExpires != null) {
                    SimpleDateFormat formatter =
                            new SimpleDateFormat(DATE_FORMAT, Locale.getDefault());
                    mSubscriptionExpires.setSummary(formatter.format(new Date(purchaseExpiry)));
                }
                checkVpnAfterVerification();
            } else {
                HnsVpnApiResponseUtils.queryPurchaseFailed(getActivity());
            }
        }
    };

    private void checkVpnAfterVerification() {
        new Thread() {
            @Override
            public void run() {
                Intent intent = GoBackend.VpnService.prepare(getActivity());
                if (intent != null || !WireguardConfigUtils.isConfigExist(getActivity())) {
                    HnsVpnUtils.dismissProgressDialog();
                    HnsVpnUtils.openHnsVpnProfileActivity(getActivity());
                    return;
                }
                HnsVpnProfileUtils.getInstance().startVpn(getActivity());
            }
        }.start();
    }

    @Override
    public void onGetSubscriberCredential(String subscriberCredential, boolean isSuccess) {
        mHnsVpnPrefModel.setSubscriberCredential(subscriberCredential);
        HnsVpnApiResponseUtils.handleOnGetSubscriberCredential(getActivity(), isSuccess);
    };

    @Override
    public void onGetTimezonesForRegions(String jsonTimezones, boolean isSuccess) {
        HnsVpnApiResponseUtils.handleOnGetTimezonesForRegions(
                getActivity(), mHnsVpnPrefModel, jsonTimezones, isSuccess);
    }

    @Override
    public void onGetHostnamesForRegion(String jsonHostNames, boolean isSuccess) {
        KeyPair keyPair = new KeyPair();
        mHnsVpnPrefModel.setClientPrivateKey(keyPair.getPrivateKey().toBase64());
        mHnsVpnPrefModel.setClientPublicKey(keyPair.getPublicKey().toBase64());
        Pair<String, String> host = HnsVpnApiResponseUtils.handleOnGetHostnamesForRegion(
                getActivity(), mHnsVpnPrefModel, jsonHostNames, isSuccess);
        mHnsVpnPrefModel.setHostname(host.first);
        mHnsVpnPrefModel.setHostnameDisplay(host.second);
    }

    @Override
    public void onGetWireguardProfileCredentials(
            String jsonWireguardProfileCredentials, boolean isSuccess) {
        if (isSuccess && mHnsVpnPrefModel != null) {
            HnsVpnWireguardProfileCredentials hnsVpnWireguardProfileCredentials =
                    HnsVpnUtils.getWireguardProfileCredentials(jsonWireguardProfileCredentials);

            int timerCount = 0;
            if (HnsVpnUtils.mIsServerLocationChanged) {
                timerCount = INVALIDATE_CREDENTIAL_TIMER_COUNT;
                HnsVpnUtils.mIsServerLocationChanged = false;
                try {
                    HnsVpnNativeWorker.getInstance().invalidateCredentials(
                            HnsVpnPrefUtils.getHostname(), HnsVpnPrefUtils.getClientId(),
                            HnsVpnPrefUtils.getSubscriberCredential(),
                            HnsVpnPrefUtils.getApiAuthToken());
                } catch (Exception ex) {
                    Log.e(TAG, ex.getMessage());
                }
            }

            new Handler().postDelayed(() -> {
                stopStartConnection(hnsVpnWireguardProfileCredentials);
            }, timerCount);
        } else {
            Toast.makeText(getActivity(), R.string.vpn_profile_creation_failed, Toast.LENGTH_LONG)
                    .show();
            HnsVpnUtils.dismissProgressDialog();
            new Handler().post(() -> updateSummaries());
        }
    }

    private void stopStartConnection(
            HnsVpnWireguardProfileCredentials hnsVpnWireguardProfileCredentials) {
        new Thread() {
            @Override
            public void run() {
                try {
                    if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(getActivity())) {
                        HnsVpnProfileUtils.getInstance().stopVpn(getActivity());
                    }
                    WireguardConfigUtils.deleteConfig(getActivity());
                    if (!WireguardConfigUtils.isConfigExist(getActivity())) {
                        WireguardConfigUtils.createConfig(getActivity(),
                                hnsVpnWireguardProfileCredentials.getMappedIpv4Address(),
                                mHnsVpnPrefModel.getHostname(),
                                mHnsVpnPrefModel.getClientPrivateKey(),
                                hnsVpnWireguardProfileCredentials.getServerPublicKey());
                    }
                    HnsVpnProfileUtils.getInstance().startVpn(getActivity());
                } catch (Exception e) {
                    Log.e(TAG, e.getMessage());
                }
                mHnsVpnPrefModel.setClientId(hnsVpnWireguardProfileCredentials.getClientId());
                mHnsVpnPrefModel.setApiAuthToken(
                        hnsVpnWireguardProfileCredentials.getApiAuthToken());
                HnsVpnPrefUtils.setPrefModel(mHnsVpnPrefModel);
                new Handler(Looper.getMainLooper()).post(() -> updateSummaries());
            }
        }.start();
    }

    @Override
    public void onStart() {
        super.onStart();
        HnsVpnNativeWorker.getInstance().addObserver(this);
    }

    @Override
    public void onStop() {
        HnsVpnNativeWorker.getInstance().removeObserver(this);
        super.onStop();
    }

    private void showConfirmDialog() {
        AlertDialog.Builder confirmDialog = new AlertDialog.Builder(getActivity());
        confirmDialog.setTitle(
                getActivity().getResources().getString(R.string.reset_vpn_config_dialog_title));
        confirmDialog.setMessage(
                getActivity().getResources().getString(R.string.reset_vpn_config_dialog_message));
        confirmDialog.setPositiveButton(
                getActivity().getResources().getString(android.R.string.yes), (dialog, which) -> {
                    resetConfiguration();
                    dialog.dismiss();
                });
        confirmDialog.setNegativeButton(getActivity().getResources().getString(android.R.string.no),
                (dialog, which) -> { dialog.dismiss(); });
        confirmDialog.show();
    }

    private void resetConfiguration() {
        HnsVpnNativeWorker.getInstance().invalidateCredentials(HnsVpnPrefUtils.getHostname(),
                HnsVpnPrefUtils.getClientId(), HnsVpnPrefUtils.getSubscriberCredential(),
                HnsVpnPrefUtils.getApiAuthToken());
        HnsVpnUtils.showProgressDialog(
                getActivity(), getResources().getString(R.string.resetting_config));
        new Handler().postDelayed(() -> {
            if (isResumed()) {
                HnsVpnUtils.resetProfileConfiguration(getActivity());
                new Handler().post(() -> updateSummaries());
            }
        }, INVALIDATE_CREDENTIAL_TIMER_COUNT);
    }

    @Override
    public void onDestroy() {
        HnsVpnUtils.dismissProgressDialog();
        super.onDestroy();
    }
}
