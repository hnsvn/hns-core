/**
 * Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn.utils;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.text.TextUtils;
import android.util.Pair;

import androidx.fragment.app.FragmentActivity;

import com.wireguard.config.Config;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.chrome.browser.HnsRewardsNativeWorker;
import org.chromium.chrome.browser.util.HnsConstants;
import org.chromium.chrome.browser.vpn.HnsVpnNativeWorker;
import org.chromium.chrome.browser.vpn.activities.HnsVpnPlansActivity;
import org.chromium.chrome.browser.vpn.activities.HnsVpnProfileActivity;
import org.chromium.chrome.browser.vpn.activities.HnsVpnSupportActivity;
import org.chromium.chrome.browser.vpn.fragments.HnsVpnAlwaysOnErrorDialogFragment;
import org.chromium.chrome.browser.vpn.fragments.HnsVpnConfirmDialogFragment;
import org.chromium.chrome.browser.vpn.models.HnsVpnServerRegion;
import org.chromium.chrome.browser.vpn.models.HnsVpnWireguardProfileCredentials;
import org.chromium.chrome.browser.vpn.split_tunnel.SplitTunnelActivity;
import org.chromium.chrome.browser.vpn.wireguard.WireguardConfigUtils;
import org.chromium.gms.ChromiumPlayServicesAvailability;
import org.chromium.ui.widget.Toast;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class HnsVpnUtils {
    private static final String TAG = "HnsVPN";
    public static final String SUBSCRIPTION_PARAM_TEXT = "subscription";
    public static final String IAP_ANDROID_PARAM_TEXT = "iap-android";
    public static final String VERIFY_CREDENTIALS_FAILED = "verify_credentials_failed";
    public static final String DESKTOP_CREDENTIAL = "desktop_credential";

    private static final String HNS_ACCOUNT_PROD_PAGE_URL =
            "https://account.hns.com?intent=connect-receipt&product=vpn";
    private static final String HNS_ACCOUNT_STAGING_PAGE_URL =
            "https://account.hnssoftware.com?intent=connect-receipt&product=vpn";

    public static boolean mIsServerLocationChanged;
    public static boolean mUpdateProfileAfterSplitTunnel;
    public static String selectedServerRegion;
    private static ProgressDialog sProgressDialog;

    public static String getHnsAccountUrl() {
        return HnsVpnPrefUtils.isLinkSubscriptionOnStaging() ? HNS_ACCOUNT_STAGING_PAGE_URL
                                                               : HNS_ACCOUNT_PROD_PAGE_URL;
    }

    public static void openHnsVpnPlansActivity(Activity activity) {
        Intent hnsVpnPlanIntent = new Intent(activity, HnsVpnPlansActivity.class);
        hnsVpnPlanIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        hnsVpnPlanIntent.setAction(Intent.ACTION_VIEW);
        activity.startActivity(hnsVpnPlanIntent);
    }

    public static void openHnsVpnProfileActivity(Activity activity) {
        Intent hnsVpnProfileIntent = new Intent(activity, HnsVpnProfileActivity.class);
        hnsVpnProfileIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        hnsVpnProfileIntent.setAction(Intent.ACTION_VIEW);
        activity.startActivity(hnsVpnProfileIntent);
    }

    public static void openHnsVpnSupportActivity(Activity activity) {
        Intent hnsVpnSupportIntent = new Intent(activity, HnsVpnSupportActivity.class);
        hnsVpnSupportIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        hnsVpnSupportIntent.setAction(Intent.ACTION_VIEW);
        activity.startActivity(hnsVpnSupportIntent);
    }

    public static void openSplitTunnelActivity(Activity activity) {
        Intent hnsVpnSupportIntent = new Intent(activity, SplitTunnelActivity.class);
        hnsVpnSupportIntent.setAction(Intent.ACTION_VIEW);
        activity.startActivity(hnsVpnSupportIntent);
    }

    public static void showProgressDialog(Activity activity, String message) {
        sProgressDialog = ProgressDialog.show(activity, "", message, true);
    }

    public static void dismissProgressDialog() {
        if (sProgressDialog != null && sProgressDialog.isShowing()) {
            sProgressDialog.dismiss();
        }
    }

    public static String getRegionForTimeZone(String jsonTimezones, String currentTimezone) {
        // Add root element to make it real JSON, otherwise getJSONArray cannot parse it
        jsonTimezones = "{\"regions\":" + jsonTimezones + "}";
        try {
            JSONObject result = new JSONObject(jsonTimezones);
            JSONArray regions = result.getJSONArray("regions");
            for (int i = 0; i < regions.length(); i++) {
                JSONObject region = regions.getJSONObject(i);
                JSONArray timezones = region.getJSONArray("timezones");
                for (int j = 0; j < timezones.length(); j++) {
                    if (timezones.getString(j).equals(currentTimezone)) {
                        return region.getString("name");
                    }
                }
            }
        } catch (JSONException e) {
            Log.e(TAG, "HnsVpnUtils -> getRegionForTimeZone JSONException error " + e);
        }
        return "";
    }

    public static Pair<String, String> getHostnameForRegion(String jsonHostnames) {
        jsonHostnames = "{\"hostnames\":" + jsonHostnames + "}";
        try {
            JSONObject result = new JSONObject(jsonHostnames);
            JSONArray hostnames = result.getJSONArray("hostnames");
            ArrayList<JSONObject> hosts = new ArrayList<JSONObject>();
            for (int i = 0; i < hostnames.length(); i++) {
                JSONObject hostname = hostnames.getJSONObject(i);
                if (hostname.getInt("capacity-score") == 0
                        || hostname.getInt("capacity-score") == 1) {
                    hosts.add(hostname);
                }
            }

            JSONObject hostname;
            if (hosts.size() < 2) {
                hostname = hostnames.getJSONObject(new Random().nextInt(hostnames.length()));
            } else {
                hostname = hosts.get(new Random().nextInt(hosts.size()));
            }
            return new Pair<>(hostname.getString("hostname"), hostname.getString("display-name"));
        } catch (JSONException e) {
            Log.e(TAG, "HnsVpnUtils -> getHostnameForRegion JSONException error " + e);
        }
        return new Pair<String, String>("", "");
    }

    public static HnsVpnWireguardProfileCredentials getWireguardProfileCredentials(
            String jsonWireguardProfileCredentials) {
        try {
            JSONObject wireguardProfileCredentials =
                    new JSONObject(jsonWireguardProfileCredentials);
            HnsVpnWireguardProfileCredentials hnsVpnWireguardProfileCredentials =
                    new HnsVpnWireguardProfileCredentials(
                            wireguardProfileCredentials.getString("api-auth-token"),
                            wireguardProfileCredentials.getString("client-id"),
                            wireguardProfileCredentials.getString("mapped-ipv4-address"),
                            wireguardProfileCredentials.getString("mapped-ipv6-address"),
                            wireguardProfileCredentials.getString("server-public-key"));
            return hnsVpnWireguardProfileCredentials;
        } catch (JSONException e) {
            Log.e(TAG, "HnsVpnUtils -> getWireguardProfileCredentials JSONException error " + e);
        }
        return null;
    }

    public static Long getPurchaseExpiryDate(String json) {
        try {
            JSONObject purchase = new JSONObject(json);
            String expiryTimeInString = purchase.getString("expiryTimeMillis");
            return Long.parseLong(expiryTimeInString);
        } catch (JSONException | NumberFormatException e) {
            Log.e(TAG,
                    "HnsVpnUtils -> getPurchaseExpiryDate JSONException | NumberFormatException error "
                            + e);
        }
        return 0L;
    }

    public static int getPaymentState(String json) {
        try {
            JSONObject purchase = new JSONObject(json);
            int paymentState = purchase.getInt("paymentState");
            return paymentState;
        } catch (JSONException e) {
            Log.e(TAG, "HnsVpnUtils -> getPaymentState JSONException error " + e);
        }
        return 0;
    }

    public static List<HnsVpnServerRegion> getServerLocations(String jsonServerLocations) {
        List<HnsVpnServerRegion> vpnServerRegions = new ArrayList<>();
        if (TextUtils.isEmpty(jsonServerLocations)) {
            return vpnServerRegions;
        }
        jsonServerLocations = "{\"servers\":" + jsonServerLocations + "}";
        try {
            JSONObject result = new JSONObject(jsonServerLocations);
            JSONArray servers = result.getJSONArray("servers");
            for (int i = 0; i < servers.length(); i++) {
                JSONObject server = servers.getJSONObject(i);
                HnsVpnServerRegion vpnServerRegion =
                        new HnsVpnServerRegion(server.getString("continent"),
                                server.getString("name"), server.getString("name-pretty"));
                vpnServerRegions.add(vpnServerRegion);
            }
        } catch (JSONException e) {
            Log.e(TAG, "HnsVpnUtils -> getServerLocations JSONException error " + e);
        }
        return vpnServerRegions;
    }

    public static void resetProfileConfiguration(Activity activity) {
        if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(activity)) {
            HnsVpnProfileUtils.getInstance().stopVpn(activity);
        }
        try {
            WireguardConfigUtils.deleteConfig(activity);
        } catch (Exception ex) {
            Log.e(TAG, "resetProfileConfiguration : " + ex.getMessage());
        }
        HnsVpnPrefUtils.setResetConfiguration(true);
        dismissProgressDialog();
    }

    public static void updateProfileConfiguration(Activity activity) {
        try {
            Config existingConfig = WireguardConfigUtils.loadConfig(activity);
            WireguardConfigUtils.deleteConfig(activity);
            WireguardConfigUtils.createConfig(activity, existingConfig);
        } catch (Exception ex) {
            Log.e(TAG, "updateProfileConfiguration : " + ex.getMessage());
        }
        if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(activity)) {
            HnsVpnProfileUtils.getInstance().stopVpn(activity);
            HnsVpnProfileUtils.getInstance().startVpn(activity);
        }
        dismissProgressDialog();
    }

    public static void showVpnAlwaysOnErrorDialog(Activity activity) {
        HnsVpnAlwaysOnErrorDialogFragment mHnsVpnAlwaysOnErrorDialogFragment =
                new HnsVpnAlwaysOnErrorDialogFragment();
        mHnsVpnAlwaysOnErrorDialogFragment.show(
                ((FragmentActivity) activity).getSupportFragmentManager(),
                "HnsVpnAlwaysOnErrorDialogFragment");
    }

    public static void showVpnConfirmDialog(Activity activity) {
        HnsVpnConfirmDialogFragment hnsVpnConfirmDialogFragment =
                new HnsVpnConfirmDialogFragment();
        hnsVpnConfirmDialogFragment.show(
                ((FragmentActivity) activity).getSupportFragmentManager(),
                "HnsVpnConfirmDialogFragment");
    }

    public static void reportBackgroundUsageP3A() {
        // Will report previous/current session timestamps...
        HnsVpnNativeWorker.getInstance().reportBackgroundP3A(
                HnsVpnPrefUtils.getSessionStartTimeMs(), HnsVpnPrefUtils.getSessionEndTimeMs());
        // ...and then reset the timestamps so we don't report the same session again.
        HnsVpnPrefUtils.setSessionStartTimeMs(-1);
        HnsVpnPrefUtils.setSessionEndTimeMs(-1);
    }

    public static void showToast(String message) {
        Context context = ContextUtils.getApplicationContext();
        Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
    }

    private static boolean isRegionSupported() {
        HnsRewardsNativeWorker hnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
        return (hnsRewardsNativeWorker != null && hnsRewardsNativeWorker.IsSupported());
    }

    private static boolean isHnsVpnFeatureEnable() {
        if ((ContextUtils.getApplicationContext().getPackageName().equals(
                     HnsConstants.HNS_PRODUCTION_PACKAGE_NAME)
                    || HnsVpnPrefUtils.isHnsVpnFeatureEnabled())) {
            return true;
        }
        return false;
    }

    public static boolean isVpnFeatureSupported(Context context) {
        return isHnsVpnFeatureEnable() && isRegionSupported()
                && ChromiumPlayServicesAvailability.isGooglePlayServicesAvailable(context);
    }
}
