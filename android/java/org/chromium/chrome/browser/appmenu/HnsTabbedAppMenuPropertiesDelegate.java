/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.appmenu;

import android.content.Context;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;
import android.widget.ImageButton;

import androidx.annotation.NonNull;
import androidx.appcompat.content.res.AppCompatResources;

import org.chromium.base.HnsFeatureList;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.OneshotSupplier;
import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.ActivityTabProvider;
import org.chromium.chrome.browser.HnsRewardsNativeWorker;
import org.chromium.chrome.browser.app.appmenu.AppMenuIconRowFooter;
import org.chromium.chrome.browser.bookmarks.BookmarkModel;
import org.chromium.chrome.browser.feed.webfeed.WebFeedSnackbarController;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.incognito.reauth.IncognitoReauthController;
import org.chromium.chrome.browser.layouts.LayoutStateProvider;
import org.chromium.chrome.browser.multiwindow.MultiWindowModeStateDispatcher;
import org.chromium.chrome.browser.playlist.settings.HnsPlaylistPreferences;
import org.chromium.chrome.browser.preferences.HnsPref;
import org.chromium.chrome.browser.preferences.HnsPrefServiceBridge;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.readaloud.ReadAloudController;
import org.chromium.chrome.browser.set_default_browser.HnsSetDefaultBrowserUtils;
import org.chromium.chrome.browser.speedreader.HnsSpeedReaderUtils;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tabbed_mode.TabbedAppMenuPropertiesDelegate;
import org.chromium.chrome.browser.tabmodel.TabModelSelector;
import org.chromium.chrome.browser.toolbar.ToolbarManager;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.toolbar.menu_button.HnsMenuButtonCoordinator;
import org.chromium.chrome.browser.ui.appmenu.AppMenuDelegate;
import org.chromium.chrome.browser.ui.appmenu.AppMenuHandler;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager;
import org.chromium.chrome.browser.vpn.utils.HnsVpnProfileUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnUtils;
import org.chromium.chrome.features.start_surface.StartSurface;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.ui.modaldialog.ModalDialogManager;

public class HnsTabbedAppMenuPropertiesDelegate extends TabbedAppMenuPropertiesDelegate {
    private Menu mMenu;
    private AppMenuDelegate mAppMenuDelegate;
    private ObservableSupplier<BookmarkModel> mBookmarkModelSupplier;

    public HnsTabbedAppMenuPropertiesDelegate(Context context,
            ActivityTabProvider activityTabProvider,
            MultiWindowModeStateDispatcher multiWindowModeStateDispatcher,
            TabModelSelector tabModelSelector, ToolbarManager toolbarManager, View decorView,
            AppMenuDelegate appMenuDelegate,
            OneshotSupplier<LayoutStateProvider> layoutStateProvider,
            OneshotSupplier<StartSurface> startSurfaceSupplier,
            ObservableSupplier<BookmarkModel> bookmarkModelSupplier,
            WebFeedSnackbarController.FeedLauncher feedLauncher,
            ModalDialogManager modalDialogManager, SnackbarManager snackbarManager,
            @NonNull OneshotSupplier<IncognitoReauthController>
                    incognitoReauthControllerOneshotSupplier,
            Supplier<ReadAloudController> readAloudControllerSupplier) {
        super(context, activityTabProvider, multiWindowModeStateDispatcher, tabModelSelector,
                toolbarManager, decorView, appMenuDelegate, layoutStateProvider,
                startSurfaceSupplier, bookmarkModelSupplier, feedLauncher, modalDialogManager,
                snackbarManager, incognitoReauthControllerOneshotSupplier,
                readAloudControllerSupplier);

        mAppMenuDelegate = appMenuDelegate;
        mBookmarkModelSupplier = bookmarkModelSupplier;
    }

