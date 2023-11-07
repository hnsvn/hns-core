/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.top;

import static org.chromium.ui.base.ViewUtils.dpToPx;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.ForegroundColorSpan;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.core.content.res.ResourcesCompat;
import androidx.core.widget.ImageViewCompat;
import androidx.fragment.app.FragmentActivity;

import com.hns.playlist.enums.PlaylistOptions;
import com.hns.playlist.listener.PlaylistOnboardingActionClickListener;
import com.hns.playlist.listener.PlaylistOptionsListener;
import com.hns.playlist.model.PlaylistOptionsModel;
import com.hns.playlist.model.SnackBarActionModel;
import com.hns.playlist.util.ConnectionUtils;
import com.hns.playlist.util.ConstantUtils;
import com.hns.playlist.util.PlaylistPreferenceUtils;
import com.hns.playlist.util.PlaylistViewUtils;
import com.hns.playlist.view.PlaylistOnboardingPanel;

import org.chromium.base.ApiCompatibilityUtils;
import org.chromium.base.HnsFeatureList;
import org.chromium.base.HnsPreferenceKeys;
import org.chromium.base.HnsReflectionUtil;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.MathUtils;
import org.chromium.base.ThreadUtils;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsRelaunchUtils;
import org.chromium.chrome.browser.HnsRewardsHelper;
import org.chromium.chrome.browser.HnsRewardsNativeWorker;
import org.chromium.chrome.browser.HnsRewardsObserver;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.hns_stats.HnsStatsUtil;
import org.chromium.chrome.browser.crypto_wallet.controller.DAppsWalletController;
import org.chromium.chrome.browser.custom_layout.popup_window_tooltip.PopupWindowTooltip;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.customtabs.features.toolbar.CustomTabToolbar;
import org.chromium.chrome.browser.dialogs.HnsAdsSignupDialog;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.local_database.HnsStatsTable;
import org.chromium.chrome.browser.local_database.DatabaseHelper;
import org.chromium.chrome.browser.local_database.SavedBandwidthTable;
import org.chromium.chrome.browser.notifications.HnsNotificationWarningDialog;
import org.chromium.chrome.browser.notifications.HnsPermissionUtils;
import org.chromium.chrome.browser.notifications.RewardsYouAreNotEarningDialog;
import org.chromium.chrome.browser.omnibox.LocationBarCoordinator;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.onboarding.SearchActivity;
import org.chromium.chrome.browser.onboarding.v2.HighlightItem;
import org.chromium.chrome.browser.onboarding.v2.HighlightView;
import org.chromium.chrome.browser.playlist.PlaylistServiceFactoryAndroid;
import org.chromium.chrome.browser.playlist.PlaylistServiceObserverImpl;
import org.chromium.chrome.browser.playlist.PlaylistServiceObserverImpl.PlaylistServiceObserverImplDelegate;
import org.chromium.chrome.browser.playlist.PlaylistWarningDialogFragment.PlaylistWarningDialogListener;
import org.chromium.chrome.browser.playlist.settings.HnsPlaylistPreferences;
import org.chromium.chrome.browser.preferences.HnsPrefServiceBridge;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.preferences.website.HnsShieldsContentSettings;
import org.chromium.chrome.browser.preferences.website.HnsShieldsContentSettingsObserver;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.rewards.HnsRewardsPanel;
import org.chromium.chrome.browser.rewards.onboarding.RewardsOnboarding;
import org.chromium.chrome.browser.settings.AppearancePreferences;
import org.chromium.chrome.browser.shields.HnsShieldsHandler;
import org.chromium.chrome.browser.shields.HnsShieldsMenuObserver;
import org.chromium.chrome.browser.shields.HnsShieldsUtils;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabHidingType;
import org.chromium.chrome.browser.tab.TabImpl;
import org.chromium.chrome.browser.tab.TabSelectionType;
import org.chromium.chrome.browser.tabmodel.TabModelSelector;
import org.chromium.chrome.browser.tabmodel.TabModelSelectorTabModelObserver;
import org.chromium.chrome.browser.tabmodel.TabModelSelectorTabObserver;
import org.chromium.chrome.browser.theme.ThemeUtils;
import org.chromium.chrome.browser.toolbar.HomeButton;
import org.chromium.chrome.browser.toolbar.ToolbarDataProvider;
import org.chromium.chrome.browser.toolbar.ToolbarTabController;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarVariationManager;
import org.chromium.chrome.browser.toolbar.menu_button.HnsMenuButtonCoordinator;
import org.chromium.chrome.browser.toolbar.menu_button.MenuButtonCoordinator;
import org.chromium.chrome.browser.toolbar.top.NavigationPopup.HistoryDelegate;
import org.chromium.chrome.browser.toolbar.top.ToolbarTablet.OfflineDownloader;
import org.chromium.chrome.browser.util.HnsConstants;
import org.chromium.chrome.browser.util.ConfigurationUtils;
import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.widget.quickactionsearchandbookmark.promo.SearchWidgetPromoPanel;
import org.chromium.components.embedder_support.util.UrlUtilities;
import org.chromium.content_public.browser.NavigationHandle;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.playlist.mojom.PlaylistEvent;
import org.chromium.playlist.mojom.PlaylistItem;
import org.chromium.playlist.mojom.PlaylistService;
import org.chromium.ui.UiUtils;
import org.chromium.ui.base.DeviceFormFactor;
import org.chromium.ui.base.ViewUtils;
import org.chromium.ui.interpolators.Interpolators;
import org.chromium.ui.util.ColorUtils;
import org.chromium.ui.widget.Toast;
import org.chromium.url.GURL;
import org.chromium.url.mojom.Url;

import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.function.BooleanSupplier;

