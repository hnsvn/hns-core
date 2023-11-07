/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.app;

import static org.chromium.ui.base.ViewUtils.dpToPx;

import android.app.Activity;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.MainThread;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.coordinatorlayout.widget.CoordinatorLayout;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.hns.playlist.util.ConstantUtils;
import com.hns.playlist.util.PlaylistPreferenceUtils;
import com.hns.playlist.util.PlaylistUtils;
import com.google.android.material.dialog.MaterialAlertDialogBuilder;
import com.wireguard.android.backend.GoBackend;

import org.chromium.base.ApplicationStatus;
import org.chromium.base.HnsFeatureList;
import org.chromium.base.HnsPreferenceKeys;
import org.chromium.base.HnsReflectionUtil;
import org.chromium.base.CollectionUtil;
import org.chromium.base.CommandLine;
import org.chromium.base.ContextUtils;
import org.chromium.base.IntentUtils;
import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.UnownedUserDataSupplier;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.hns_news.mojom.HnsNewsController;
import org.chromium.hns_wallet.mojom.AssetRatioService;
import org.chromium.hns_wallet.mojom.BlockchainRegistry;
import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.CoinType;
import org.chromium.hns_wallet.mojom.EthTxManagerProxy;
import org.chromium.hns_wallet.mojom.JsonRpcService;
import org.chromium.hns_wallet.mojom.KeyringService;
import org.chromium.hns_wallet.mojom.NetworkInfo;
import org.chromium.hns_wallet.mojom.SolanaTxManagerProxy;
import org.chromium.hns_wallet.mojom.SwapService;
import org.chromium.hns_wallet.mojom.TxService;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.ApplicationLifetime;
import org.chromium.chrome.browser.HnsAdFreeCalloutDialogFragment;
import org.chromium.chrome.browser.HnsFeatureUtil;
import org.chromium.chrome.browser.HnsHelper;
import org.chromium.chrome.browser.HnsRelaunchUtils;
import org.chromium.chrome.browser.HnsRewardsHelper;
import org.chromium.chrome.browser.HnsSyncInformers;
import org.chromium.chrome.browser.HnsSyncWorker;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.chrome.browser.CrossPromotionalModalDialogFragment;
import org.chromium.chrome.browser.DormantUsersEngagementDialogFragment;
import org.chromium.chrome.browser.InternetConnection;
import org.chromium.chrome.browser.LaunchIntentDispatcher;
import org.chromium.chrome.browser.app.domain.WalletModel;
import org.chromium.chrome.browser.bookmarks.TabBookmarker;
import org.chromium.chrome.browser.hns_news.HnsNewsConnectionErrorHandler;
import org.chromium.chrome.browser.hns_news.HnsNewsControllerFactory;
import org.chromium.chrome.browser.hns_news.HnsNewsUtils;
import org.chromium.chrome.browser.hns_news.models.FeedItemsCard;
import org.chromium.chrome.browser.hns_stats.HnsStatsBottomSheetDialogFragment;
import org.chromium.chrome.browser.hns_stats.HnsStatsUtil;
import org.chromium.chrome.browser.browsing_data.BrowsingDataBridge;
import org.chromium.chrome.browser.browsing_data.BrowsingDataType;
import org.chromium.chrome.browser.browsing_data.TimePeriod;
import org.chromium.chrome.browser.crypto_wallet.AssetRatioServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.BlockchainRegistryFactory;
import org.chromium.chrome.browser.crypto_wallet.HnsWalletServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.JsonRpcServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.KeyringServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.SwapServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.TxServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.activities.AddAccountActivity;
import org.chromium.chrome.browser.crypto_wallet.activities.HnsWalletActivity;
import org.chromium.chrome.browser.crypto_wallet.activities.HnsWalletDAppsActivity;
import org.chromium.chrome.browser.crypto_wallet.model.CryptoAccountTypeInfo;
import org.chromium.chrome.browser.crypto_wallet.util.Utils;
import org.chromium.chrome.browser.custom_layout.popup_window_tooltip.PopupWindowTooltip;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.flags.ChromeSwitches;
import org.chromium.chrome.browser.fullscreen.BrowserControlsManager;
import org.chromium.chrome.browser.informers.HnsAndroidSyncDisabledInformer;
import org.chromium.chrome.browser.informers.HnsSyncAccountDeletedInformer;
import org.chromium.chrome.browser.misc_metrics.PrivacyHubMetricsConnectionErrorHandler;
import org.chromium.chrome.browser.misc_metrics.PrivacyHubMetricsFactory;
import org.chromium.chrome.browser.notifications.HnsNotificationWarningDialog;
import org.chromium.chrome.browser.notifications.permissions.NotificationPermissionController;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationUtil;
import org.chromium.chrome.browser.ntp.NewTabPageManager;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.onboarding.v2.HighlightDialogFragment;
import org.chromium.chrome.browser.onboarding.v2.HighlightItem;
import org.chromium.chrome.browser.onboarding.v2.HighlightView;
import org.chromium.chrome.browser.playlist.PlaylistHostActivity;
import org.chromium.chrome.browser.playlist.PlaylistWarningDialogFragment;
import org.chromium.chrome.browser.playlist.PlaylistWarningDialogFragment.PlaylistWarningDialogListener;
import org.chromium.chrome.browser.playlist.settings.HnsPlaylistPreferences;
import org.chromium.chrome.browser.preferences.HnsPref;
import org.chromium.chrome.browser.preferences.HnsPrefServiceBridge;
import org.chromium.chrome.browser.preferences.PrefChangeRegistrar;
import org.chromium.chrome.browser.preferences.PrefChangeRegistrar.PrefObserver;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.preferences.website.HnsShieldsContentSettings;
import org.chromium.chrome.browser.prefetch.settings.PreloadPagesSettingsBridge;
import org.chromium.chrome.browser.prefetch.settings.PreloadPagesState;
import org.chromium.chrome.browser.privacy.settings.HnsPrivacySettings;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.rate.HnsRateDialogFragment;
import org.chromium.chrome.browser.rate.RateUtils;
import org.chromium.chrome.browser.rewards.adaptive_captcha.AdaptiveCaptchaHelper;
import org.chromium.chrome.browser.safe_browsing.SafeBrowsingBridge;
import org.chromium.chrome.browser.safe_browsing.SafeBrowsingState;
import org.chromium.chrome.browser.set_default_browser.HnsSetDefaultBrowserUtils;
import org.chromium.chrome.browser.set_default_browser.OnHnsSetDefaultBrowserListener;
import org.chromium.chrome.browser.settings.HnsNewsPreferencesV2;
import org.chromium.chrome.browser.settings.HnsSearchEngineUtils;
import org.chromium.chrome.browser.settings.HnsWalletPreferences;
import org.chromium.chrome.browser.settings.SettingsLauncherImpl;
import org.chromium.chrome.browser.settings.developer.HnsQAPreferences;
import org.chromium.chrome.browser.share.ShareDelegate;
import org.chromium.chrome.browser.share.ShareDelegate.ShareOrigin;
import org.chromium.chrome.browser.site_settings.HnsWalletEthereumConnectedSites;
import org.chromium.chrome.browser.speedreader.HnsSpeedReaderUtils;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabImpl;
import org.chromium.chrome.browser.tab.TabLaunchType;
import org.chromium.chrome.browser.tab.TabSelectionType;
import org.chromium.chrome.browser.tabmodel.TabModel;
import org.chromium.chrome.browser.tabmodel.TabModelUtils;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.toolbar.top.HnsToolbarLayoutImpl;
import org.chromium.chrome.browser.util.HnsConstants;
import org.chromium.chrome.browser.util.HnsDbUtil;
import org.chromium.chrome.browser.util.ConfigurationUtils;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.vpn.HnsVpnNativeWorker;
import org.chromium.chrome.browser.vpn.HnsVpnObserver;
import org.chromium.chrome.browser.vpn.activities.HnsVpnProfileActivity;
import org.chromium.chrome.browser.vpn.billing.InAppPurchaseWrapper;
import org.chromium.chrome.browser.vpn.billing.PurchaseModel;
import org.chromium.chrome.browser.vpn.fragments.HnsVpnCalloutDialogFragment;
import org.chromium.chrome.browser.vpn.fragments.LinkVpnSubscriptionDialogFragment;
import org.chromium.chrome.browser.vpn.utils.HnsVpnApiResponseUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnProfileUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnUtils;
import org.chromium.chrome.browser.vpn.wireguard.WireguardConfigUtils;
import org.chromium.components.browser_ui.settings.SettingsLauncher;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.components.embedder_support.util.UrlUtilities;
import org.chromium.components.safe_browsing.HnsSafeBrowsingApiHandler;
import org.chromium.components.search_engines.TemplateUrl;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.content_public.browser.WebContents;
import org.chromium.misc_metrics.mojom.PrivacyHubMetrics;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.ui.widget.Toast;

import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Hns's extension for ChromeActivity
 */