    @Override
    public void prepareMenu(Menu menu, AppMenuHandler handler) {
        super.prepareMenu(menu, handler);

        mMenu = menu;

        if (HnsVpnUtils.isVpnFeatureSupported(mContext)) {
            SubMenu vpnSubMenu = menu.findItem(R.id.request_hns_vpn_row_menu_id).getSubMenu();
            MenuItem hnsVpnSubMenuItem = vpnSubMenu.findItem(R.id.request_hns_vpn_id);
            if (shouldShowIconBeforeItem()) {
                hnsVpnSubMenuItem.setIcon(
                        AppCompatResources.getDrawable(mContext, R.drawable.ic_vpn));
            }
            MenuItem hnsVpnCheckedSubMenuItem =
                    vpnSubMenu.findItem(R.id.request_hns_vpn_check_id);
            if (hnsVpnCheckedSubMenuItem != null) {
                hnsVpnCheckedSubMenuItem.setCheckable(true);
                hnsVpnCheckedSubMenuItem.setChecked(
                        HnsVpnProfileUtils.getInstance().isHnsVPNConnected(mContext));
            }
        } else {
            menu.findItem(R.id.request_hns_vpn_row_menu_id).setVisible(false);
        }

        // Hns's items are only visible for page menu.
        // To make logic simple, below three items are added whenever menu gets visible
        // and removed when menu is dismissed.

        if (!shouldShowPageMenu()) return;

        if (isMenuButtonInBottomToolbar()) {
            // Do not show icon row on top when menu itself is on bottom
            menu.findItem(R.id.icon_row_menu_id).setVisible(false).setEnabled(false);
        }

        // Hns donesn't show help menu item in app menu.
        menu.findItem(R.id.help_id).setVisible(false).setEnabled(false);

        // Always hide share row menu item in app menu if it's not on tablet.
        if (!mIsTablet) menu.findItem(R.id.share_row_menu_id).setVisible(false);

        MenuItem setAsDefault = menu.findItem(R.id.set_default_browser);
        if (shouldShowIconBeforeItem()) {
            setAsDefault.setIcon(
                    AppCompatResources.getDrawable(mContext, R.drawable.hns_menu_set_as_default));
        }

        HnsRewardsNativeWorker hnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
        if (hnsRewardsNativeWorker != null && hnsRewardsNativeWorker.IsSupported()
                && !HnsPrefServiceBridge.getInstance().getSafetynetCheckFailed()) {
            MenuItem rewards =
                    menu.add(Menu.NONE, R.id.hns_rewards_id, 0, R.string.menu_hns_rewards);
            if (shouldShowIconBeforeItem()) {
                rewards.setIcon(
                        AppCompatResources.getDrawable(mContext, R.drawable.hns_menu_rewards));
            }
        }
        MenuItem hnsWallet = menu.findItem(R.id.hns_wallet_id);
        if (hnsWallet != null) {
            if (ChromeFeatureList.isEnabled(HnsFeatureList.NATIVE_HNS_WALLET)) {
                hnsWallet.setVisible(true);
                if (shouldShowIconBeforeItem()) {
                    hnsWallet.setIcon(
                            AppCompatResources.getDrawable(mContext, R.drawable.ic_crypto_wallets));
                }
            } else {
                hnsWallet.setVisible(false);
            }
        }

        MenuItem hnsPlaylist = menu.findItem(R.id.hns_playlist_id);
        if (hnsPlaylist != null) {
            if (ChromeFeatureList.isEnabled(HnsFeatureList.HNS_PLAYLIST)
                    && SharedPreferencesManager.getInstance().readBoolean(
                            HnsPlaylistPreferences.PREF_ENABLE_PLAYLIST, true)) {
                hnsPlaylist.setVisible(true);
                if (shouldShowIconBeforeItem()) {
                    hnsPlaylist.setIcon(
                            AppCompatResources.getDrawable(mContext, R.drawable.ic_open_playlist));
                }
            } else {
                hnsPlaylist.setVisible(false);
            }
        }

        MenuItem hnsNews = menu.add(Menu.NONE, R.id.hns_news_id, 0, R.string.hns_news_title);
        if (shouldShowIconBeforeItem()) {
            hnsNews.setIcon(AppCompatResources.getDrawable(mContext, R.drawable.ic_news));
        }

        MenuItem hnsSpeedReader = menu.findItem(R.id.hns_speedreader_id);
        hnsSpeedReader.setVisible(false);
        if (ChromeFeatureList.isEnabled(HnsFeatureList.HNS_SPEEDREADER)
                && UserPrefs.get(mTabModelSelector.getCurrentModel().getProfile())
                           .getBoolean(HnsPref.SPEEDREADER_PREF_ENABLED)) {
            final Tab currentTab = mActivityTabProvider.get();
            if (currentTab != null && HnsSpeedReaderUtils.tabSupportsDistillation(currentTab)) {
                hnsSpeedReader.setVisible(true);
                if (shouldShowIconBeforeItem()) {
                    hnsSpeedReader.setIcon(
                            AppCompatResources.getDrawable(mContext, R.drawable.ic_readermode));
                }
            }
        }

        MenuItem exit = menu.add(Menu.NONE, R.id.exit_id, 0, R.string.menu_exit);
        if (shouldShowIconBeforeItem()) {
            exit.setIcon(AppCompatResources.getDrawable(mContext, R.drawable.hns_menu_exit));
        }

        if (HnsSetDefaultBrowserUtils.isHnsSetAsDefaultBrowser(mContext)) {
            menu.findItem(R.id.set_default_browser).setVisible(false);
        }

        // Replace info item with share
        MenuItem shareItem = menu.findItem(R.id.info_menu_id);
        if (shareItem != null) {
            shareItem.setTitle(mContext.getString(R.string.share));
            shareItem.setIcon(AppCompatResources.getDrawable(mContext, R.drawable.share_icon));
        }

        // By this we forcibly initialize BookmarkBridge
        MenuItem bookmarkItem = menu.findItem(R.id.bookmark_this_page_id);
        Tab currentTab = mActivityTabProvider.get();
        if (bookmarkItem != null && currentTab != null) {
            updateBookmarkMenuItemShortcut(bookmarkItem, currentTab, /*fromCCT=*/false);
        }

        // Remove unused dividers. This needs to be done after the visibility of all the items is
        // set.
        boolean hasItemBetweenDividers = false;
        for (int i = 0; i < menu.size(); ++i) {
            MenuItem item = menu.getItem(i);
            if (item.getItemId() == R.id.divider_line_id) {
                if (!hasItemBetweenDividers) {
                    // If there isn't any visible menu items between the two divider lines, mark
                    // this line invisible.
                    item.setVisible(false);
                } else {
                    hasItemBetweenDividers = false;
                }
            } else if (!hasItemBetweenDividers && item.isVisible()) {
                // When the item isn't a divider line and is visible, we set hasItemBetweenDividers
                // to be true.
                hasItemBetweenDividers = true;
            }
        }
    }