public abstract class HnsToolbarLayoutImpl extends ToolbarLayout
        implements HnsToolbarLayout, OnClickListener, View.OnLongClickListener,
                   HnsRewardsObserver, HnsRewardsNativeWorker.PublisherObserver,
                   ConnectionErrorHandler, PlaylistServiceObserverImplDelegate {
    private static final String TAG = "HnsToolbar";

    private static final String YOUTUBE_DOMAIN = "youtube.com";
    private static final List<String> HNS_SEARCH_ENGINE_DEFAULT_REGIONS =
            Arrays.asList("CA", "DE", "FR", "GB", "US", "AT", "ES", "MX", "BR", "AR", "IN");
    private static final long MB_10 = 10000000;
    private static final long MINUTES_10 = 10 * 60 * 1000;
    private static final int URL_FOCUS_TOOLBAR_BUTTONS_TRANSLATION_X_DP = 10;

    private static final int PLAYLIST_MEDIA_COUNT_LIMIT = 3;

    private PlaylistServiceObserverImpl mPlaylistServiceObserver;

    private DatabaseHelper mDatabaseHelper = DatabaseHelper.getInstance();

    private ImageButton mHnsWalletButton;
    private ImageButton mHnsShieldsButton;
    private ImageButton mHnsRewardsButton;
    private HomeButton mHomeButton;
    private FrameLayout mWalletLayout;
    private FrameLayout mShieldsLayout;
    private FrameLayout mRewardsLayout;
    private HnsShieldsHandler mHnsShieldsHandler;
    private TabModelSelectorTabObserver mTabModelSelectorTabObserver;
    private TabModelSelectorTabModelObserver mTabModelSelectorTabModelObserver;
    private HnsRewardsNativeWorker mHnsRewardsNativeWorker;
    private HnsRewardsPanel mRewardsPopup;
    private DAppsWalletController mDAppsWalletController;
    private HnsShieldsContentSettings mHnsShieldsContentSettings;
    private HnsShieldsContentSettingsObserver mHnsShieldsContentSettingsObserver;
    private TextView mHnsRewardsNotificationsCount;
    private ImageView mHnsRewardsOnboardingIcon;
    private View mHnsWalletBadge;
    private ImageView mWalletIcon;
    private int mCurrentToolbarColor;

    private boolean mIsPublisherVerified;
    private boolean mIsNotificationPosted;
    private boolean mIsInitialNotificationPosted; // initial red circle notification

    private PopupWindowTooltip mShieldsPopupWindowTooltip;

    private boolean mIsBottomToolbarVisible;

    private ColorStateList mDarkModeTint;
    private ColorStateList mLightModeTint;

    private SearchWidgetPromoPanel mSearchWidgetPromoPanel;

    private final Set<Integer> mTabsWithWalletIcon =
            Collections.synchronizedSet(new HashSet<Integer>());

    private PlaylistService mPlaylistService;

    private enum BigtechCompany { Google, Facebook, Amazon }

    public HnsToolbarLayoutImpl(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void destroy() {
        if (mHnsShieldsContentSettings != null) {
            mHnsShieldsContentSettings.removeObserver(mHnsShieldsContentSettingsObserver);
        }
        if (mPlaylistService != null) {
            mPlaylistService.close();
        }
        if (mPlaylistServiceObserver != null) {
            mPlaylistServiceObserver.close();
            mPlaylistServiceObserver.destroy();
            mPlaylistServiceObserver = null;
        }
        super.destroy();
        if (mHnsRewardsNativeWorker != null) {
            mHnsRewardsNativeWorker.RemoveObserver(this);
            mHnsRewardsNativeWorker.RemovePublisherObserver(this);
        }
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        if (HnsReflectionUtil.EqualTypes(this.getClass(), ToolbarTablet.class)) {
            ImageButton forwardButton = findViewById(R.id.forward_button);
            if (forwardButton != null) {
                final Drawable forwardButtonDrawable = UiUtils.getTintedDrawable(getContext(),
                        R.drawable.btn_right_tablet, R.color.default_icon_color_tint_list);
                forwardButton.setImageDrawable(forwardButtonDrawable);
            }
        }

        mWalletLayout = (FrameLayout) findViewById(R.id.hns_wallet_button_layout);
        mShieldsLayout = (FrameLayout) findViewById(R.id.hns_shields_button_layout);
        mRewardsLayout = (FrameLayout) findViewById(R.id.hns_rewards_button_layout);
        mHnsRewardsNotificationsCount = (TextView) findViewById(R.id.br_notifications_count);
        mHnsRewardsOnboardingIcon = findViewById(R.id.br_rewards_onboarding_icon);
        mHnsWalletButton = (ImageButton) findViewById(R.id.hns_wallet_button);
        mHnsShieldsButton = (ImageButton) findViewById(R.id.hns_shields_button);
        mHnsRewardsButton = (ImageButton) findViewById(R.id.hns_rewards_button);
        mHomeButton = (HomeButton) findViewById(R.id.home_button);
        mHnsWalletBadge = findViewById(R.id.wallet_notfication_badge);
        if (mWalletLayout != null) {
            mWalletIcon = mWalletLayout.findViewById(R.id.hns_wallet_button);
        }

        mDarkModeTint = ThemeUtils.getThemedToolbarIconTint(getContext(), false);
        mLightModeTint =
                ColorStateList.valueOf(ContextCompat.getColor(getContext(), R.color.hns_white));
        mSearchWidgetPromoPanel = new SearchWidgetPromoPanel(getContext());
        if (mHomeButton != null) {
            mHomeButton.setOnLongClickListener(this);
        }

        if (mHnsShieldsButton != null) {
            mHnsShieldsButton.setClickable(true);
            mHnsShieldsButton.setOnClickListener(this);
            mHnsShieldsButton.setOnLongClickListener(this);
        }

        if (mHnsRewardsButton != null) {
            mHnsRewardsButton.setClickable(true);
            mHnsRewardsButton.setOnClickListener(this);
            mHnsRewardsButton.setOnLongClickListener(this);
        }

        if (mHnsWalletButton != null) {
            mHnsWalletButton.setClickable(true);
            mHnsWalletButton.setOnClickListener(this);
            mHnsWalletButton.setOnLongClickListener(this);
        }

        mHnsShieldsHandler = new HnsShieldsHandler(getContext());
        if (!mHnsShieldsHandler.isDisconnectEntityLoaded
                && !HnsShieldsUtils.hasShieldsTooltipShown(
                        HnsShieldsUtils.PREF_SHIELDS_TOOLTIP)) {
            mHnsShieldsHandler.loadDisconnectEntityList(getContext());
        }
        mHnsShieldsHandler.addObserver(new HnsShieldsMenuObserver() {
            @Override
            public void onMenuTopShieldsChanged(boolean isOn, boolean isTopShield) {
                Tab currentTab = getToolbarDataProvider().getTab();
                if (currentTab == null) {
                    return;
                }
                if (isTopShield) {
                    updateHnsShieldsButtonState(currentTab);
                }
                if (currentTab.isLoading()) {
                    currentTab.stopLoading();
                }
                currentTab.reloadIgnoringCache();
                if (null != mHnsShieldsHandler) {
                    // Clean the Hnsry Panel
                    mHnsShieldsHandler.updateValues(0, 0, 0, 0);
                }
            }
        });
        mHnsShieldsContentSettingsObserver = new HnsShieldsContentSettingsObserver() {
            @Override
            public void blockEvent(int tabId, String blockType, String subresource) {
                mHnsShieldsHandler.addStat(tabId, blockType, subresource);
                Tab currentTab = getToolbarDataProvider().getTab();
                if (currentTab == null || currentTab.getId() != tabId) {
                    return;
                }
                mHnsShieldsHandler.updateValues(tabId);
                if (!isIncognito() && OnboardingPrefManager.getInstance().isHnsStatsEnabled()
                        && (blockType.equals(HnsShieldsContentSettings.RESOURCE_IDENTIFIER_ADS)
                                || blockType.equals(HnsShieldsContentSettings
                                                            .RESOURCE_IDENTIFIER_TRACKERS))) {
                    addStatsToDb(blockType, subresource, currentTab.getUrl().getSpec());
                }
            }

            @Override
            public void savedBandwidth(long savings) {
                if (!isIncognito() && OnboardingPrefManager.getInstance().isHnsStatsEnabled()) {
                    addSavedBandwidthToDb(savings);
                }
            }
        };
        // Initially show shields off image. Shields button state will be updated when tab is
        // shown and loading state is changed.
        updateHnsShieldsButtonState(null);
        if (HnsReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)) {
            if (getMenuButtonCoordinator() != null && isMenuButtonOnBottom()) {
                getMenuButtonCoordinator().setVisibility(false);
            }
        }

        if (HnsReflectionUtil.EqualTypes(this.getClass(), CustomTabToolbar.class)) {
            LinearLayout customActionButtons = findViewById(R.id.action_buttons);
            assert customActionButtons != null : "Something has changed in the upstream!";
            if (customActionButtons != null && mHnsShieldsButton != null) {
                ViewGroup.MarginLayoutParams hnsShieldsButtonLayout =
                        (ViewGroup.MarginLayoutParams) mHnsShieldsButton.getLayoutParams();
                ViewGroup.MarginLayoutParams actionButtonsLayout =
                        (ViewGroup.MarginLayoutParams) customActionButtons.getLayoutParams();
                actionButtonsLayout.setMarginEnd(actionButtonsLayout.getMarginEnd()
                        + hnsShieldsButtonLayout.getMarginEnd());
                customActionButtons.setLayoutParams(actionButtonsLayout);
            }
        }
        updateShieldsLayoutBackground(isIncognito()
                || !ContextUtils.getAppSharedPreferences().getBoolean(
                        AppearancePreferences.PREF_SHOW_HNS_REWARDS_ICON, true));
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (isPlaylistEnabledByPrefsAndFlags()) {
            mPlaylistService = null;
            initPlaylistService();
        }
    }

    private void initPlaylistService() {
        if (mPlaylistService != null) {
            return;
        }

        mPlaylistService = PlaylistServiceFactoryAndroid.getInstance().getPlaylistService(this);
    }

    @Override
    protected void onNativeLibraryReady() {
        super.onNativeLibraryReady();
        if (isPlaylistEnabledByPrefsAndFlags()) {
            initPlaylistService();
            mPlaylistServiceObserver = new PlaylistServiceObserverImpl(this);
            mPlaylistService.addObserver(mPlaylistServiceObserver);
        }
        mHnsShieldsContentSettings = HnsShieldsContentSettings.getInstance();
        mHnsShieldsContentSettings.addObserver(mHnsShieldsContentSettingsObserver);

        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        mHnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
        if (mHnsRewardsNativeWorker != null && mHnsRewardsNativeWorker.IsSupported()
                && !HnsPrefServiceBridge.getInstance().getSafetynetCheckFailed()
                && sharedPreferences.getBoolean(
                        AppearancePreferences.PREF_SHOW_HNS_REWARDS_ICON, true)
                && mRewardsLayout != null) {
            mRewardsLayout.setVisibility(View.VISIBLE);
        }
        if (mShieldsLayout != null) {
            updateShieldsLayoutBackground(
                    !(mRewardsLayout != null && mRewardsLayout.getVisibility() == View.VISIBLE));
            mShieldsLayout.setVisibility(View.VISIBLE);
        }
        if (mHnsRewardsNativeWorker != null) {
            mHnsRewardsNativeWorker.AddObserver(this);
            mHnsRewardsNativeWorker.AddPublisherObserver(this);
            mHnsRewardsNativeWorker.TriggerOnNotifyFrontTabUrlChanged();
            mHnsRewardsNativeWorker.GetAllNotifications();
        }
    }

    @Override
    public void setTabModelSelector(TabModelSelector selector) {
        // We might miss events before calling setTabModelSelector, so we need
        // to proactively update the shields button state here, otherwise shields
        // might sometimes show as disabled while it is actually enabled.
        updateHnsShieldsButtonState(getToolbarDataProvider().getTab());
        mTabModelSelectorTabObserver = new TabModelSelectorTabObserver(selector) {
            @Override
            protected void onTabRegistered(Tab tab) {
                super.onTabRegistered(tab);
                if (tab.isIncognito()) {
                    showWalletIcon(false);
                }
            }

            @Override
            public void onShown(Tab tab, @TabSelectionType int type) {
                // Update shields button state when visible tab is changed.
                updateHnsShieldsButtonState(tab);
                // case when window.open is triggered from dapps site and new tab is in focus
                if (type != TabSelectionType.FROM_USER) {
                    dismissWalletPanelOrDialog();
                }
                findMediaFiles(tab);
            }

            @Override
            public void onHidden(Tab tab, @TabHidingType int reason) {
                hidePlaylistButton();
            }

            @Override
            public void onPageLoadStarted(Tab tab, GURL url) {
                showWalletIcon(false, tab);
                if (getToolbarDataProvider().getTab() == tab) {
                    updateHnsShieldsButtonState(tab);
                }
                mHnsShieldsHandler.clearHnsShieldsCount(tab.getId());
                dismissShieldsTooltip();
                hidePlaylistButton();
            }

            @Override
            public void onPageLoadFinished(final Tab tab, GURL url) {
                if (getToolbarDataProvider().getTab() == tab) {
                    mHnsShieldsHandler.updateUrlSpec(url.getSpec());
                    updateHnsShieldsButtonState(tab);

                    if (mHnsShieldsButton != null && mHnsShieldsButton.isShown()
                            && mHnsShieldsHandler != null && !mHnsShieldsHandler.isShowing()) {
                        checkForTooltip(tab);
                    }
                }

                if (mHnsShieldsButton != null && mHnsShieldsButton.isShown()
                        && mHnsShieldsHandler != null && !mHnsShieldsHandler.isShowing()
                        && url.getSpec().contains("rewards")
                        && ((!HnsPermissionUtils.hasNotificationPermission(getContext()))
                                || HnsNotificationWarningDialog.shouldShowRewardWarningDialog(
                                        getContext()))) {
                    showNotificationNotEarningDialog();
                }

                String countryCode = Locale.getDefault().getCountry();
                if (countryCode.equals(HnsConstants.INDIA_COUNTRY_CODE)
                        && url.domainIs(YOUTUBE_DOMAIN)
                        && SharedPreferencesManager.getInstance().readBoolean(
                                HnsPreferenceKeys.HNS_AD_FREE_CALLOUT_DIALOG, true)) {
                    SharedPreferencesManager.getInstance().writeBoolean(
                            HnsPreferenceKeys.HNS_OPENED_YOUTUBE, true);
                }
            }

            private void showNotificationNotEarningDialog() {
                try {
                    RewardsYouAreNotEarningDialog rewardsYouAreNotEarningDialog =
                            RewardsYouAreNotEarningDialog.newInstance();
                    rewardsYouAreNotEarningDialog.setCancelable(false);
                    rewardsYouAreNotEarningDialog.show(
                            HnsActivity.getHnsActivity().getSupportFragmentManager(),
                            RewardsYouAreNotEarningDialog.RewardsYouAreNotEarningDialogTAG);

                } catch (HnsActivity.HnsActivityNotFoundException | IllegalStateException e) {
                    Log.e(TAG, "showNotificationNotEarningDialog " + e);
                }
            }

            @Override
            public void onDidFinishNavigationInPrimaryMainFrame(
                    Tab tab, NavigationHandle navigation) {
                if (getToolbarDataProvider().getTab() == tab && mHnsRewardsNativeWorker != null
                        && !tab.isIncognito()) {
                    mHnsRewardsNativeWorker.OnNotifyFrontTabUrlChanged(
                            tab.getId(), tab.getUrl().getSpec());
                }
                if (PackageUtils.isFirstInstall(getContext()) && tab.getUrl().getSpec() != null
                        && (tab.getUrl().getSpec().equals(HnsActivity.HNS_REWARDS_SETTINGS_URL))
                        && HnsRewardsHelper.shouldShowHnsRewardsOnboardingModal()
                        && mHnsRewardsNativeWorker != null
                        && !mHnsRewardsNativeWorker.isRewardsEnabled()
                        && mHnsRewardsNativeWorker.IsSupported()) {
                    showOnBoarding();
                }
                findMediaFiles(tab);
            }

            @Override
            public void onDestroyed(Tab tab) {
                // Remove references for the ads from the Database. Tab is destroyed, they are not
                // needed anymore.
                new Thread() {
                    @Override
                    public void run() {
                        mDatabaseHelper.deleteDisplayAdsFromTab(tab.getId());
                    }
                }.start();
                mHnsShieldsHandler.removeStat(tab.getId());
                mTabsWithWalletIcon.remove(tab.getId());
            }
        };

        mTabModelSelectorTabModelObserver = new TabModelSelectorTabModelObserver(selector) {
            @Override
            public void didSelectTab(Tab tab, @TabSelectionType int type, int lastId) {
                if (mHnsRewardsNativeWorker != null && !tab.isIncognito()) {
                    mHnsRewardsNativeWorker.OnNotifyFrontTabUrlChanged(
                            tab.getId(), tab.getUrl().getSpec());
                    Tab providerTab = getToolbarDataProvider().getTab();
                    if (providerTab != null && providerTab.getId() == tab.getId()) {
                        showWalletIcon(mTabsWithWalletIcon.contains(tab.getId()));
                    } else if (mWalletLayout != null) {
                        mWalletLayout.setVisibility(mTabsWithWalletIcon.contains(tab.getId())
                                        ? View.VISIBLE
                                        : View.GONE);
                    }
                }
            }
        };
    }

    private void showOnBoarding() {
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            int deviceWidth = ConfigurationUtils.getDisplayMetrics(activity).get("width");
            boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(activity);
            deviceWidth = (int) (isTablet ? (deviceWidth * 0.6) : (deviceWidth * 0.95));
            RewardsOnboarding panel = new RewardsOnboarding(mHnsRewardsButton, deviceWidth);
            panel.showLikePopDownMenu();
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "RewardsOnboarding failed " + e);
        }
    }

    private static boolean isPlaylistEnabledByPrefsAndFlags() {
        return ChromeFeatureList.isEnabled(HnsFeatureList.HNS_PLAYLIST)
                && SharedPreferencesManager.getInstance().readBoolean(
                        HnsPlaylistPreferences.PREF_ENABLE_PLAYLIST, true);
    }

    private void hidePlaylistButton() {
        try {
            ViewGroup viewGroup =
                    HnsActivity.getHnsActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);
            View playlistButton = viewGroup.findViewById(R.id.playlist_button_id);
            if (playlistButton != null && playlistButton.getVisibility() == View.VISIBLE) {
                playlistButton.setVisibility(View.GONE);
            }
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "hidePlaylistButton " + e);
        }
    }

    private boolean isPlaylistButtonVisible() {
        try {
            ViewGroup viewGroup =
                    HnsActivity.getHnsActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);
            View playlistButton = viewGroup.findViewById(R.id.playlist_button_id);
            return playlistButton != null && playlistButton.getVisibility() == View.VISIBLE;
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "isPlaylistButtonVisible " + e);
            return false;
        }
    }

    private void findMediaFiles(Tab tab) {
        if (isPlaylistEnabledByPrefsAndFlags() && mPlaylistService != null) {
            hidePlaylistButton();
            mPlaylistService.findMediaFilesFromActiveTab((url, playlistItems) -> {});
        }
    }

    private void showPlaylistButton(PlaylistItem[] items) {
        try {
            ViewGroup viewGroup =
                    HnsActivity.getHnsActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);

            PlaylistOptionsListener playlistOptionsListener = new PlaylistOptionsListener() {
                @Override
                public void onOptionClicked(PlaylistOptionsModel playlistOptionsModel) {
                    try {
                        if (playlistOptionsModel.getOptionType() == PlaylistOptions.ADD_MEDIA) {
                            int mediaCount = SharedPreferencesManager.getInstance().readInt(
                                    PlaylistPreferenceUtils.ADD_MEDIA_COUNT);
                            if (mediaCount == 2) {
                                PlaylistWarningDialogListener playlistWarningDialogListener =
                                        new PlaylistWarningDialogListener() {
                                            @Override
                                            public void onActionClicked() {
                                                addMediaToPlaylist(items);
                                            }

                                            @Override
                                            public void onSettingsClicked() {
                                                try {
                                                    HnsActivity.getHnsActivity()
                                                            .openHnsPlaylistSettings();
                                                } catch (
                                                        HnsActivity
                                                                .HnsActivityNotFoundException e) {
                                                    Log.e(TAG,
                                                            "showPlaylistButton"
                                                                    + " onOptionClicked"
                                                                    + " onSettingsClicked" + e);
                                                }
                                            }
                                        };
                                HnsActivity.getHnsActivity().showPlaylistWarningDialog(
                                        playlistWarningDialogListener);

                            } else {
                                addMediaToPlaylist(items);
                            }
                        } else if (playlistOptionsModel.getOptionType()
                                == PlaylistOptions.OPEN_PLAYLIST) {
                            HnsActivity.getHnsActivity().openPlaylistActivity(
                                    getContext(), ConstantUtils.DEFAULT_PLAYLIST);
                        } else if (playlistOptionsModel.getOptionType()
                                == PlaylistOptions.PLAYLIST_SETTINGS) {
                            HnsActivity.getHnsActivity().openHnsPlaylistSettings();
                        }
                    } catch (HnsActivity.HnsActivityNotFoundException e) {
                        Log.e(TAG, "showPlaylistButton onOptionClicked " + e);
                    }
                }
            };
            if (!isPlaylistButtonVisible()) {
                PlaylistViewUtils.showPlaylistButton(
                        HnsActivity.getHnsActivity(), viewGroup, playlistOptionsListener);
                if (SharedPreferencesManager.getInstance().readBoolean(
                            PlaylistPreferenceUtils.SHOULD_SHOW_PLAYLIST_ONBOARDING, true)) {
                    View playlistButton = viewGroup.findViewById(R.id.playlist_button_id);
                    if (playlistButton != null) {
                        playlistButton.post(new Runnable() {
                            @Override
                            public void run() {
                                PlaylistOnboardingActionClickListener
                                        playlistOnboardingActionClickListener =
                                                new PlaylistOnboardingActionClickListener() {
                                                    @Override
                                                    public void onOnboardingActionClick() {
                                                        addMediaToPlaylist(items);
                                                    }
                                                };
                                try {
                                    new PlaylistOnboardingPanel(
                                            (FragmentActivity) HnsActivity.getHnsActivity(),
                                            playlistButton, playlistOnboardingActionClickListener);
                                } catch (HnsActivity.HnsActivityNotFoundException e) {
                                    Log.e(TAG, "showPlaylistButton " + e);
                                }
                            }
                        });
                    }
                    SharedPreferencesManager.getInstance().writeBoolean(
                            PlaylistPreferenceUtils.SHOULD_SHOW_PLAYLIST_ONBOARDING, false);
                }
            }
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "showPlaylistButton " + e);
        }
    }

    private void addMediaToPlaylist(PlaylistItem[] items) {
        if (mPlaylistService == null) {
            return;
        }
        mPlaylistService.addMediaFiles(items, ConstantUtils.DEFAULT_PLAYLIST,
                shouldCacheMediaFilesForPlaylist(), addedItems -> {});
        int mediaCount = SharedPreferencesManager.getInstance().readInt(
                PlaylistPreferenceUtils.ADD_MEDIA_COUNT);
        if (mediaCount < PLAYLIST_MEDIA_COUNT_LIMIT) {
            SharedPreferencesManager.getInstance().writeInt(
                    PlaylistPreferenceUtils.ADD_MEDIA_COUNT, mediaCount + 1);
        }
    }

    private void addMediaToPlaylist() {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (mPlaylistService == null || currentTab == null) {
            return;
        }
        org.chromium.url.mojom.Url contentUrl = new org.chromium.url.mojom.Url();
        contentUrl.url = currentTab.getUrl().getSpec();
        mPlaylistService.addMediaFilesFromPageToPlaylist(
                ConstantUtils.DEFAULT_PLAYLIST, contentUrl, shouldCacheMediaFilesForPlaylist());
    }

    private void showAddedToPlaylistSnackBar() {
        SnackBarActionModel snackBarActionModel =
                new SnackBarActionModel(getContext().getResources().getString(R.string.view_action),
                        new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                try {
                                    HnsActivity.getHnsActivity().openPlaylistActivity(
                                            getContext(), ConstantUtils.DEFAULT_PLAYLIST);
                                } catch (HnsActivity.HnsActivityNotFoundException e) {
                                    Log.e(TAG, "showAddedToPlaylistSnackBar onClick " + e);
                                }
                            }
                        });
        try {
            ViewGroup viewGroup =
                    HnsActivity.getHnsActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);
            PlaylistViewUtils.showSnackBarWithActions(viewGroup,
                    String.format(getContext().getResources().getString(R.string.added_to_playlist),
                            getContext().getResources().getString(R.string.playlist_play_later)),
                    snackBarActionModel);
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "showAddedToPlaylistSnackBar " + e);
        }
    }

    private boolean shouldCacheMediaFilesForPlaylist() {
        boolean shouldCacheOnlyOnWifi =
                (SharedPreferencesManager.getInstance().readInt(
                         HnsPlaylistPreferences.PREF_AUTO_SAVE_MEDIA_FOR_OFFLINE, 0)
                                == 2
                        && ConnectionUtils.isWifiAvailable(getContext()));

        boolean shouldCache = SharedPreferencesManager.getInstance().readInt(
                                      HnsPlaylistPreferences.PREF_AUTO_SAVE_MEDIA_FOR_OFFLINE, 0)
                        == 0
                || shouldCacheOnlyOnWifi;
        return shouldCache;
    }

    private void checkForTooltip(Tab tab) {
        try {
            if (!HnsShieldsUtils.isTooltipShown
                    && !HnsActivity.getHnsActivity().mIsDeepLink) {
                if (!HnsShieldsUtils.hasShieldsTooltipShown(
                            HnsShieldsUtils.PREF_SHIELDS_TOOLTIP)
                        && mHnsShieldsHandler.getTrackersBlockedCount(tab.getId())
                                        + mHnsShieldsHandler.getAdsBlockedCount(tab.getId())
                                > 0) {
                    showTooltip(HnsShieldsUtils.PREF_SHIELDS_TOOLTIP, tab.getId());
                }
            }
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "checkForTooltip " + e);
        }
    }

    private void showTooltip(String tooltipPref, int tabId) {
        try {
            HighlightView highlightView = new HighlightView(getContext(), null);
            highlightView.setColor(ContextCompat.getColor(
                    getContext(), R.color.onboarding_search_highlight_color));
            ViewGroup viewGroup =
                    HnsActivity.getHnsActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);
            float padding = (float) dpToPx(getContext(), 20);
            mShieldsPopupWindowTooltip =
                    new PopupWindowTooltip.Builder(getContext())
                            .anchorView(mHnsShieldsButton)
                            .arrowColor(ContextCompat.getColor(
                                    getContext(), R.color.onboarding_arrow_color))
                            .gravity(Gravity.BOTTOM)
                            .dismissOnOutsideTouch(true)
                            .dismissOnInsideTouch(false)
                            .backgroundDimDisabled(true)
                            .padding(padding)
                            .parentPaddingHorizontal(dpToPx(getContext(), 10))
                            .modal(true)
                            .onDismissListener(tooltip -> {
                                if (viewGroup != null && highlightView != null) {
                                    highlightView.stopAnimation();
                                    viewGroup.removeView(highlightView);
                                }
                            })
                            .contentView(R.layout.hns_shields_tooltip_layout)
                            .build();

            ArrayList<String> blockerNamesList = mHnsShieldsHandler.getBlockerNamesList(tabId);

            int adsTrackersCount = mHnsShieldsHandler.getTrackersBlockedCount(tabId)
                    + mHnsShieldsHandler.getAdsBlockedCount(tabId);

            String displayTrackerName = "";
            if (blockerNamesList.contains(BigtechCompany.Google.name())) {
                displayTrackerName = BigtechCompany.Google.name();
            } else if (blockerNamesList.contains(BigtechCompany.Facebook.name())) {
                displayTrackerName = BigtechCompany.Facebook.name();
            } else if (blockerNamesList.contains(BigtechCompany.Amazon.name())) {
                displayTrackerName = BigtechCompany.Amazon.name();
            }

            String trackerText = "";
            if (!displayTrackerName.isEmpty()) {
                if (adsTrackersCount - 1 == 0) {
                    trackerText =
                            String.format(getContext().getResources().getString(
                                                  R.string.shield_bigtech_tracker_only_blocked),
                                    displayTrackerName);

                } else {
                    trackerText = String.format(getContext().getResources().getString(
                                                        R.string.shield_bigtech_tracker_blocked),
                            displayTrackerName, String.valueOf(adsTrackersCount - 1));
                }
            } else {
                trackerText = String.format(
                        getContext().getResources().getString(R.string.shield_tracker_blocked),
                        String.valueOf(adsTrackersCount));
            }

            TextView tvBlocked = mShieldsPopupWindowTooltip.findViewById(R.id.tv_blocked);
            tvBlocked.setText(trackerText);

            if (mHnsShieldsButton != null && mHnsShieldsButton.isShown()) {
                viewGroup.addView(highlightView);
                HighlightItem item = new HighlightItem(mHnsShieldsButton);

                ImageButton hnsShieldButton =
                        new ImageButton(getContext(), null, R.style.ToolbarButton);
                hnsShieldButton.setImageResource(R.drawable.btn_hns);
                FrameLayout.LayoutParams hnsShieldParams =
                        new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                FrameLayout.LayoutParams.WRAP_CONTENT);

                int[] location = new int[2];
                highlightView.getLocationOnScreen(location);
                hnsShieldParams.leftMargin = item.getScreenLeft() + dpToPx(getContext(), 10);
                hnsShieldParams.topMargin = item.getScreenTop()
                        + ((item.getScreenBottom() - item.getScreenTop()) / 4) - location[1];
                hnsShieldButton.setLayoutParams(hnsShieldParams);
                highlightView.addView(hnsShieldButton);

                highlightView.setShouldShowHighlight(true);
                highlightView.setHighlightTransparent(true);
                highlightView.setHighlightItem(item);
                highlightView.initializeAnimators();
                highlightView.startAnimation();

                mShieldsPopupWindowTooltip.show();
                HnsShieldsUtils.setShieldsTooltipShown(tooltipPref, true);
                HnsShieldsUtils.isTooltipShown = true;
            }

        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "showTooltip " + e);
        }
    }

    public void dismissShieldsTooltip() {
        if (mShieldsPopupWindowTooltip != null && mShieldsPopupWindowTooltip.isShowing()) {
            mShieldsPopupWindowTooltip.dismiss();
            mShieldsPopupWindowTooltip = null;
        }
    }

    public void reopenShieldsPanel() {
        if (mHnsShieldsHandler != null && mHnsShieldsHandler.isShowing()) {
            mHnsShieldsHandler.hideHnsShieldsMenu();
            showShieldsMenu(mHnsShieldsButton);
        }
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        dismissShieldsTooltip();
        reopenShieldsPanel();
        // TODO: show wallet panel
    }

    private void showHnsRewardsOnboardingModal() {
        Context context = getContext();
        final Dialog dialog = new Dialog(context);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setCancelable(false);
        dialog.setContentView(R.layout.hns_rewards_onboarding_modal);
        dialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);

        View hnsRewardsOnboardingModalView =
                dialog.findViewById(R.id.hns_rewards_onboarding_modal_layout);

        String tosText =
                String.format(context.getResources().getString(R.string.hns_rewards_tos_text),
                        context.getResources().getString(R.string.terms_of_service),
                        context.getResources().getString(R.string.privacy_policy));
        int termsOfServiceIndex =
                tosText.indexOf(context.getResources().getString(R.string.terms_of_service));
        Spanned tosTextSpanned = HnsRewardsHelper.spannedFromHtmlString(tosText);
        SpannableString tosTextSS = new SpannableString(tosTextSpanned.toString());

        ClickableSpan tosClickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View textView) {
                CustomTabActivity.showInfoPage(context, HnsActivity.HNS_TERMS_PAGE);
            }
            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setUnderlineText(false);
            }
        };

        tosTextSS.setSpan(tosClickableSpan, termsOfServiceIndex,
                termsOfServiceIndex
                        + context.getResources().getString(R.string.terms_of_service).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        tosTextSS.setSpan(new ForegroundColorSpan(context.getResources().getColor(
                                  R.color.hns_rewards_modal_theme_color)),
                termsOfServiceIndex,
                termsOfServiceIndex
                        + context.getResources().getString(R.string.terms_of_service).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

        ClickableSpan privacyProtectionClickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View textView) {
                CustomTabActivity.showInfoPage(context, HnsActivity.HNS_PRIVACY_POLICY);
            }
            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setUnderlineText(false);
            }
        };

        int privacyPolicyIndex =
                tosText.indexOf(context.getResources().getString(R.string.privacy_policy));
        tosTextSS.setSpan(privacyProtectionClickableSpan, privacyPolicyIndex,
                privacyPolicyIndex
                        + context.getResources().getString(R.string.privacy_policy).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        tosTextSS.setSpan(new ForegroundColorSpan(context.getResources().getColor(
                                  R.color.hns_rewards_modal_theme_color)),
                privacyPolicyIndex,
                privacyPolicyIndex
                        + context.getResources().getString(R.string.privacy_policy).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

        TextView tosAndPpText = hnsRewardsOnboardingModalView.findViewById(
                R.id.hns_rewards_onboarding_modal_tos_pp_text);
        tosAndPpText.setMovementMethod(LinkMovementMethod.getInstance());
        tosAndPpText.setText(tosTextSS);

        TextView takeQuickTourButton =
                hnsRewardsOnboardingModalView.findViewById(R.id.take_quick_tour_button);
        takeQuickTourButton.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                HnsRewardsHelper.setShowHnsRewardsOnboardingOnce(true);
                openRewardsPanel();
                dialog.dismiss();
            }
        }));
        TextView btnHnsRewards =
                hnsRewardsOnboardingModalView.findViewById(R.id.start_using_hns_rewards_text);
        btnHnsRewards.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                HnsRewardsHelper.setShowDeclareGeoModal(true);
                openRewardsPanel();
                dialog.dismiss();
            }
        }));

        dialog.show();
    }

    private void addSavedBandwidthToDb(long savings) {
        new AsyncTask<Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    SavedBandwidthTable savedBandwidthTable = new SavedBandwidthTable(
                            savings, HnsStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                    long rowId = mDatabaseHelper.insertSavedBandwidth(savedBandwidthTable);
                } catch (Exception e) {
                    // Do nothing if url is invalid.
                    // Just return w/o showing shields popup.
                    return null;
                }
                return null;
            }
            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void addStatsToDb(String statType, String statSite, String url) {
        new AsyncTask<Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    URL urlObject = new URL(url);
                    URL siteObject = new URL(statSite);
                    HnsStatsTable hnsStatsTable = new HnsStatsTable(url, urlObject.getHost(),
                            statType, statSite, siteObject.getHost(),
                            HnsStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                    long rowId = mDatabaseHelper.insertStats(hnsStatsTable);
                } catch (Exception e) {
                    // Do nothing if url is invalid.
                    // Just return w/o showing shields popup.
                    return null;
                }
                return null;
            }
            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    public boolean isWalletIconVisible() {
        if (mWalletLayout == null) {
            return false;
        }
        return mWalletLayout.getVisibility() == View.VISIBLE;
    }

    public void showWalletIcon(boolean show, Tab tab) {
        // The layout could be null in Custom Tabs layout
        if (mWalletLayout == null) {
            return;
        }
        Tab currentTab = tab;
        if (currentTab == null) {
            currentTab = getToolbarDataProvider().getTab();
            if (currentTab == null) {
                return;
            }
        }
        if (show) {
            mWalletLayout.setVisibility(View.VISIBLE);
            mTabsWithWalletIcon.add(currentTab.getId());
        } else {
            mWalletLayout.setVisibility(View.GONE);
            mTabsWithWalletIcon.remove(currentTab.getId());
        }
    }

    public void showWalletIcon(boolean show) {
        showWalletIcon(show, null);
    }

    public void hideRewardsOnboardingIcon() {
        if (mHnsRewardsOnboardingIcon != null) {
            mHnsRewardsOnboardingIcon.setVisibility(View.GONE);
        }
        if (mHnsRewardsNotificationsCount != null) {
            mHnsRewardsNotificationsCount.setVisibility(View.GONE);
        }
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putBoolean(HnsRewardsPanel.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, true);
        editor.apply();
    }

    @Override
    public void onClickImpl(View v) {
        if (mHnsShieldsHandler == null) {
            assert false;
            return;
        }
        if (mHnsShieldsButton == v && mHnsShieldsButton != null) {
            showShieldsMenu(mHnsShieldsButton);
        } else if (mHnsRewardsButton == v && mHnsRewardsButton != null) {
            if (null != mRewardsPopup) {
                return;
            }
            hideRewardsOnboardingIcon();
            OnboardingPrefManager.getInstance().setOnboardingShown(true);
            mRewardsPopup = new HnsRewardsPanel(v);
            mRewardsPopup.showLikePopDownMenu();
            if (mHnsRewardsNotificationsCount.isShown()) {
                SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
                SharedPreferences.Editor editor = sharedPref.edit();
                editor.putBoolean(HnsRewardsPanel.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, true);
                editor.apply();
                mHnsRewardsNotificationsCount.setVisibility(View.INVISIBLE);
                mIsInitialNotificationPosted = false;
            }
        } else if (mHomeButton == v) {
            // Helps Hns News know how to behave on home button action
            try {
                HnsActivity.getHnsActivity().setComesFromNewTab(true);
            } catch (HnsActivity.HnsActivityNotFoundException e) {
                Log.e(TAG, "HomeButton click " + e);
            }
        } else if (mHnsWalletButton == v && mHnsWalletButton != null) {
            maybeShowWalletPanel(v);
        }
    }

    private void maybeShowWalletPanel(View v) {
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            activity.showWalletPanel(true);
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "maybeShowWalletPanel " + e);
        }
    }

    private void showWalletPanelInternal(View v) {
        mDAppsWalletController =
                new DAppsWalletController(getContext(), v, dialog -> mDAppsWalletController = null);
        mDAppsWalletController.showWalletPanel();
    }

    public void showWalletPanel() {
        dismissWalletPanelOrDialog();
        showWalletPanelInternal(this);
    }

    @Override
    public void onClick(View v) {
        onClickImpl(v);
    }

    private boolean checkForRewardsOnboarding() {
        return PackageUtils.isFirstInstall(getContext()) && mHnsRewardsNativeWorker != null
                && !mHnsRewardsNativeWorker.isRewardsEnabled()
                && mHnsRewardsNativeWorker.IsSupported()
                && !OnboardingPrefManager.getInstance().isOnboardingShown();
    }

    private void showShieldsMenu(View mHnsShieldsButton) {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (currentTab == null) {
            return;
        }
        try {
            URL url = new URL(currentTab.getUrl().getSpec());
            // Don't show shields popup if protocol is not valid for shields.
            if (!isValidProtocolForShields(url.getProtocol())) {
                return;
            }
            mHnsShieldsHandler.show(mHnsShieldsButton, currentTab);
        } catch (Exception e) {
            // Do nothing if url is invalid.
            // Just return w/o showing shields popup.
            return;
        }
    }

    @Override
    public boolean onLongClickImpl(View v) {
        // Use null as the default description since Toast.showAnchoredToast
        // will return false if it is null.
        String description = null;
        Context context = getContext();
        Resources resources = context.getResources();

        if (v == mHnsShieldsButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_hns_shields);
        } else if (v == mHnsRewardsButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_hns_rewards);
        } else if (v == mHomeButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_home);
        } else if (v == mHnsWalletButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_hns_wallet);
        }

        return Toast.showAnchoredToast(context, v, description);
    }

    @Override
    public boolean onLongClick(View v) {
        return onLongClickImpl(v);
    }

    @Override
    public void onUrlFocusChange(boolean hasFocus) {
        Context context = getContext();
        String countryCode = Locale.getDefault().getCountry();
        try {
            if (hasFocus && PackageUtils.isFirstInstall(context)
                    && HnsActivity.getHnsActivity().getActivityTab() != null
                    && UrlUtilities.isNTPUrl(
                            HnsActivity.getHnsActivity().getActivityTab().getUrl().getSpec())
                    && !OnboardingPrefManager.getInstance().hasSearchEngineOnboardingShown()
                    && OnboardingPrefManager.getInstance().getUrlFocusCount() == 1
                    && !HNS_SEARCH_ENGINE_DEFAULT_REGIONS.contains(countryCode)) {
                Intent searchActivityIntent = new Intent(context, SearchActivity.class);
                searchActivityIntent.setAction(Intent.ACTION_VIEW);
                context.startActivity(searchActivityIntent);
            }

        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "onUrlFocusChange " + e);
        }

        // We need to enable the promo for later release.
        // Delay showing the panel. Otherwise there are ANRs on holding onUrlFocusChange
        /* PostTask.postTask(TaskTraits.UI_DEFAULT, () -> {
            int appOpenCountForWidgetPromo = SharedPreferencesManager.getInstance().readInt(
                    HnsPreferenceKeys.HNS_APP_OPEN_COUNT_FOR_WIDGET_PROMO);
            if (hasFocus
                    && appOpenCountForWidgetPromo >= HnsActivity.APP_OPEN_COUNT_FOR_WIDGET_PROMO)
                mSearchWidgetPromoPanel.showIfNeeded(this);
        }); */

        if (OnboardingPrefManager.getInstance().getUrlFocusCount() == 0) {
            OnboardingPrefManager.getInstance().updateUrlFocusCount();
        }
        super.onUrlFocusChange(hasFocus);
    }

    @Override
    public void populateUrlAnimatorSetImpl(boolean showExpandedState,
            int urlFocusToolbarButtonsDuration, int urlClearFocusTabStackDelayMs,
            List<Animator> animators) {
        if (mHnsShieldsButton != null) {
            Animator animator;
            if (showExpandedState) {
                float density = getContext().getResources().getDisplayMetrics().density;
                boolean isRtl = getLayoutDirection() == LAYOUT_DIRECTION_RTL;
                float toolbarButtonTranslationX =
                        MathUtils.flipSignIf(URL_FOCUS_TOOLBAR_BUTTONS_TRANSLATION_X_DP, isRtl)
                        * density;
                animator = ObjectAnimator.ofFloat(
                        mHnsShieldsButton, TRANSLATION_X, toolbarButtonTranslationX);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setInterpolator(Interpolators.FAST_OUT_LINEAR_IN_INTERPOLATOR);
                animators.add(animator);

                animator = ObjectAnimator.ofFloat(mHnsShieldsButton, ALPHA, 0);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setInterpolator(Interpolators.FAST_OUT_LINEAR_IN_INTERPOLATOR);
                animators.add(animator);
            } else {
                animator = ObjectAnimator.ofFloat(mHnsShieldsButton, TRANSLATION_X, 0);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setStartDelay(urlClearFocusTabStackDelayMs);
                animator.setInterpolator(Interpolators.FAST_OUT_SLOW_IN_INTERPOLATOR);
                animators.add(animator);

                animator = ObjectAnimator.ofFloat(mHnsShieldsButton, ALPHA, 1);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setStartDelay(urlClearFocusTabStackDelayMs);
                animator.setInterpolator(Interpolators.FAST_OUT_SLOW_IN_INTERPOLATOR);
                animators.add(animator);
            }
        }
    }

    @Override
    public void updateModernLocationBarColorImpl(int color) {
        mCurrentToolbarColor = color;
        if (mShieldsLayout != null) {
            mShieldsLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
        if (mRewardsLayout != null) {
            mRewardsLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
        if (mWalletLayout != null) {
            mWalletLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
    }

    /**
     * If |tab| is null, set disabled image to shields button and |urlString| is
     * ignored.
     * If |urlString| is null, url is fetched from |tab|.
     */
    private void updateHnsShieldsButtonState(Tab tab) {
        if (mHnsShieldsButton == null) {
            assert false;
            return;
        }

        if (tab == null) {
            mHnsShieldsButton.setImageResource(R.drawable.btn_hns_off);
            return;
        }
        mHnsShieldsButton.setImageResource(
                isShieldsOnForTab(tab) ? R.drawable.btn_hns : R.drawable.btn_hns_off);

        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();

        if (mRewardsLayout == null) return;
        if (isIncognito()) {
            mRewardsLayout.setVisibility(View.GONE);
            updateShieldsLayoutBackground(true);
        } else if (isNativeLibraryReady() && mHnsRewardsNativeWorker != null
                && mHnsRewardsNativeWorker.IsSupported()
                && !HnsPrefServiceBridge.getInstance().getSafetynetCheckFailed()
                && sharedPreferences.getBoolean(
                        AppearancePreferences.PREF_SHOW_HNS_REWARDS_ICON, true)) {
            mRewardsLayout.setVisibility(View.VISIBLE);
            updateShieldsLayoutBackground(false);
        }
    }

    private boolean isShieldsOnForTab(Tab tab) {
        if (!isNativeLibraryReady() || tab == null
                || Profile.fromWebContents(((TabImpl) tab).getWebContents()) == null) {
            return false;
        }

        return HnsShieldsContentSettings.getShields(
                Profile.fromWebContents(((TabImpl) tab).getWebContents()), tab.getUrl().getSpec(),
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_HNS_SHIELDS);
    }

    private boolean isValidProtocolForShields(String protocol) {
        if (protocol.equals("http") || protocol.equals("https")) {
            return true;
        }

        return false;
    }

    public void dismissRewardsPanel() {
        if (mRewardsPopup != null) {
            mRewardsPopup.dismiss();
            mRewardsPopup = null;
        }
    }

    public void dismissWalletPanelOrDialog() {
        if (mDAppsWalletController != null) {
            mDAppsWalletController.dismiss();
            mDAppsWalletController = null;
        }
    }

    public void onRewardsPanelDismiss() {
        mRewardsPopup = null;
    }

    public void openRewardsPanel() {
        onClick(mHnsRewardsButton);
    }

    public boolean isShieldsTooltipShown() {
        if (mShieldsPopupWindowTooltip != null) {
            return mShieldsPopupWindowTooltip.isShowing();
        }
        return false;
    }

    @Override
    public void onCompleteReset(boolean success) {
        if (success) {
            HnsRewardsHelper.resetRewards();
            try {
                HnsRelaunchUtils.askForRelaunch(HnsActivity.getHnsActivity());
            } catch (HnsActivity.HnsActivityNotFoundException e) {
                Log.e(TAG, "onCompleteReset " + e);
            }
        }
    }

    @Override
    public void OnNotificationAdded(String id, int type, long timestamp, String[] args) {
        if (mHnsRewardsNativeWorker == null) {
            return;
        }

        if (type == HnsRewardsNativeWorker.REWARDS_NOTIFICATION_GRANT) {
            // Set flag
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(
                    HnsRewardsPanel.PREF_GRANTS_NOTIFICATION_RECEIVED, true);
            sharedPreferencesEditor.apply();
        }
        mHnsRewardsNativeWorker.GetAllNotifications();
    }

    private boolean mayShowHnsAdsOnboardingDialog() {
        Context context = getContext();

        if (HnsAdsSignupDialog.shouldShowNewUserDialog(context)) {
            HnsAdsSignupDialog.showNewUserDialog(getContext());
            return true;
        } else if (HnsAdsSignupDialog.shouldShowNewUserDialogIfRewardsIsSwitchedOff(context)) {
            HnsAdsSignupDialog.showNewUserDialog(getContext());
            return true;
        } else if (HnsAdsSignupDialog.shouldShowExistingUserDialog(context)) {
            HnsAdsSignupDialog.showExistingUserDialog(getContext());
            return true;
        }

        return false;
    }

    @Override
    public void OnNotificationsCount(int count) {
        if (mHnsRewardsNotificationsCount != null) {
            if (count != 0) {
                String value = Integer.toString(count);
                if (count > 99) {
                    mHnsRewardsNotificationsCount.setBackground(
                            ResourcesCompat.getDrawable(getContext().getResources(),
                                    R.drawable.hns_rewards_rectangle, /* theme= */ null));
                    value = "99+";
                } else {
                    mHnsRewardsNotificationsCount.setBackground(
                            ResourcesCompat.getDrawable(getContext().getResources(),
                                    R.drawable.hns_rewards_circle, /* theme= */ null));
                }
                mHnsRewardsNotificationsCount.setText(value);
                mHnsRewardsNotificationsCount.setVisibility(View.VISIBLE);
                mIsNotificationPosted = true;
            } else {
                mHnsRewardsNotificationsCount.setText("");
                mHnsRewardsNotificationsCount.setBackgroundResource(0);
                mHnsRewardsNotificationsCount.setVisibility(View.INVISIBLE);
                mIsNotificationPosted = false;
                updateVerifiedPublisherMark();
            }
        }

        updateNotificationBadgeForNewInstall();
        if (!PackageUtils.isFirstInstall(getContext())
                && !OnboardingPrefManager.getInstance().isAdsAvailable()) {
            mayShowHnsAdsOnboardingDialog();
        }

        if (checkForRewardsOnboarding()) {
            if (mHnsRewardsOnboardingIcon != null) {
                mHnsRewardsOnboardingIcon.setVisibility(View.VISIBLE);
            }
            if (mHnsRewardsNotificationsCount != null) {
                mHnsRewardsNotificationsCount.setVisibility(View.GONE);
            }
        }
    }

    private void updateNotificationBadgeForNewInstall() {
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        boolean shownBefore = sharedPref.getBoolean(
                HnsRewardsPanel.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, false);
        boolean shouldShow = mHnsRewardsNotificationsCount != null && !shownBefore;
        mIsInitialNotificationPosted = shouldShow; // initial notification

        if (!shouldShow) return;

        mHnsRewardsNotificationsCount.setText("");
        mHnsRewardsNotificationsCount.setBackground(ResourcesCompat.getDrawable(
                getContext().getResources(), R.drawable.hns_rewards_circle, /* theme= */ null));
        mHnsRewardsNotificationsCount.setVisibility(View.VISIBLE);
    }

    @Override
    public void onThemeColorChanged(int color, boolean shouldAnimate) {
        if (mWalletIcon != null) {
            ImageViewCompat.setImageTintList(mWalletIcon,
                    !ColorUtils.shouldUseLightForegroundOnBackground(color) ? mDarkModeTint
                                                                            : mLightModeTint);
        }

        final int textBoxColor = ThemeUtils.getTextBoxColorForToolbarBackgroundInNonNativePage(
                getContext(), color, isIncognito());
        updateModernLocationBarColorImpl(textBoxColor);
    }

    /**
     * HnsRewardsNativeWorker.PublisherObserver:
     *   Update a 'verified publisher' checkmark on url bar BAT icon only if
     *   no notifications are posted.
     */
    @Override
    public void onFrontTabPublisherChanged(boolean verified) {
        mIsPublisherVerified = verified;
        updateVerifiedPublisherMark();
    }

    private void updateVerifiedPublisherMark() {
        if (mHnsRewardsNotificationsCount == null) {
            // Most likely we are on a custom page
            return;
        }
        if (mIsInitialNotificationPosted) {
            return;
        } else if (!mIsNotificationPosted) {
            if (mIsPublisherVerified) {
                mHnsRewardsNotificationsCount.setVisibility(View.VISIBLE);
                mHnsRewardsNotificationsCount.setBackground(
                        ResourcesCompat.getDrawable(getContext().getResources(),
                                R.drawable.rewards_verified_tick_icon, /* theme= */ null));
            } else {
                mHnsRewardsNotificationsCount.setBackgroundResource(0);
                mHnsRewardsNotificationsCount.setVisibility(View.INVISIBLE);
            }
        }
    }

    public void onBottomToolbarVisibilityChanged(boolean isVisible) {
        mIsBottomToolbarVisible = isVisible;
        if (HnsReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)
                && getMenuButtonCoordinator() != null) {
            getMenuButtonCoordinator().setVisibility(!isVisible);
            ToggleTabStackButton toggleTabStackButton = findViewById(R.id.tab_switcher_button);
            if (toggleTabStackButton != null) {
                toggleTabStackButton.setVisibility(isTabSwitcherOnBottom() ? GONE : VISIBLE);
            }
        }
    }

    private void updateShieldsLayoutBackground(boolean rounded) {
        if (mShieldsLayout == null) {
            return;
        }

        mShieldsLayout.setBackgroundDrawable(
                ApiCompatibilityUtils.getDrawable(getContext().getResources(),
                        rounded ? R.drawable.modern_toolbar_background_grey_end_segment
                                : R.drawable.modern_toolbar_background_grey_middle_segment));

        updateModernLocationBarColorImpl(mCurrentToolbarColor);
    }

    private boolean isTabSwitcherOnBottom() {
        return mIsBottomToolbarVisible && BottomToolbarVariationManager.isTabSwitcherOnBottom();
    }

    private boolean isMenuButtonOnBottom() {
        return mIsBottomToolbarVisible && BottomToolbarVariationManager.isMenuButtonOnBottom();
    }

    @Override
    public void initialize(ToolbarDataProvider toolbarDataProvider,
            ToolbarTabController tabController, MenuButtonCoordinator menuButtonCoordinator,
            HistoryDelegate historyDelegate, BooleanSupplier partnerHomepageEnabledSupplier,
            OfflineDownloader offlineDownloader) {
        super.initialize(toolbarDataProvider, tabController, menuButtonCoordinator, historyDelegate,
                partnerHomepageEnabledSupplier, offlineDownloader);

        HnsMenuButtonCoordinator.setMenuFromBottom(isMenuButtonOnBottom());
    }

    public void updateWalletBadgeVisibility(boolean visible) {
        assert mHnsWalletBadge != null;
        mHnsWalletBadge.setVisibility(visible ? View.VISIBLE : View.GONE);
    }

    public void updateMenuButtonState() {
        HnsMenuButtonCoordinator.setMenuFromBottom(mIsBottomToolbarVisible);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (HnsReflectionUtil.EqualTypes(this.getClass(), CustomTabToolbar.class)
                || HnsReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)) {
            updateMenuButtonState();
            Tab tab = getToolbarDataProvider() != null ? getToolbarDataProvider().getTab() : null;
            if (tab != null && ((TabImpl) tab).getWebContents() != null) {
                updateHnsShieldsButtonState(tab);
            }
        }
        super.onDraw(canvas);
    }

    @Override
    public boolean isLocationBarValid(LocationBarCoordinator locationBar) {
        return locationBar != null && locationBar.getPhoneCoordinator() != null
                && locationBar.getPhoneCoordinator().getViewForDrawing() != null;
    }

    @Override
    public void drawAnimationOverlay(ViewGroup toolbarButtonsContainer, Canvas canvas) {
        if (mWalletLayout != null && mWalletLayout.getVisibility() != View.GONE) {
            canvas.save();
            ViewUtils.translateCanvasToView(toolbarButtonsContainer, mWalletLayout, canvas);
            mWalletLayout.draw(canvas);
            canvas.restore();
        }
        if (mShieldsLayout != null && mShieldsLayout.getVisibility() != View.GONE) {
            canvas.save();
            ViewUtils.translateCanvasToView(toolbarButtonsContainer, mShieldsLayout, canvas);
            mShieldsLayout.draw(canvas);
            canvas.restore();
        }
        if (mRewardsLayout != null && mRewardsLayout.getVisibility() != View.GONE) {
            canvas.save();
            ViewUtils.translateCanvasToView(toolbarButtonsContainer, mRewardsLayout, canvas);
            mRewardsLayout.draw(canvas);
            canvas.restore();
        }
    }

    @Override
    public void onEvent(int eventType, String playlistId) {
        if (eventType == PlaylistEvent.ITEM_ADDED) {
            showAddedToPlaylistSnackBar();
        }
    }

    @Override
    public void onMediaFilesUpdated(Url pageUrl, PlaylistItem[] items) {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (currentTab == null || !pageUrl.url.equals(currentTab.getUrl().getSpec())) {
            return;
        }
        showPlaylistButton(items);
    }
}