@JNINamespace("chrome::android")
public abstract class HnsActivity extends ChromeActivity
        implements BrowsingDataBridge.OnClearBrowsingDataListener, HnsVpnObserver,
                   OnHnsSetDefaultBrowserListener, ConnectionErrorHandler, PrefObserver,
                   HnsSafeBrowsingApiHandler.HnsSafeBrowsingApiHandlerDelegate,
                   HnsNewsConnectionErrorHandler.HnsNewsConnectionErrorHandlerDelegate,
                   PrivacyHubMetricsConnectionErrorHandler
                           .PrivacyHubMetricsConnectionErrorHandlerDelegate {
    public static final String HNS_BUY_URL = "hns://wallet/fund-wallet";
    public static final String HNS_SEND_URL = "hns://wallet/send";
    public static final String HNS_SWAP_URL = "hns://wallet/swap";
    public static final String HNS_DEPOSIT_URL = "hns://wallet/deposit-funds";
    public static final String HNS_REWARDS_SETTINGS_URL = "hns://rewards/";
    public static final String HNS_REWARDS_SETTINGS_WALLET_VERIFICATION_URL =
            "hns://rewards/#verify";
    public static final String HNS_REWARDS_SETTINGS_MONTHLY_URL = "hns://rewards/#monthly";
    public static final String REWARDS_AC_SETTINGS_URL = "hns://rewards/contribute";
    public static final String REWARDS_LEARN_MORE_URL = "https://hns.com/faq-rewards/#unclaimed-funds";
    public static final String HNS_TERMS_PAGE =
            "https://basicattentiontoken.org/user-terms-of-service/";
    public static final String HNS_PRIVACY_POLICY = "https://hns.com/privacy/browser/#rewards";
    private static final String PREF_CLOSE_TABS_ON_EXIT = "close_tabs_on_exit";
    private static final String PREF_CLEAR_ON_EXIT = "clear_on_exit";
    public static final String OPEN_URL = "open_url";

    private static final int DAYS_1 = 1;
    private static final int DAYS_4 = 4;
    private static final int DAYS_5 = 5;
    private static final int DAYS_12 = 12;

    public static final int MAX_FAILED_CAPTCHA_ATTEMPTS = 10;

    public static final int APP_OPEN_COUNT_FOR_WIDGET_PROMO = 25;

    /**
     * Settings for sending local notification reminders.
     */
    public static final String CHANNEL_ID = "com.hns.browser";

    // Explicitly declare this variable to avoid build errors.
    // It will be removed in asm and parent variable will be used instead.
    private UnownedUserDataSupplier<BrowserControlsManager> mBrowserControlsManagerSupplier;

    private static final List<String> sYandexRegions =
            Arrays.asList("AM", "AZ", "BY", "KG", "KZ", "MD", "RU", "TJ", "TM", "UZ");

    private String mPurchaseToken = "";
    private String mProductId = "";
    private boolean mIsVerification;
    private boolean mIsDefaultCheckOnResume;
    private boolean mIsSetDefaultBrowserNotification;
    public boolean mIsDeepLink;
    private HnsWalletService mHnsWalletService;
    private KeyringService mKeyringService;
    private JsonRpcService mJsonRpcService;
    private PrivacyHubMetrics mPrivacyHubMetrics;
    private SwapService mSwapService;
    private WalletModel mWalletModel;
    private BlockchainRegistry mBlockchainRegistry;
    private TxService mTxService;
    private EthTxManagerProxy mEthTxManagerProxy;
    private SolanaTxManagerProxy mSolanaTxManagerProxy;
    private AssetRatioService mAssetRatioService;
    public boolean mLoadedFeed;
    public boolean mComesFromNewTab;
    public CopyOnWriteArrayList<FeedItemsCard> mNewsItemsFeedCards;
    private boolean mIsProcessingPendingDappsTxRequest;
    private int mLastTabId;
    private boolean mNativeInitialized;
    private boolean mSafeBrowsingFlagEnabled;
    private NewTabPageManager mNewTabPageManager;
    private NotificationPermissionController mNotificationPermissionController;
    private HnsNewsController mHnsNewsController;
    private HnsNewsConnectionErrorHandler mHnsNewsConnectionErrorHandler;
    private PrivacyHubMetricsConnectionErrorHandler mPrivacyHubMetricsConnectionErrorHandler;

    /**
     * Serves as a general exception for failed attempts to get HnsActivity.
     */
    public static class HnsActivityNotFoundException extends Exception {
        public HnsActivityNotFoundException(String message) {
            super(message);
        }
    }

    public HnsActivity() {}

    @Override
    public void onResumeWithNative() {
        super.onResumeWithNative();
        HnsActivityJni.get().restartStatsUpdater();
        if (HnsVpnUtils.isVpnFeatureSupported(HnsActivity.this)) {
            HnsVpnNativeWorker.getInstance().addObserver(this);
            HnsVpnUtils.reportBackgroundUsageP3A();
        }
        Profile profile = getCurrentTabModel().getProfile();
        if (profile != null) {
            // Set proper active DSE whenever hns returns to foreground.
            // If active tab is private, set private DSE as an active DSE.
            HnsSearchEngineUtils.updateActiveDSE(profile);
        }

        // The check on mNativeInitialized is mostly to ensure that mojo
        // services for wallet are initialized.
        // TODO(sergz): verify do we need it in that phase or not.
        if (mNativeInitialized) {
            HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
            if (layout == null || !layout.isWalletIconVisible()) {
                return;
            }
            updateWalletBadgeVisibility();
        }

        HnsSafeBrowsingApiHandler.getInstance().setDelegate(
                HnsActivityJni.get().getSafeBrowsingApiKey(), this);
        // We can store a state of that flag as a browser has to be restarted
        // when the flag state is changed in any case
        mSafeBrowsingFlagEnabled =
                ChromeFeatureList.isEnabled(HnsFeatureList.HNS_ANDROID_SAFE_BROWSING);

        executeInitSafeBrowsing(0);
    }

    @Override
    public void onPauseWithNative() {
        if (HnsVpnUtils.isVpnFeatureSupported(HnsActivity.this)) {
            HnsVpnNativeWorker.getInstance().removeObserver(this);
        }
        Profile profile = getCurrentTabModel().getProfile();
        if (profile != null && profile.isOffTheRecord()) {
            // Set normal DSE as an active DSE when hns goes in background
            // because currently set DSE is used by outside of hns(ex, hns search widget).
            HnsSearchEngineUtils.updateActiveDSE(profile);
        }
        super.onPauseWithNative();
    }

    @Override
    public boolean onMenuOrKeyboardAction(int id, boolean fromMenu) {
        final TabImpl currentTab = (TabImpl) getActivityTab();
        // Handle items replaced by Hns.
        if (id == R.id.info_menu_id && currentTab != null) {
            ShareDelegate shareDelegate = (ShareDelegate) getShareDelegateSupplier().get();
            shareDelegate.share(currentTab, false, ShareOrigin.OVERFLOW_MENU);
            return true;
        } else if (id == R.id.reload_menu_id) {
            setComesFromNewTab(true);
        }

        if (super.onMenuOrKeyboardAction(id, fromMenu)) {
            return true;
        }

        // Handle items added by Hns.
        if (currentTab == null) {
            return false;
        } else if (id == R.id.exit_id) {
            ApplicationLifetime.terminate(false);
        } else if (id == R.id.set_default_browser) {
            HnsSetDefaultBrowserUtils.showHnsSetDefaultBrowserDialog(HnsActivity.this, true);
        } else if (id == R.id.hns_rewards_id) {
            openNewOrSelectExistingTab(HNS_REWARDS_SETTINGS_URL);
        } else if (id == R.id.hns_wallet_id) {
            openHnsWallet(false, false, false);
        } else if (id == R.id.hns_playlist_id) {
            openPlaylist(true);
        } else if (id == R.id.hns_news_id) {
            openHnsNewsSettings();
        } else if (id == R.id.request_hns_vpn_id || id == R.id.request_hns_vpn_check_id) {
            if (!InternetConnection.isNetworkAvailable(HnsActivity.this)) {
                Toast.makeText(HnsActivity.this, R.string.no_internet, Toast.LENGTH_SHORT).show();
            } else {
                if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(HnsActivity.this)) {
                    HnsVpnUtils.showProgressDialog(HnsActivity.this,
                            getResources().getString(R.string.vpn_disconnect_text));
                    HnsVpnProfileUtils.getInstance().stopVpn(HnsActivity.this);
                    HnsVpnUtils.dismissProgressDialog();
                } else {
                    if (HnsVpnNativeWorker.getInstance().isPurchasedUser()) {
                        HnsVpnPrefUtils.setSubscriptionPurchase(true);
                        if (WireguardConfigUtils.isConfigExist(HnsActivity.this)) {
                            HnsVpnProfileUtils.getInstance().startVpn(HnsActivity.this);
                        } else {
                            HnsVpnUtils.openHnsVpnProfileActivity(HnsActivity.this);
                        }
                    } else {
                        HnsVpnUtils.showProgressDialog(HnsActivity.this,
                                getResources().getString(R.string.vpn_connect_text));
                        if (HnsVpnPrefUtils.isSubscriptionPurchase()) {
                            verifySubscription();
                        } else {
                            HnsVpnUtils.dismissProgressDialog();
                            HnsVpnUtils.openHnsVpnPlansActivity(HnsActivity.this);
                        }
                    }
                }
            }
        } else if (id == R.id.hns_speedreader_id) {
            enableSpeedreaderMode();
        } else {
            return false;
        }
        return true;
    }

    @Override
    public void cleanUpHnsNewsController() {
        if (mHnsNewsController != null) {
            mHnsNewsController.close();
        }
        mHnsNewsController = null;
    }

    // Handles only wallet related mojo failures. Don't add handlers for mojo connections that
    // are not related to wallet functionality.
    @Override
    public void onConnectionError(MojoException e) {
        cleanUpWalletNativeServices();
        initWalletNativeServices();
    }

    @Override
    protected void onDestroyInternal() {
        if (mNotificationPermissionController != null) {
            NotificationPermissionController.detach(mNotificationPermissionController);
            mNotificationPermissionController = null;
        }
        HnsSafeBrowsingApiHandler.getInstance().shutdownSafeBrowsing();
        super.onDestroyInternal();
        cleanUpHnsNewsController();
        cleanUpWalletNativeServices();
        cleanUpPrivacyHubMetrics();
    }

    public WalletModel getWalletModel() {
        return mWalletModel;
    }

    private void maybeHasPendingUnlockRequest() {
        assert mKeyringService != null;
        mKeyringService.hasPendingUnlockRequest(pending -> {
            if (pending) {
                HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
                if (layout != null) {
                    layout.showWalletPanel();
                }

                return;
            }
            maybeShowPendingTransactions();
            maybeShowSignTxRequestLayout();
        });
    }

    private void setWalletBadgeVisibility(boolean visibile) {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.updateWalletBadgeVisibility(visibile);
        }
    }

    private void maybeShowPendingTransactions() {
        assert mWalletModel != null;
        // trigger to observer to refresh data to process the pending request
        mWalletModel.getCryptoModel().refreshTransactions();
    }

    private void maybeShowSignTxRequestLayout() {
        assert mHnsWalletService != null;
        mHnsWalletService.getPendingSignTransactionRequests(requests -> {
            if (requests != null && requests.length != 0) {
                openHnsWalletDAppsActivity(
                        HnsWalletDAppsActivity.ActivityType.SIGN_TRANSACTION);
                return;
            }
            maybeShowSignAllTxRequestLayout();
        });
    }

    private void maybeShowSignAllTxRequestLayout() {
        assert mHnsWalletService != null;
        mHnsWalletService.getPendingSignAllTransactionsRequests(requests -> {
            if (requests != null && requests.length != 0) {
                openHnsWalletDAppsActivity(
                        HnsWalletDAppsActivity.ActivityType.SIGN_ALL_TRANSACTIONS);
                return;
            }
            maybeShowSignMessageRequestLayout();
        });
    }

    private void maybeShowSignMessageRequestLayout() {
        assert mHnsWalletService != null;
        mHnsWalletService.getPendingSignMessageRequests(requests -> {
            if (requests != null && requests.length != 0) {
                openHnsWalletDAppsActivity(HnsWalletDAppsActivity.ActivityType.SIGN_MESSAGE);

                return;
            }
            maybeShowChainRequestLayout();
        });
    }

    private void maybeShowChainRequestLayout() {
        assert mJsonRpcService != null;
        mJsonRpcService.getPendingAddChainRequests(networks -> {
            if (networks != null && networks.length != 0) {
                openHnsWalletDAppsActivity(
                        HnsWalletDAppsActivity.ActivityType.ADD_ETHEREUM_CHAIN);

                return;
            }
            maybeShowSwitchChainRequestLayout();
        });
    }

    private void maybeShowSwitchChainRequestLayout() {
        assert mJsonRpcService != null;
        mJsonRpcService.getPendingSwitchChainRequests(requests -> {
            if (requests != null && requests.length != 0) {
                openHnsWalletDAppsActivity(
                        HnsWalletDAppsActivity.ActivityType.SWITCH_ETHEREUM_CHAIN);

                return;
            }
            maybeShowAddSuggestTokenRequestLayout();
        });
    }

    private void maybeShowAddSuggestTokenRequestLayout() {
        assert mHnsWalletService != null;
        mHnsWalletService.getPendingAddSuggestTokenRequests(requests -> {
            if (requests != null && requests.length != 0) {
                openHnsWalletDAppsActivity(HnsWalletDAppsActivity.ActivityType.ADD_TOKEN);

                return;
            }
            maybeShowGetEncryptionPublicKeyRequestLayout();
        });
    }

    private void maybeShowGetEncryptionPublicKeyRequestLayout() {
        assert mHnsWalletService != null;
        mHnsWalletService.getPendingGetEncryptionPublicKeyRequests(requests -> {
            if (requests != null && requests.length != 0) {
                openHnsWalletDAppsActivity(
                        HnsWalletDAppsActivity.ActivityType.GET_ENCRYPTION_PUBLIC_KEY_REQUEST);

                return;
            }
            maybeShowDecryptRequestLayout();
        });
    }

    private void maybeShowDecryptRequestLayout() {
        assert mHnsWalletService != null;
        mHnsWalletService.getPendingDecryptRequests(requests -> {
            if (requests != null && requests.length != 0) {
                openHnsWalletDAppsActivity(HnsWalletDAppsActivity.ActivityType.DECRYPT_REQUEST);

                return;
            }
            HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
            if (layout != null) {
                layout.showWalletPanel();
            }
        });
    }

    public void dismissWalletPanelOrDialog() {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.dismissWalletPanelOrDialog();
        }
    }

    public void showWalletPanel(boolean ignoreWeb3NotificationPreference) {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.showWalletIcon(true);
        }
        if (!ignoreWeb3NotificationPreference
                && !HnsWalletPreferences.getPrefWeb3NotificationsEnabled()) {
            return;
        }
        assert mKeyringService != null;
        mKeyringService.isLocked(locked -> {
            if (locked) {
                layout.showWalletPanel();
                return;
            }
            maybeHasPendingUnlockRequest();
        });
    }

    public void showWalletOnboarding() {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.showWalletIcon(true);
            if (!HnsWalletPreferences.getPrefWeb3NotificationsEnabled()) {
                return;
            }
            layout.showWalletPanel();
        }
    }

    public void walletInteractionDetected(WebContents webContents) {
        Tab tab = getActivityTab();
        if (tab == null
                || !webContents.getLastCommittedUrl().equals(
                        tab.getWebContents().getLastCommittedUrl())) {
            return;
        }
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.showWalletIcon(true);
            updateWalletBadgeVisibility();
        }
    }

    public void showAccountCreation(@CoinType.EnumType int coinType) {
        assert mWalletModel != null : " mWalletModel is null ";
        mWalletModel.getDappsModel().addAccountCreationRequest(coinType);
    }

    private void updateWalletBadgeVisibility() {
        assert mWalletModel != null;
        mWalletModel.getDappsModel().updateWalletBadgeVisibility();
    }

    private void verifySubscription() {
        MutableLiveData<PurchaseModel> _activePurchases = new MutableLiveData();
        LiveData<PurchaseModel> activePurchases = _activePurchases;
        InAppPurchaseWrapper.getInstance().queryPurchases(_activePurchases);
        LiveDataUtil.observeOnce(activePurchases, activePurchaseModel -> {
            if (activePurchaseModel != null) {
                mPurchaseToken = activePurchaseModel.getPurchaseToken();
                mProductId = activePurchaseModel.getProductId();
                HnsVpnNativeWorker.getInstance().verifyPurchaseToken(mPurchaseToken, mProductId,
                        HnsVpnUtils.SUBSCRIPTION_PARAM_TEXT, getPackageName());
            } else {
                HnsVpnApiResponseUtils.queryPurchaseFailed(HnsActivity.this);
                if (!mIsVerification) {
                    HnsVpnUtils.openHnsVpnPlansActivity(HnsActivity.this);
                }
            }
        });
    }

    @Override
    public void onVerifyPurchaseToken(String jsonResponse, boolean isSuccess) {
        if (isSuccess) {
            Long purchaseExpiry = HnsVpnUtils.getPurchaseExpiryDate(jsonResponse);
            int paymentState = HnsVpnUtils.getPaymentState(jsonResponse);
            if (purchaseExpiry > 0 && purchaseExpiry >= System.currentTimeMillis()) {
                HnsVpnPrefUtils.setPurchaseToken(mPurchaseToken);
                HnsVpnPrefUtils.setProductId(mProductId);
                HnsVpnPrefUtils.setPurchaseExpiry(purchaseExpiry);
                HnsVpnPrefUtils.setSubscriptionPurchase(true);
                HnsVpnPrefUtils.setPaymentState(paymentState);
                if (HnsVpnPrefUtils.isResetConfiguration()) {
                    HnsVpnUtils.dismissProgressDialog();
                    HnsVpnUtils.openHnsVpnProfileActivity(HnsActivity.this);
                } else {
                    if (!mIsVerification) {
                        checkForVpn();
                    } else {
                        mIsVerification = false;
                        if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(
                                    HnsActivity.this)
                                && !TextUtils.isEmpty(HnsVpnPrefUtils.getHostname())
                                && !TextUtils.isEmpty(HnsVpnPrefUtils.getClientId())
                                && !TextUtils.isEmpty(HnsVpnPrefUtils.getSubscriberCredential())
                                && !TextUtils.isEmpty(HnsVpnPrefUtils.getApiAuthToken())) {
                            HnsVpnNativeWorker.getInstance().verifyCredentials(
                                    HnsVpnPrefUtils.getHostname(),
                                    HnsVpnPrefUtils.getClientId(),
                                    HnsVpnPrefUtils.getSubscriberCredential(),
                                    HnsVpnPrefUtils.getApiAuthToken());
                        }
                    }
                    HnsVpnUtils.dismissProgressDialog();
                }
            } else {
                HnsVpnApiResponseUtils.queryPurchaseFailed(HnsActivity.this);
                if (!mIsVerification) {
                    HnsVpnUtils.openHnsVpnPlansActivity(HnsActivity.this);
                }
                mIsVerification = false;
            }
            mPurchaseToken = "";
            mProductId = "";
        } else {
            HnsVpnApiResponseUtils.queryPurchaseFailed(HnsActivity.this);
            if (!mIsVerification) {
                HnsVpnUtils.openHnsVpnPlansActivity(HnsActivity.this);
            }
            mIsVerification = false;
        }
    };

    private void checkForVpn() {
        HnsVpnNativeWorker.getInstance().reportForegroundP3A();
        new Thread() {
            @Override
            public void run() {
                Intent intent = GoBackend.VpnService.prepare(HnsActivity.this);
                if (intent != null
                        || !WireguardConfigUtils.isConfigExist(getApplicationContext())) {
                    HnsVpnUtils.dismissProgressDialog();
                    HnsVpnUtils.openHnsVpnProfileActivity(HnsActivity.this);
                    return;
                }
                HnsVpnProfileUtils.getInstance().startVpn(HnsActivity.this);
            }
        }.start();
    }

    @Override
    public void onVerifyCredentials(String jsonVerifyCredentials, boolean isSuccess) {
        if (!isSuccess) {
            if (HnsVpnProfileUtils.getInstance().isHnsVPNConnected(HnsActivity.this)) {
                HnsVpnProfileUtils.getInstance().stopVpn(HnsActivity.this);
            }
            Intent hnsVpnProfileIntent =
                    new Intent(HnsActivity.this, HnsVpnProfileActivity.class);
            hnsVpnProfileIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            hnsVpnProfileIntent.putExtra(HnsVpnUtils.VERIFY_CREDENTIALS_FAILED, true);
            hnsVpnProfileIntent.setAction(Intent.ACTION_VIEW);
            startActivity(hnsVpnProfileIntent);
        }
    }

    @Override
    public void initializeState() {
        super.initializeState();
        if (isNoRestoreState()) {
            CommandLine.getInstance().appendSwitch(ChromeSwitches.NO_RESTORE_STATE);
        }

        if (isClearBrowsingDataOnExit()) {
            List<Integer> dataTypes = Arrays.asList(
                    BrowsingDataType.HISTORY, BrowsingDataType.COOKIES, BrowsingDataType.CACHE);

            int[] dataTypesArray = CollectionUtil.integerCollectionToIntArray(dataTypes);

            // has onBrowsingDataCleared() as an @Override callback from implementing
            // BrowsingDataBridge.OnClearBrowsingDataListener
            BrowsingDataBridge.getInstance().clearBrowsingData(
                    this, dataTypesArray, TimePeriod.ALL_TIME);
        }

        setLoadedFeed(false);
        setComesFromNewTab(false);
        setNewsItemsFeedCards(null);
        HnsSearchEngineUtils.initializeHnsSearchEngineStates(getTabModelSelector());
        Intent intent = getIntent();
        if (intent != null && intent.getBooleanExtra(Utils.RESTART_WALLET_ACTIVITY, false)) {
            openHnsWallet(false,
                    intent.getBooleanExtra(Utils.RESTART_WALLET_ACTIVITY_SETUP, false),
                    intent.getBooleanExtra(Utils.RESTART_WALLET_ACTIVITY_RESTORE, false));
        }
    }

    public int getLastTabId() {
        return mLastTabId;
    }

    public void setLastTabId(int lastTabId) {
        this.mLastTabId = lastTabId;
    }

    public boolean isLoadedFeed() {
        return mLoadedFeed;
    }

    public void setLoadedFeed(boolean loadedFeed) {
        this.mLoadedFeed = loadedFeed;
    }

    public CopyOnWriteArrayList<FeedItemsCard> getNewsItemsFeedCards() {
        return mNewsItemsFeedCards;
    }

    public void setNewsItemsFeedCards(CopyOnWriteArrayList<FeedItemsCard> newsItemsFeedCards) {
        this.mNewsItemsFeedCards = newsItemsFeedCards;
    }

    public void setComesFromNewTab(boolean comesFromNewTab) {
        this.mComesFromNewTab = comesFromNewTab;
    }

    public boolean isComesFromNewTab() {
        return mComesFromNewTab;
    }

    @Override
    public void onBrowsingDataCleared() {}

    @Override
    public void OnCheckDefaultResume() {
        mIsDefaultCheckOnResume = true;
    }

    @Override
    public void onResume() {
        super.onResume();
        mIsProcessingPendingDappsTxRequest = false;
        if (mIsDefaultCheckOnResume) {
            mIsDefaultCheckOnResume = false;

            if (HnsSetDefaultBrowserUtils.isHnsSetAsDefaultBrowser(this)) {
                HnsSetDefaultBrowserUtils.setHnsDefaultSuccess();
            }
        }

        PostTask.postTask(
                TaskTraits.BEST_EFFORT_MAY_BLOCK, () -> { HnsStatsUtil.removeShareStatsFile(); });

        // We need to enable widget promo for later release
        /* int appOpenCountForWidgetPromo = SharedPreferencesManager.getInstance().readInt(
                HnsPreferenceKeys.HNS_APP_OPEN_COUNT_FOR_WIDGET_PROMO);
        if (appOpenCountForWidgetPromo < APP_OPEN_COUNT_FOR_WIDGET_PROMO) {
            SharedPreferencesManager.getInstance().writeInt(
                    HnsPreferenceKeys.HNS_APP_OPEN_COUNT_FOR_WIDGET_PROMO,
                    appOpenCountForWidgetPromo + 1);
        } */
    }

    @Override
    public void performPostInflationStartup() {
        super.performPostInflationStartup();

        createNotificationChannel();
    }

    @Override
    protected void initializeStartupMetrics() {
        super.initializeStartupMetrics();

        // Disable FRE for arm64 builds where ChromeActivity is the one that
        // triggers FRE instead of ChromeLauncherActivity on arm32 build.
        HnsHelper.DisableFREDRP();
    }

    @Override
    public void onPreferenceChange() {
        String captchaID = UserPrefs.get(Profile.getLastUsedRegularProfile())
                                   .getString(HnsPref.SCHEDULED_CAPTCHA_ID);
        String paymentID = UserPrefs.get(Profile.getLastUsedRegularProfile())
                                   .getString(HnsPref.SCHEDULED_CAPTCHA_PAYMENT_ID);
        if (HnsQAPreferences.shouldVlogRewards()) {
            Log.e(AdaptiveCaptchaHelper.TAG,
                    "captchaID : " + captchaID + " Payment ID : " + paymentID);
        }
        maybeSolveAdaptiveCaptcha();
    }

    @Override
    public void turnSafeBrowsingOff() {
        SafeBrowsingBridge.setSafeBrowsingState(SafeBrowsingState.NO_SAFE_BROWSING);
    }

    @Override
    public boolean isSafeBrowsingEnabled() {
        return mSafeBrowsingFlagEnabled;
    }

    @Override
    public Activity getActivity() {
        return this;
    }

    public void maybeSolveAdaptiveCaptcha() {
        String captchaID = UserPrefs.get(Profile.getLastUsedRegularProfile())
                                   .getString(HnsPref.SCHEDULED_CAPTCHA_ID);
        String paymentID = UserPrefs.get(Profile.getLastUsedRegularProfile())
                                   .getString(HnsPref.SCHEDULED_CAPTCHA_PAYMENT_ID);
        if (!TextUtils.isEmpty(captchaID) && !TextUtils.isEmpty(paymentID)
                && !HnsPrefServiceBridge.getInstance().getSafetynetCheckFailed()) {
            AdaptiveCaptchaHelper.startAttestation(captchaID, paymentID);
        }
    }

    @Override
    public void finishNativeInitialization() {
        super.finishNativeInitialization();
        HnsVpnNativeWorker.getInstance().reloadPurchasedState();

        HnsHelper.maybeMigrateSettings();

        PrefChangeRegistrar mPrefChangeRegistrar = new PrefChangeRegistrar();
        mPrefChangeRegistrar.addObserver(HnsPref.SCHEDULED_CAPTCHA_ID, this);

        if (UserPrefs.get(Profile.getLastUsedRegularProfile())
                        .getInteger(HnsPref.SCHEDULED_CAPTCHA_FAILED_ATTEMPTS)
                >= MAX_FAILED_CAPTCHA_ATTEMPTS) {
            UserPrefs.get(Profile.getLastUsedRegularProfile())
                    .setBoolean(HnsPref.SCHEDULED_CAPTCHA_PAUSED, true);
        }

        if (HnsQAPreferences.shouldVlogRewards()) {
            Log.e(AdaptiveCaptchaHelper.TAG,
                    "Failed attempts : "
                            + UserPrefs.get(Profile.getLastUsedRegularProfile())
                                      .getInteger(HnsPref.SCHEDULED_CAPTCHA_FAILED_ATTEMPTS));
        }
        if (!UserPrefs.get(Profile.getLastUsedRegularProfile())
                        .getBoolean(HnsPref.SCHEDULED_CAPTCHA_PAUSED)) {
            maybeSolveAdaptiveCaptcha();
        }

        if (SharedPreferencesManager.getInstance().readBoolean(
                    HnsPreferenceKeys.HNS_DOUBLE_RESTART, false)) {
            SharedPreferencesManager.getInstance().writeBoolean(
                    HnsPreferenceKeys.HNS_DOUBLE_RESTART, false);
            HnsRelaunchUtils.restart();
            return;
        }

        // Make sure this option is disabled
        if (PreloadPagesSettingsBridge.getState() != PreloadPagesState.NO_PRELOADING) {
            PreloadPagesSettingsBridge.setState(PreloadPagesState.NO_PRELOADING);
        }

        if (HnsRewardsHelper.hasRewardsEnvChange()) {
            HnsPrefServiceBridge.getInstance().resetPromotionLastFetchStamp();
            HnsRewardsHelper.setRewardsEnvChange(false);
        }

        int appOpenCount = SharedPreferencesManager.getInstance().readInt(HnsPreferenceKeys.HNS_APP_OPEN_COUNT);
        SharedPreferencesManager.getInstance().writeInt(HnsPreferenceKeys.HNS_APP_OPEN_COUNT, appOpenCount + 1);

        if (PackageUtils.isFirstInstall(this) && appOpenCount == 0) {
            checkForYandexSE();
        }

        migrateBgPlaybackToFeature();

        Context app = ContextUtils.getApplicationContext();
        if (null != app
                && HnsReflectionUtil.EqualTypes(this.getClass(), ChromeTabbedActivity.class)) {
            // Trigger HnsSyncWorker CTOR to make migration from sync v1 if sync is enabled
            HnsSyncWorker.get();
        }

        checkForNotificationData();

        if (RateUtils.getInstance().isLastSessionShown()) {
            RateUtils.getInstance().setPrefNextRateDate();
            RateUtils.getInstance().setLastSessionShown(false);
        }

        if (!RateUtils.getInstance().getPrefRateEnabled()) {
            RateUtils.getInstance().setPrefRateEnabled(true);
            RateUtils.getInstance().setPrefNextRateDate();
        }
        RateUtils.getInstance().setTodayDate();

        if (RateUtils.getInstance().shouldShowRateDialog(this)) {
            showHnsRateDialog();
            RateUtils.getInstance().setLastSessionShown(true);
        }

        // TODO commenting out below code as we may use it in next release

        // if (PackageUtils.isFirstInstall(this)
        //         &&
        //         SharedPreferencesManager.getInstance().readInt(HnsPreferenceKeys.HNS_APP_OPEN_COUNT)
        //         == 1) {
        //     Calendar calender = Calendar.getInstance();
        //     calender.setTime(new Date());
        //     calender.add(Calendar.DATE, DAYS_4);
        //     OnboardingPrefManager.getInstance().setNextOnboardingDate(
        //         calender.getTimeInMillis());
        // }

        // OnboardingActivity onboardingActivity = null;
        // for (Activity ref : ApplicationStatus.getRunningActivities()) {
        //     if (!(ref instanceof OnboardingActivity)) continue;

        //     onboardingActivity = (OnboardingActivity) ref;
        // }

        // if (onboardingActivity == null
        //         && OnboardingPrefManager.getInstance().showOnboardingForSkip(this)) {
        //     OnboardingPrefManager.getInstance().showOnboarding(this);
        //     OnboardingPrefManager.getInstance().setOnboardingShownForSkip(true);
        // }

        if (SharedPreferencesManager.getInstance().readInt(HnsPreferenceKeys.HNS_APP_OPEN_COUNT) == 1) {
            Calendar calender = Calendar.getInstance();
            calender.setTime(new Date());
            calender.add(Calendar.DATE, DAYS_12);
            OnboardingPrefManager.getInstance().setNextCrossPromoModalDate(
                    calender.getTimeInMillis());
        }

        if (OnboardingPrefManager.getInstance().showCrossPromoModal()) {
            showCrossPromotionalDialog();
            OnboardingPrefManager.getInstance().setCrossPromoModalShown(true);
        }
        HnsSyncInformers.show();
        HnsAndroidSyncDisabledInformer.showInformers();
        HnsSyncAccountDeletedInformer.show();

        if (!OnboardingPrefManager.getInstance().isOneTimeNotificationStarted()
                && PackageUtils.isFirstInstall(this)) {
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.HOUR_3);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.HOUR_24);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DAY_6);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DAY_10);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DAY_30);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DAY_35);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DEFAULT_BROWSER_1);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DEFAULT_BROWSER_2);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DEFAULT_BROWSER_3);
            OnboardingPrefManager.getInstance().setOneTimeNotificationStarted(true);
        }

        if (PackageUtils.isFirstInstall(this)
                && SharedPreferencesManager.getInstance().readInt(
                           HnsPreferenceKeys.HNS_APP_OPEN_COUNT)
                        == 1) {
            Calendar calender = Calendar.getInstance();
            calender.setTime(new Date());
            calender.add(Calendar.DATE, DAYS_4);
            HnsRewardsHelper.setNextRewardsOnboardingModalDate(calender.getTimeInMillis());
        }

        if (!mIsSetDefaultBrowserNotification) {
            HnsSetDefaultBrowserUtils.checkSetDefaultBrowserModal(this);
        }

        checkFingerPrintingOnUpgrade();
        checkForVpnCallout();

        if (ChromeFeatureList.isEnabled(HnsFeatureList.HNS_VPN_LINK_SUBSCRIPTION_ANDROID_UI)
                && HnsVpnPrefUtils.isSubscriptionPurchase()
                && !HnsVpnPrefUtils.isLinkSubscriptionDialogShown()) {
            showLinkVpnSubscriptionDialog();
        }
        if (PackageUtils.isFirstInstall(this)
                && (OnboardingPrefManager.getInstance().isDormantUsersEngagementEnabled()
                        || getPackageName().equals(HnsConstants.HNS_PRODUCTION_PACKAGE_NAME))) {
            OnboardingPrefManager.getInstance().setDormantUsersPrefs();
            if (!OnboardingPrefManager.getInstance().isDormantUsersNotificationsStarted()) {
                RetentionNotificationUtil.scheduleDormantUsersNotifications(this);
                OnboardingPrefManager.getInstance().setDormantUsersNotificationsStarted(true);
            }
        }
        initWalletNativeServices();

        mNativeInitialized = true;

        String countryCode = Locale.getDefault().getCountry();
        if (countryCode.equals(HnsConstants.INDIA_COUNTRY_CODE)
                && SharedPreferencesManager.getInstance().readBoolean(
                        HnsPreferenceKeys.HNS_AD_FREE_CALLOUT_DIALOG, true)
                && getActivityTab() != null && getActivityTab().getUrl().getSpec() != null
                && UrlUtilities.isNTPUrl(getActivityTab().getUrl().getSpec())
                && (SharedPreferencesManager.getInstance().readBoolean(
                            HnsPreferenceKeys.HNS_OPENED_YOUTUBE, false)
                        || SharedPreferencesManager.getInstance().readInt(
                                   HnsPreferenceKeys.HNS_APP_OPEN_COUNT)
                                >= 7)) {
            showAdFreeCalloutDialog();
        }

        initHnsNewsController();
        if (SharedPreferencesManager.getInstance().readBoolean(
                    HnsPreferenceKeys.HNS_DEFERRED_DEEPLINK_PLAYLIST, false)) {
            SharedPreferencesManager.getInstance().writeBoolean(
                    HnsPreferenceKeys.HNS_DEFERRED_DEEPLINK_PLAYLIST, false);
            openPlaylist(false);
        } else if (SharedPreferencesManager.getInstance().readBoolean(
                           HnsPreferenceKeys.HNS_DEFERRED_DEEPLINK_VPN, false)) {
            SharedPreferencesManager.getInstance().writeBoolean(
                    HnsPreferenceKeys.HNS_DEFERRED_DEEPLINK_VPN, false);
            handleDeepLinkVpn();
        } else if (!mIsDeepLink
                && OnboardingPrefManager.getInstance().isOnboardingSearchBoxTooltip()
                && getActivityTab() != null && getActivityTab().getUrl().getSpec() != null
                && UrlUtilities.isNTPUrl(getActivityTab().getUrl().getSpec())) {
            showSearchBoxTooltip();
        }

        // Added to reset app links settings for upgrade case
        if (!PackageUtils.isFirstInstall(this)
                && !SharedPreferencesManager.getInstance().readBoolean(
                        HnsPrivacySettings.PREF_APP_LINKS, true)
                && SharedPreferencesManager.getInstance().readBoolean(
                        HnsPrivacySettings.PREF_APP_LINKS_RESET, true)) {
            SharedPreferencesManager.getInstance().writeBoolean(
                    HnsPrivacySettings.PREF_APP_LINKS, true);
            SharedPreferencesManager.getInstance().writeBoolean(
                    HnsPrivacySettings.PREF_APP_LINKS_RESET, false);
        }
    }

    private void handleDeepLinkVpn() {
        mIsDeepLink = true;
        HnsVpnUtils.openHnsVpnPlansActivity(this);
    }

    private void checkForVpnCallout() {
        String countryCode = Locale.getDefault().getCountry();

        if (!countryCode.equals(HnsConstants.INDIA_COUNTRY_CODE)
                && HnsVpnUtils.isVpnFeatureSupported(HnsActivity.this)) {
            if (HnsVpnPrefUtils.shouldShowCallout() && !HnsVpnPrefUtils.isSubscriptionPurchase()
                            && (SharedPreferencesManager.getInstance().readInt(
                                        HnsPreferenceKeys.HNS_APP_OPEN_COUNT)
                                            == 1
                                    && !PackageUtils.isFirstInstall(this))
                    || (SharedPreferencesManager.getInstance().readInt(
                                HnsPreferenceKeys.HNS_APP_OPEN_COUNT)
                                    == 7
                            && PackageUtils.isFirstInstall(this))) {
                showVpnCalloutDialog();
            }

            if (!TextUtils.isEmpty(HnsVpnPrefUtils.getPurchaseToken())
                    && !TextUtils.isEmpty(HnsVpnPrefUtils.getProductId())) {
                mIsVerification = true;
                HnsVpnNativeWorker.getInstance().verifyPurchaseToken(
                        HnsVpnPrefUtils.getPurchaseToken(), HnsVpnPrefUtils.getProductId(),
                        HnsVpnUtils.SUBSCRIPTION_PARAM_TEXT, getPackageName());
            }
        }
    }

    @Override
    public void initHnsNewsController() {
        if (mHnsNewsController != null) {
            return;
        }
        if (mHnsNewsConnectionErrorHandler == null) {
            mHnsNewsConnectionErrorHandler = HnsNewsConnectionErrorHandler.getInstance();
            mHnsNewsConnectionErrorHandler.setDelegate(this);
        }

        if (HnsPrefServiceBridge.getInstance().getShowNews()
                && HnsPrefServiceBridge.getInstance().getNewsOptIn()) {
            mHnsNewsController = HnsNewsControllerFactory.getInstance().getHnsNewsController(
                    mHnsNewsConnectionErrorHandler);

            HnsNewsUtils.getHnsNewsSettingsData(mHnsNewsController, null);
        }
    }

    private void migrateBgPlaybackToFeature() {
        if (SharedPreferencesManager.getInstance().readBoolean(
                    HnsPreferenceKeys.HNS_BACKGROUND_VIDEO_PLAYBACK_CONVERTED_TO_FEATURE,
                    false)) {
            if (HnsPrefServiceBridge.getInstance().getBackgroundVideoPlaybackEnabled()
                    && ChromeFeatureList.isEnabled(
                            HnsFeatureList.HNS_BACKGROUND_VIDEO_PLAYBACK)) {
                HnsPrefServiceBridge.getInstance().setBackgroundVideoPlaybackEnabled(false);
            }
            return;
        }
        if (HnsPrefServiceBridge.getInstance().getBackgroundVideoPlaybackEnabled()) {
            HnsFeatureUtil.enableFeature(
                    HnsFeatureList.HNS_BACKGROUND_VIDEO_PLAYBACK_INTERNAL, true, true);
        }
        SharedPreferencesManager.getInstance().writeBoolean(
                HnsPreferenceKeys.HNS_BACKGROUND_VIDEO_PLAYBACK_CONVERTED_TO_FEATURE, true);
    }

    private void showSearchBoxTooltip() {
        OnboardingPrefManager.getInstance().setOnboardingSearchBoxTooltip(false);
        HighlightView highlightView = new HighlightView(this, null);
        highlightView.setColor(
                ContextCompat.getColor(this, R.color.onboarding_search_highlight_color));
        ViewGroup viewGroup = findViewById(android.R.id.content);
        View anchorView = (View) findViewById(R.id.toolbar);
        float padding = (float) dpToPx(this, 20);
        boolean isTablet = ConfigurationUtils.isTablet(this);
        new Handler().postDelayed(() -> {
            PopupWindowTooltip popupWindowTooltip =
                    new PopupWindowTooltip.Builder(this)
                            .anchorView(anchorView)
                            .arrowColor(getResources().getColor(R.color.onboarding_arrow_color))
                            .gravity(Gravity.BOTTOM)
                            .dismissOnOutsideTouch(true)
                            .dismissOnInsideTouch(false)
                            .backgroundDimDisabled(true)
                            .contentArrowAtStart(!isTablet)
                            .padding(padding)
                            .parentPaddingHorizontal(dpToPx(this, 10))
                            .onDismissListener(tooltip -> {
                                if (viewGroup != null && highlightView != null) {
                                    viewGroup.removeView(highlightView);
                                }
                            })
                            .modal(true)
                            .contentView(R.layout.hns_onboarding_searchbox)
                            .build();

            String countryCode = Locale.getDefault().getCountry();
            if (countryCode.equals(HnsConstants.INDIA_COUNTRY_CODE)) {
                TextView toolTipBody = popupWindowTooltip.findViewById(R.id.tv_tooltip_title);
                toolTipBody.setText(getResources().getString(R.string.searchbox_onboarding_india));
            }
            viewGroup.addView(highlightView);
            HighlightItem item = new HighlightItem(anchorView);
            highlightView.setHighlightTransparent(true);
            highlightView.setHighlightItem(item);
            popupWindowTooltip.show();
        }, 500);
    }

    public void setDormantUsersPrefs() {
        OnboardingPrefManager.getInstance().setDormantUsersPrefs();
        RetentionNotificationUtil.scheduleDormantUsersNotifications(this);
    }

    private void openPlaylist(boolean shouldHandlePlaylistActivity) {
        if (!shouldHandlePlaylistActivity) mIsDeepLink = true;

        if (SharedPreferencesManager.getInstance().readBoolean(
                    PlaylistPreferenceUtils.SHOULD_SHOW_PLAYLIST_ONBOARDING, true)) {
            PlaylistUtils.openPlaylistMenuOnboardingActivity(HnsActivity.this);
            SharedPreferencesManager.getInstance().writeBoolean(
                    PlaylistPreferenceUtils.SHOULD_SHOW_PLAYLIST_ONBOARDING, false);
        } else if (shouldHandlePlaylistActivity) {
            openPlaylistActivity(HnsActivity.this, ConstantUtils.ALL_PLAYLIST);
        }
    }

    public void openPlaylistActivity(Context context, String playlistId) {
        Intent playlistActivityIntent = new Intent(context, PlaylistHostActivity.class);
        playlistActivityIntent.putExtra(ConstantUtils.PLAYLIST_ID, playlistId);
        playlistActivityIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        playlistActivityIntent.setAction(Intent.ACTION_VIEW);
        context.startActivity(playlistActivityIntent);
    }

    public void showPlaylistWarningDialog(
            PlaylistWarningDialogListener playlistWarningDialogListener) {
        PlaylistWarningDialogFragment playlistWarningDialogFragment =
                new PlaylistWarningDialogFragment();
        playlistWarningDialogFragment.setCancelable(false);
        playlistWarningDialogFragment.setPlaylistWarningDialogListener(
                playlistWarningDialogListener);
        playlistWarningDialogFragment.show(
                getSupportFragmentManager(), "PlaylistWarningDialogFragment");
    }

    private void showVpnCalloutDialog() {
        try {
            HnsVpnCalloutDialogFragment hnsVpnCalloutDialogFragment =
                    new HnsVpnCalloutDialogFragment();
            hnsVpnCalloutDialogFragment.show(
                    getSupportFragmentManager(), "HnsVpnCalloutDialogFragment");
        } catch (IllegalStateException e) {
            Log.e("showVpnCalloutDialog", e.getMessage());
        }
    }

    private void showLinkVpnSubscriptionDialog() {
        LinkVpnSubscriptionDialogFragment linkVpnSubscriptionDialogFragment =
                new LinkVpnSubscriptionDialogFragment();
        linkVpnSubscriptionDialogFragment.setCancelable(false);
        linkVpnSubscriptionDialogFragment.show(
                getSupportFragmentManager(), "LinkVpnSubscriptionDialogFragment");
    }

    private void showAdFreeCalloutDialog() {
        SharedPreferencesManager.getInstance().writeBoolean(
                HnsPreferenceKeys.HNS_AD_FREE_CALLOUT_DIALOG, false);

        HnsAdFreeCalloutDialogFragment hnsAdFreeCalloutDialogFragment =
                new HnsAdFreeCalloutDialogFragment();
        hnsAdFreeCalloutDialogFragment.show(
                getSupportFragmentManager(), "HnsAdFreeCalloutDialogFragment");
    }

    public void setNewTabPageManager(NewTabPageManager manager) {
        mNewTabPageManager = manager;
    }

    public void focusSearchBox() {
        if (mNewTabPageManager != null) {
            mNewTabPageManager.focusSearchBox(false, null);
        }
    }

    private void checkFingerPrintingOnUpgrade() {
        if (!PackageUtils.isFirstInstall(this)
                && SharedPreferencesManager.getInstance().readInt(
                           HnsPreferenceKeys.HNS_APP_OPEN_COUNT)
                        == 0) {
            boolean value = SharedPreferencesManager.getInstance().readBoolean(
                    HnsPrivacySettings.PREF_FINGERPRINTING_PROTECTION, true);
            if (value) {
                HnsShieldsContentSettings.setShieldsValue(Profile.getLastUsedRegularProfile(), "",
                        HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                        HnsShieldsContentSettings.DEFAULT, false);
            } else {
                HnsShieldsContentSettings.setShieldsValue(Profile.getLastUsedRegularProfile(), "",
                        HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                        HnsShieldsContentSettings.ALLOW_RESOURCE, false);
            }
        }
    }

    public void openHnsPlaylistSettings() {
        SettingsLauncher settingsLauncher = new SettingsLauncherImpl();
        settingsLauncher.launchSettingsActivity(this, HnsPlaylistPreferences.class);
    }

    public void openHnsNewsSettings() {
        SettingsLauncher settingsLauncher = new SettingsLauncherImpl();
        settingsLauncher.launchSettingsActivity(this, HnsNewsPreferencesV2.class);
    }

    // TODO: Once we have a ready for https://github.com/hnsvn/hns-browser/issues/33015, We'll use
    // this code
    /*public void openHnsContentFilteringSettings() {
        SettingsLauncher settingsLauncher = new SettingsLauncherImpl();
        settingsLauncher.launchSettingsActivity(this, ContentFilteringFragment.class);
    }*/

    public void openHnsWalletSettings() {
        SettingsLauncher settingsLauncher = new SettingsLauncherImpl();
        settingsLauncher.launchSettingsActivity(this, HnsWalletPreferences.class);
    }

    public void openHnsConnectedSitesSettings() {
        SettingsLauncher settingsLauncher = new SettingsLauncherImpl();
        settingsLauncher.launchSettingsActivity(this, HnsWalletEthereumConnectedSites.class);
    }

    public void openHnsWallet(boolean fromDapp, boolean setupAction, boolean restoreAction) {
        Intent hnsWalletIntent = new Intent(this, HnsWalletActivity.class);
        hnsWalletIntent.putExtra(Utils.IS_FROM_DAPPS, fromDapp);
        hnsWalletIntent.putExtra(Utils.RESTART_WALLET_ACTIVITY_SETUP, setupAction);
        hnsWalletIntent.putExtra(Utils.RESTART_WALLET_ACTIVITY_RESTORE, restoreAction);
        hnsWalletIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        hnsWalletIntent.setAction(Intent.ACTION_VIEW);
        startActivity(hnsWalletIntent);
    }

    public void viewOnBlockExplorer(
            String address, @CoinType.EnumType int coinType, NetworkInfo networkInfo) {
        Utils.openAddress("/address/" + address, this, coinType, networkInfo);
    }

    public void openHnsWalletDAppsActivity(HnsWalletDAppsActivity.ActivityType activityType) {
        Intent hnsWalletIntent = new Intent(this, HnsWalletDAppsActivity.class);
        hnsWalletIntent.putExtra("activityType", activityType.getValue());
        hnsWalletIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        hnsWalletIntent.setAction(Intent.ACTION_VIEW);
        startActivity(hnsWalletIntent);
    }

    public PrivacyHubMetrics getPrivacyHubMetrics() {
        return mPrivacyHubMetrics;
    }

    private void checkForYandexSE() {
        String countryCode = Locale.getDefault().getCountry();
        if (sYandexRegions.contains(countryCode)) {
            Profile lastUsedRegularProfile = Profile.getLastUsedRegularProfile();
            TemplateUrl yandexTemplateUrl = HnsSearchEngineUtils.getTemplateUrlByShortName(
                    lastUsedRegularProfile, OnboardingPrefManager.YANDEX);
            if (yandexTemplateUrl != null) {
                HnsSearchEngineUtils.setDSEPrefs(yandexTemplateUrl, lastUsedRegularProfile);
                HnsSearchEngineUtils.setDSEPrefs(yandexTemplateUrl,
                        lastUsedRegularProfile.getPrimaryOTRProfile(/* createIfNeeded= */ true));
            }
        }
    }

    private HnsNotificationWarningDialog.DismissListener mCloseDialogListener =
            new HnsNotificationWarningDialog.DismissListener() {
                @Override
                public void onDismiss() {
                    checkForNotificationData();
                }
            };

    private void checkForNotificationData() {
        Intent notifIntent = getIntent();
        if (notifIntent != null && notifIntent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE) != null) {
            String notificationType = notifIntent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE);
            switch (notificationType) {
                case RetentionNotificationUtil.HOUR_3:
                case RetentionNotificationUtil.HOUR_24:
                case RetentionNotificationUtil.EVERY_SUNDAY:
                    checkForHnsStats();
                    break;
                case RetentionNotificationUtil.DAY_6:
                    if (getActivityTab() != null && getActivityTab().getUrl().getSpec() != null
                            && !UrlUtilities.isNTPUrl(getActivityTab().getUrl().getSpec())) {
                        getTabCreator(false).launchUrl(
                                UrlConstants.NTP_URL, TabLaunchType.FROM_CHROME_UI);
                    }
                    break;
                case RetentionNotificationUtil.DAY_10:
                case RetentionNotificationUtil.DAY_30:
                case RetentionNotificationUtil.DAY_35:
                    openRewardsPanel();
                    break;
                case RetentionNotificationUtil.DORMANT_USERS_DAY_14:
                case RetentionNotificationUtil.DORMANT_USERS_DAY_25:
                case RetentionNotificationUtil.DORMANT_USERS_DAY_40:
                    showDormantUsersEngagementDialog(notificationType);
                    break;
                case RetentionNotificationUtil.DEFAULT_BROWSER_1:
                case RetentionNotificationUtil.DEFAULT_BROWSER_2:
                case RetentionNotificationUtil.DEFAULT_BROWSER_3:
                    if (!HnsSetDefaultBrowserUtils.isHnsSetAsDefaultBrowser(HnsActivity.this)
                            && !HnsSetDefaultBrowserUtils.isHnsDefaultDontAsk()) {
                        mIsSetDefaultBrowserNotification = true;
                        HnsSetDefaultBrowserUtils.showHnsSetDefaultBrowserDialog(
                                HnsActivity.this, false);
                    }
                    break;
            }
        }
    }

    public void checkForHnsStats() {
        if (OnboardingPrefManager.getInstance().isHnsStatsEnabled()) {
            HnsStatsUtil.showHnsStats();
        } else {
            if (getActivityTab() != null && getActivityTab().getUrl().getSpec() != null
                    && !UrlUtilities.isNTPUrl(getActivityTab().getUrl().getSpec())) {
                OnboardingPrefManager.getInstance().setFromNotification(true);
                if (getTabCreator(false) != null) {
                    getTabCreator(false).launchUrl(
                            UrlConstants.NTP_URL, TabLaunchType.FROM_CHROME_UI);
                }
            } else {
                showOnboardingV2(false);
            }
        }
    }

    public void showOnboardingV2(boolean fromStats) {
        try {
            OnboardingPrefManager.getInstance().setNewOnboardingShown(true);
            FragmentManager fm = getSupportFragmentManager();
            HighlightDialogFragment fragment = (HighlightDialogFragment) fm.findFragmentByTag(
                    HighlightDialogFragment.TAG_FRAGMENT);
            FragmentTransaction transaction = fm.beginTransaction();

            if (fragment != null) {
                transaction.remove(fragment);
            }

            fragment = new HighlightDialogFragment();
            Bundle fragmentBundle = new Bundle();
            fragmentBundle.putBoolean(OnboardingPrefManager.FROM_STATS, fromStats);
            fragment.setArguments(fragmentBundle);
            transaction.add(fragment, HighlightDialogFragment.TAG_FRAGMENT);
            transaction.commitAllowingStateLoss();
        } catch (IllegalStateException e) {
            Log.e("HighlightDialogFragment", e.getMessage());
        }
    }

    public void hideRewardsOnboardingIcon() {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.hideRewardsOnboardingIcon();
        }
    }

    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(
                    CHANNEL_ID, getString(R.string.hns_browser), importance);
            channel.setDescription(
                    getString(R.string.hns_browser_notification_channel_description));
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }

    private boolean isNoRestoreState() {
        return SharedPreferencesManager.getInstance().readBoolean(PREF_CLOSE_TABS_ON_EXIT, false);
    }

    private boolean isClearBrowsingDataOnExit() {
        return SharedPreferencesManager.getInstance().readBoolean(PREF_CLEAR_ON_EXIT, false);
    }

    public void onRewardsPanelDismiss() {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.onRewardsPanelDismiss();
        }
    }

    public void dismissRewardsPanel() {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.dismissRewardsPanel();
        }
    }

    public void dismissShieldsTooltip() {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.dismissShieldsTooltip();
        }
    }

    public void openRewardsPanel() {
        HnsToolbarLayoutImpl layout = getHnsToolbarLayout();
        if (layout != null) {
            layout.openRewardsPanel();
        }
    }

    public Profile getCurrentProfile() {
        Tab tab = getActivityTab();
        if (tab == null) {
            return Profile.getLastUsedRegularProfile();
        }

        return Profile.fromWebContents(tab.getWebContents());
    }

    public Tab selectExistingTab(String url) {
        Tab tab = getActivityTab();
        if (tab != null && tab.getUrl().getSpec().equals(url)) {
            return tab;
        }

        TabModel tabModel = getCurrentTabModel();
        int tabIndex = TabModelUtils.getTabIndexByUrl(tabModel, url);

        // Find if tab exists
        if (tabIndex != TabModel.INVALID_TAB_INDEX) {
            tab = tabModel.getTabAt(tabIndex);
            // Set active tab
            tabModel.setIndex(tabIndex, TabSelectionType.FROM_USER, false);
            return tab;
        } else {
            return null;
        }
    }

    public Tab openNewOrSelectExistingTab(String url, boolean refresh) {
        Tab tab = selectExistingTab(url);
        if (tab != null) {
            if (refresh) {
                tab.reload();
            }
            return tab;
        } else { // Open a new tab
            return getTabCreator(false).launchUrl(url, TabLaunchType.FROM_CHROME_UI);
        }
    }

    public Tab openNewOrSelectExistingTab(String url) {
        return openNewOrSelectExistingTab(url, false);
    }

    private void clearWalletModelServices() {
        if (mWalletModel == null) {
            return;
        }

        mWalletModel.resetServices(
                getApplicationContext(), null, null, null, null, null, null, null, null, null);
    }

    private void setupWalletModel() {
        PostTask.postTask(TaskTraits.UI_DEFAULT, () -> {
            if (mWalletModel == null) {
                mWalletModel = new WalletModel(getApplicationContext(), mKeyringService,
                        mBlockchainRegistry, mJsonRpcService, mTxService, mEthTxManagerProxy,
                        mSolanaTxManagerProxy, mAssetRatioService, mHnsWalletService,
                        mSwapService);
            } else {
                mWalletModel.resetServices(getApplicationContext(), mKeyringService,
                        mBlockchainRegistry, mJsonRpcService, mTxService, mEthTxManagerProxy,
                        mSolanaTxManagerProxy, mAssetRatioService, mHnsWalletService,
                        mSwapService);
            }
            setupObservers();
        });
    }

    @MainThread
    private void setupObservers() {
        ThreadUtils.assertOnUiThread();
        clearObservers();
        mWalletModel.getCryptoModel().getPendingTxHelper().mSelectedPendingRequest.observe(
                this, transactionInfo -> {
                    if (transactionInfo == null) {
                        return;
                    }
                    // don't show dapps panel if the wallet is locked and requests are being
                    // processed by the approve dialog already
                    mKeyringService.isLocked(locked -> {
                        if (locked) {
                            return;
                        }

                        if (!mIsProcessingPendingDappsTxRequest) {
                            mIsProcessingPendingDappsTxRequest = true;
                            openHnsWalletDAppsActivity(
                                    HnsWalletDAppsActivity.ActivityType.CONFIRM_TRANSACTION);
                        }

                        // update badge if there's a pending tx
                        updateWalletBadgeVisibility();
                    });
                });

        mWalletModel.getDappsModel().mWalletIconNotificationVisible.observe(
                this, this::setWalletBadgeVisibility);

        mWalletModel.getDappsModel().mPendingWalletAccountCreationRequest.observe(this, request -> {
            if (request == null) return;
            mWalletModel.getKeyringModel().isWalletLocked(isLocked -> {
                if (!HnsWalletPreferences.getPrefWeb3NotificationsEnabled()) return;
                if (isLocked) {
                    Tab tab = getActivityTab();
                    if (tab != null) {
                        walletInteractionDetected(tab.getWebContents());
                    }
                    showWalletPanel(false);
                    return;
                }
                for (CryptoAccountTypeInfo info :
                        mWalletModel.getCryptoModel().getSupportedCryptoAccountTypes()) {
                    if (info.getCoinType() == request.getCoinType()) {
                        Intent intent = AddAccountActivity.createIntentToAddAccount(
                                this, info.getCoinType());
                        startActivity(intent);
                        mWalletModel.getDappsModel().removeProcessedAccountCreationRequest(request);
                        break;
                    }
                }
            });
        });

        mWalletModel.getCryptoModel().getNetworkModel().mNeedToCreateAccountForNetwork.observe(
                this, networkInfo -> {
                    if (networkInfo == null) return;

                    MaterialAlertDialogBuilder builder =
                            new MaterialAlertDialogBuilder(
                                    this, R.style.HnsWalletAlertDialogTheme)
                                    .setMessage(getString(
                                            R.string.hns_wallet_create_account_description,
                                            networkInfo.symbolName))
                                    .setPositiveButton(R.string.hns_action_yes,
                                            (dialog, which) -> {
                                                mWalletModel.createAccountAndSetDefaultNetwork(
                                                        networkInfo);
                                            })
                                    .setNegativeButton(
                                            R.string.hns_action_no, (dialog, which) -> {
                                                mWalletModel.getCryptoModel()
                                                        .getNetworkModel()
                                                        .clearCreateAccountState();
                                                dialog.dismiss();
                                            });
                    builder.show();
                });
    }

    @MainThread
    private void clearObservers() {
        ThreadUtils.assertOnUiThread();
        mWalletModel.getCryptoModel().getPendingTxHelper().mSelectedPendingRequest.removeObservers(
                this);
        mWalletModel.getDappsModel().mWalletIconNotificationVisible.removeObservers(this);
        mWalletModel.getCryptoModel()
                .getNetworkModel()
                .mNeedToCreateAccountForNetwork.removeObservers(this);
    }

    private void showHnsRateDialog() {
        HnsRateDialogFragment rateDialogFragment = HnsRateDialogFragment.newInstance(false);
        rateDialogFragment.show(getSupportFragmentManager(), HnsRateDialogFragment.TAG_FRAGMENT);
    }

    private void showCrossPromotionalDialog() {
        CrossPromotionalModalDialogFragment mCrossPromotionalModalDialogFragment =
                new CrossPromotionalModalDialogFragment();
        mCrossPromotionalModalDialogFragment.show(getSupportFragmentManager(), "CrossPromotionalModalDialogFragment");
    }

    public void showDormantUsersEngagementDialog(String notificationType) {
        if (!HnsSetDefaultBrowserUtils.isHnsSetAsDefaultBrowser(HnsActivity.this)
                && !HnsSetDefaultBrowserUtils.isHnsDefaultDontAsk()) {
            DormantUsersEngagementDialogFragment dormantUsersEngagementDialogFragment =
                    new DormantUsersEngagementDialogFragment();
            dormantUsersEngagementDialogFragment.setNotificationType(notificationType);
            dormantUsersEngagementDialogFragment.show(
                    getSupportFragmentManager(), "DormantUsersEngagementDialogFragment");
            setDormantUsersPrefs();
        }
    }

    private static Activity getActivityOfType(Class<?> classOfActivity) {
        for (Activity ref : ApplicationStatus.getRunningActivities()) {
            if (!classOfActivity.isInstance(ref)) continue;

            return ref;
        }

        return null;
    }

    private void enableSpeedreaderMode() {
        final Tab currentTab = getActivityTab();
        if (currentTab != null) {
            HnsSpeedReaderUtils.enableSpeedreaderMode(currentTab);
        }
    }

    public static ChromeTabbedActivity getChromeTabbedActivity() {
        return (ChromeTabbedActivity) getActivityOfType(ChromeTabbedActivity.class);
    }

    public static CustomTabActivity getCustomTabActivity() {
        return (CustomTabActivity) getActivityOfType(CustomTabActivity.class);
    }

    @NonNull
    public static HnsActivity getHnsActivity() throws HnsActivityNotFoundException {
        HnsActivity activity = (HnsActivity) getActivityOfType(HnsActivity.class);
        if (activity != null) {
            return activity;
        }

        throw new HnsActivityNotFoundException("HnsActivity Not Found");
    }

    @Override
    public void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        if (intent != null) {
            String openUrl = intent.getStringExtra(HnsActivity.OPEN_URL);
            if (!TextUtils.isEmpty(openUrl)) {
                try {
                    openNewOrSelectExistingTab(openUrl);
                } catch (NullPointerException e) {
                    Log.e("HnsActivity", "opening new tab " + e.getMessage());
                }
            }
        }
        checkForNotificationData();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK
                && (requestCode == HnsConstants.VERIFY_WALLET_ACTIVITY_REQUEST_CODE
                        || requestCode == HnsConstants.USER_WALLET_ACTIVITY_REQUEST_CODE
                        || requestCode == HnsConstants.SITE_BANNER_REQUEST_CODE)) {
            dismissRewardsPanel();
            if (data != null) {
                String open_url = data.getStringExtra(HnsActivity.OPEN_URL);
                if (!TextUtils.isEmpty(open_url)) {
                    openNewOrSelectExistingTab(open_url);
                }
            }
        } else if (resultCode == RESULT_OK
                && requestCode == HnsConstants.MONTHLY_CONTRIBUTION_REQUEST_CODE) {
            dismissRewardsPanel();

        } else if (resultCode == RESULT_OK
                && requestCode == HnsConstants.DEFAULT_BROWSER_ROLE_REQUEST_CODE) {
            HnsSetDefaultBrowserUtils.setHnsDefaultSuccess();
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onRequestPermissionsResult(
            int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        FragmentManager fm = getSupportFragmentManager();
        HnsStatsBottomSheetDialogFragment fragment =
                (HnsStatsBottomSheetDialogFragment) fm.findFragmentByTag(
                        HnsStatsUtil.STATS_FRAGMENT_TAG);
        if (fragment != null) {
            fragment.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }

        if (requestCode == HnsStatsUtil.SHARE_STATS_WRITE_EXTERNAL_STORAGE_PERM
                && grantResults.length != 0
                && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            HnsStatsUtil.shareStats(R.layout.hns_stats_share_layout);
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    @Override
    public void performPreInflationStartup() {
        HnsDbUtil dbUtil = HnsDbUtil.getInstance();
        if (dbUtil.dbOperationRequested()) {
            AlertDialog dialog =
                    new AlertDialog.Builder(this)
                            .setMessage(dbUtil.performDbExportOnStart()
                                            ? getString(
                                                    R.string.hns_db_processing_export_alert_info)
                                            : getString(
                                                    R.string.hns_db_processing_import_alert_info))
                            .setCancelable(false)
                            .create();
            dialog.setCanceledOnTouchOutside(false);
            if (dbUtil.performDbExportOnStart()) {
                dbUtil.setPerformDbExportOnStart(false);
                dbUtil.ExportRewardsDb(dialog);
            } else if (dbUtil.performDbImportOnStart() && !dbUtil.dbImportFile().isEmpty()) {
                dbUtil.setPerformDbImportOnStart(false);
                dbUtil.ImportRewardsDb(dialog, dbUtil.dbImportFile());
            }
            dbUtil.cleanUpDbOperationRequest();
        }
        super.performPreInflationStartup();
    }

    @Override
    protected @LaunchIntentDispatcher.Action int maybeDispatchLaunchIntent(
            Intent intent, Bundle savedInstanceState) {
        boolean notificationUpdate = IntentUtils.safeGetBooleanExtra(
                intent, HnsPreferenceKeys.HNS_UPDATE_EXTRA_PARAM, false);
        if (notificationUpdate) {
            setUpdatePreferences();
        }

        return super.maybeDispatchLaunchIntent(intent, savedInstanceState);
    }

    private void setUpdatePreferences() {
        Calendar currentTime = Calendar.getInstance();
        long milliSeconds = currentTime.getTimeInMillis();

        SharedPreferences sharedPref = getApplicationContext().getSharedPreferences(
                HnsPreferenceKeys.HNS_NOTIFICATION_PREF_NAME, 0);
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.putLong(HnsPreferenceKeys.HNS_MILLISECONDS_NAME, milliSeconds);
        editor.apply();
    }

    public ObservableSupplier<BrowserControlsManager> getBrowserControlsManagerSupplier() {
        return mBrowserControlsManagerSupplier;
    }

    public int getToolbarShadowHeight() {
        View toolbarShadow = findViewById(R.id.toolbar_hairline);
        assert toolbarShadow != null;
        if (toolbarShadow != null) {
            return toolbarShadow.getHeight();
        }
        return 0;
    }

    public float getToolbarBottom() {
        View toolbarShadow = findViewById(R.id.toolbar_hairline);
        assert toolbarShadow != null;
        if (toolbarShadow != null) {
            return toolbarShadow.getY();
        }
        return 0;
    }

    public boolean isViewBelowToolbar(View view) {
        View toolbarShadow = findViewById(R.id.toolbar_hairline);
        assert toolbarShadow != null;
        assert view != null;
        if (toolbarShadow != null && view != null) {
            int[] coordinatesToolbar = new int[2];
            toolbarShadow.getLocationInWindow(coordinatesToolbar);
            int[] coordinatesView = new int[2];
            view.getLocationInWindow(coordinatesView);
            return coordinatesView[1] >= coordinatesToolbar[1];
        }

        return false;
    }

    @NativeMethods
    interface Natives {
        void restartStatsUpdater();
        String getSafeBrowsingApiKey();
    }

    private void initHnsWalletService() {
        if (mHnsWalletService != null) {
            return;
        }

        mHnsWalletService = HnsWalletServiceFactory.getInstance().getHnsWalletService(this);
    }

    private void initKeyringService() {
        if (mKeyringService != null) {
            return;
        }

        mKeyringService = KeyringServiceFactory.getInstance().getKeyringService(this);
    }

    private void initJsonRpcService() {
        if (mJsonRpcService != null) {
            return;
        }

        mJsonRpcService = JsonRpcServiceFactory.getInstance().getJsonRpcService(this);
    }

    private void initTxService() {
        if (mTxService != null) {
            return;
        }

        mTxService = TxServiceFactory.getInstance().getTxService(this);
    }

    private void initEthTxManagerProxy() {
        if (mEthTxManagerProxy != null) {
            return;
        }

        mEthTxManagerProxy = TxServiceFactory.getInstance().getEthTxManagerProxy(this);
    }

    private void initSolanaTxManagerProxy() {
        if (mSolanaTxManagerProxy != null) {
            return;
        }

        mSolanaTxManagerProxy = TxServiceFactory.getInstance().getSolanaTxManagerProxy(this);
    }

    private void initBlockchainRegistry() {
        if (mBlockchainRegistry != null) {
            return;
        }

        mBlockchainRegistry = BlockchainRegistryFactory.getInstance().getBlockchainRegistry(this);
    }

    private void initAssetRatioService() {
        if (mAssetRatioService != null) {
            return;
        }

        mAssetRatioService = AssetRatioServiceFactory.getInstance().getAssetRatioService(this);
    }

    @Override
    public void initPrivacyHubMetrics() {
        if (mPrivacyHubMetrics != null) {
            return;
        }
        if (mPrivacyHubMetricsConnectionErrorHandler == null) {
            mPrivacyHubMetricsConnectionErrorHandler =
                    PrivacyHubMetricsConnectionErrorHandler.getInstance();
            mPrivacyHubMetricsConnectionErrorHandler.setDelegate(this);
        }

        mPrivacyHubMetrics = PrivacyHubMetricsFactory.getInstance().getMetricsService(
                mPrivacyHubMetricsConnectionErrorHandler);
        mPrivacyHubMetrics.recordEnabledStatus(
                OnboardingPrefManager.getInstance().isHnsStatsEnabled());
    }

    private void initSwapService() {
        if (mSwapService != null) {
            return;
        }
        mSwapService = SwapServiceFactory.getInstance().getSwapService(this);
    }

    private void initWalletNativeServices() {
        initBlockchainRegistry();
        initTxService();
        initEthTxManagerProxy();
        initSolanaTxManagerProxy();
        initAssetRatioService();
        initHnsWalletService();
        initKeyringService();
        initJsonRpcService();
        initPrivacyHubMetrics();
        initSwapService();
        setupWalletModel();
    }

    private void cleanUpWalletNativeServices() {
        clearWalletModelServices();
        if (mKeyringService != null) mKeyringService.close();
        if (mAssetRatioService != null) mAssetRatioService.close();
        if (mBlockchainRegistry != null) mBlockchainRegistry.close();
        if (mJsonRpcService != null) mJsonRpcService.close();
        if (mTxService != null) mTxService.close();
        if (mEthTxManagerProxy != null) mEthTxManagerProxy.close();
        if (mSolanaTxManagerProxy != null) mSolanaTxManagerProxy.close();
        if (mHnsWalletService != null) mHnsWalletService.close();
        mKeyringService = null;
        mBlockchainRegistry = null;
        mJsonRpcService = null;
        mTxService = null;
        mEthTxManagerProxy = null;
        mSolanaTxManagerProxy = null;
        mAssetRatioService = null;
        mHnsWalletService = null;
    }

    @Override
    public void cleanUpPrivacyHubMetrics() {
        if (mPrivacyHubMetrics != null) mPrivacyHubMetrics.close();
        mPrivacyHubMetrics = null;
    }

    @NonNull
    private HnsToolbarLayoutImpl getHnsToolbarLayout() {
        HnsToolbarLayoutImpl layout = findViewById(R.id.toolbar);
        assert layout != null;
        return layout;
    }

    public void addOrEditBookmark(final Tab tabToBookmark) {
        RateUtils.getInstance().setPrefAddedBookmarkCount();
        ((TabBookmarker) mTabBookmarkerSupplier.get()).addOrEditBookmark(tabToBookmark);
    }

    // We call that method with an interval
    // HnsSafeBrowsingApiHandler.SAFE_BROWSING_INIT_INTERVAL_MS,
    // as upstream does, to keep the GmsCore process alive.
    private void executeInitSafeBrowsing(long delay) {
        // SafeBrowsingBridge.getSafeBrowsingState() has to be executed on a main thread
        PostTask.postDelayedTask(TaskTraits.UI_DEFAULT, () -> {
            if (SafeBrowsingBridge.getSafeBrowsingState() != SafeBrowsingState.NO_SAFE_BROWSING) {
                // initSafeBrowsing could be executed on a background thread
                PostTask.postTask(TaskTraits.USER_VISIBLE_MAY_BLOCK,
                        () -> { HnsSafeBrowsingApiHandler.getInstance().initSafeBrowsing(); });
            }
            executeInitSafeBrowsing(HnsSafeBrowsingApiHandler.SAFE_BROWSING_INIT_INTERVAL_MS);
        }, delay);
    }

    public void updateBottomSheetPosition(int orientation) {
        if (BottomToolbarConfiguration.isBottomToolbarEnabled()) {
            // Ensure the bottom sheet's container is adjusted to the height of the bottom toolbar.
            ViewGroup sheetContainer = findViewById(R.id.sheet_container);
            assert sheetContainer != null;

            if (sheetContainer != null) {
                CoordinatorLayout.LayoutParams params =
                        (CoordinatorLayout.LayoutParams) sheetContainer.getLayoutParams();
                params.bottomMargin = orientation == Configuration.ORIENTATION_LANDSCAPE
                        ? 0
                        : getResources().getDimensionPixelSize(R.dimen.bottom_controls_height);
                sheetContainer.setLayoutParams(params);
            }
        }
    }
}