    @Override
    public void onMenuDismissed() {
        super.onMenuDismissed();

        mMenu.removeItem(R.id.set_default_browser);
        mMenu.removeItem(R.id.hns_rewards_id);
        mMenu.removeItem(R.id.hns_wallet_id);
        mMenu.removeItem(R.id.hns_playlist_id);
        mMenu.removeItem(R.id.hns_speedreader_id);
        mMenu.removeItem(R.id.exit_id);
        mMenu.removeItem(R.id.request_hns_vpn_row_menu_id);
    }

    @Override
    public void onFooterViewInflated(AppMenuHandler appMenuHandler, View view) {
        // If it's still null, just hide the whole view
        if (mBookmarkModelSupplier.get() == null) {
            if (view != null) {
                view.setVisibility(View.GONE);
            }
            // Normally it should not happen
            assert false;
            return;
        }
        super.onFooterViewInflated(appMenuHandler, view);

        if (view instanceof AppMenuIconRowFooter) {
            ((AppMenuIconRowFooter) view)
                    .initialize(appMenuHandler, mBookmarkModelSupplier.get(),
                            mActivityTabProvider.get(), mAppMenuDelegate);
        }

        // Hide bookmark button if bottom toolbar is enabled
        ImageButton bookmarkButton = view.findViewById(R.id.bookmark_this_page_id);
        if (bookmarkButton != null && BottomToolbarConfiguration.isBottomToolbarEnabled()) {
            bookmarkButton.setVisibility(View.GONE);
        }

        // Replace info button with share
        ImageButton shareButton = view.findViewById(R.id.info_menu_id);
        if (shareButton != null) {
            shareButton.setImageDrawable(
                    AppCompatResources.getDrawable(mContext, R.drawable.share_icon));
            shareButton.setContentDescription(mContext.getString(R.string.share));
        }
    }

    @Override
    public boolean shouldShowHeader(int maxMenuHeight) {
        if (isMenuButtonInBottomToolbar()) return false;
        return super.shouldShowHeader(maxMenuHeight);
    }

    @Override
    public boolean shouldShowFooter(int maxMenuHeight) {
        if (isMenuButtonInBottomToolbar()) return true;
        return super.shouldShowFooter(maxMenuHeight);
    }

    @Override
    public int getFooterResourceId() {
        if (isMenuButtonInBottomToolbar()) {
            return shouldShowPageMenu() ? R.layout.icon_row_menu_footer : 0;
        }
        return super.getFooterResourceId();
    }

    private boolean isMenuButtonInBottomToolbar() {
        return HnsMenuButtonCoordinator.isMenuFromBottom();
    }
}
