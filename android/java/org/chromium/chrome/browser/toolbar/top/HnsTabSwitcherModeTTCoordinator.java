/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.top;

import android.view.ViewStub;

import org.chromium.chrome.browser.toolbar.menu_button.MenuButtonCoordinator;

import java.util.function.BooleanSupplier;

class HnsTabSwitcherModeTTCoordinator extends TabSwitcherModeTTCoordinator {
    private TabSwitcherModeTopToolbar mActiveTabSwitcherToolbar;

    private boolean mIsBottomToolbarVisible;
    private MenuButtonCoordinator mHnsMenuButtonCoordinator;

    HnsTabSwitcherModeTTCoordinator(ViewStub tabSwitcherToolbarStub,
            MenuButtonCoordinator menuButtonCoordinator, boolean isTabToGtsAnimationEnabled,
            BooleanSupplier isIncognitoModeEnabledSupplier,
            ToolbarColorObserverManager toolbarColorObserverManager) {
        super(tabSwitcherToolbarStub, menuButtonCoordinator, isTabToGtsAnimationEnabled,
                isIncognitoModeEnabledSupplier, toolbarColorObserverManager);

        mHnsMenuButtonCoordinator = menuButtonCoordinator;
    }

    @Override
    public void setTabSwitcherMode(boolean inTabSwitcherMode) {
        super.setTabSwitcherMode(inTabSwitcherMode);
        if (inTabSwitcherMode
                && (mActiveTabSwitcherToolbar instanceof HnsTabSwitcherModeTopToolbar)) {
            ((HnsTabSwitcherModeTopToolbar) mActiveTabSwitcherToolbar)
                    .onBottomToolbarVisibilityChanged(mIsBottomToolbarVisible);
        }
        if (mHnsMenuButtonCoordinator != null && mIsBottomToolbarVisible) {
            mHnsMenuButtonCoordinator.setVisibility(!inTabSwitcherMode);
        }
    }

    void onBottomToolbarVisibilityChanged(boolean isVisible) {
        if (mIsBottomToolbarVisible == isVisible) {
            return;
        }
        mIsBottomToolbarVisible = isVisible;
        if (mActiveTabSwitcherToolbar instanceof HnsTabSwitcherModeTopToolbar) {
            ((HnsTabSwitcherModeTopToolbar) mActiveTabSwitcherToolbar)
                    .onBottomToolbarVisibilityChanged(isVisible);
        }
    }
}
