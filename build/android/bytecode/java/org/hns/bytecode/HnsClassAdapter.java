/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsClassAdapter {
    public static ClassVisitor createAdapter(ClassVisitor chain) {
        chain = new HnsActivityClassAdapter(chain);
        chain = new HnsAppHooksClassAdapter(chain);
        chain = new HnsAppMenuClassAdapter(chain);
        chain = new HnsApplicationImplBaseClassAdapter(chain);
        chain = new HnsAutocompleteCoordinatorClassAdapter(chain);
        chain = new HnsAutocompleteMediatorClassAdapter(chain);
        chain = new HnsAutofillPopupBridgeClassAdapter(chain);
        chain = new HnsBookmarkActivityClassAdapter(chain);
        chain = new HnsBookmarkBridgeClassAdapter(chain);
        chain = new HnsBookmarkDelegateClassAdapter(chain);
        chain = new HnsBookmarkManagerCoordinatorClassAdapter(chain);
        chain = new HnsBookmarkManagerMediatorClassAdapter(chain);
        chain = new HnsBookmarkModelClassAdapter(chain);
        chain = new HnsBookmarkPageClassAdapter(chain);
        chain = new HnsBookmarkToolbarClassAdapter(chain);
        chain = new HnsBookmarkToolbarCoordinatorClassAdapter(chain);
        chain = new HnsBookmarkUtilsClassAdapter(chain);
        chain = new HnsBottomControlsCoordinatorClassAdapter(chain);
        chain = new HnsBottomControlsMediatorClassAdapter(chain);
        chain = new HnsCachedFlagClassAdapter(chain);
        chain = new HnsChromeContextMenuPopulatorAdapter(chain);
        chain = new HnsCommandLineInitUtilClassAdapter(chain);
        chain = new HnsContentSettingsResourcesClassAdapter(chain);
        chain = new HnsCustomizationProviderDelegateImplClassAdapter(chain);
        chain = new HnsDefaultBrowserPromoUtilsClassAdapter(chain);
        chain = new HnsDownloadMessageUiControllerImplClassAdapter(chain);
        chain = new HnsDropdownItemViewInfoListBuilderClassAdapter(chain);
        chain = new HnsDropdownItemViewInfoListManagerClassAdapter(chain);
        chain = new HnsDynamicColorsClassAdapter(chain);
        chain = new HnsExternalNavigationHandlerClassAdapter(chain);
        chain = new HnsFeedSurfaceCoordinatorClassAdapter(chain);
        chain = new HnsFeedSurfaceMediatorClassAdapter(chain);
        chain = new HnsFourStateCookieSettingsPreferenceBaseClassAdapter(chain);
        chain = new HnsFreIntentCreatorClassAdapter(chain);
        chain = new HnsHelpAndFeedbackLauncherImplClassAdapter(chain);
        chain = new HnsHomepageManagerClassAdapter(chain);
        chain = new HnsIncognitoToggleTabLayoutClassAdapter(chain);
        chain = new HnsIntentHandlerClassAdapter(chain);
        chain = new HnsLauncherActivityClassAdapter(chain);
        chain = new HnsLaunchIntentDispatcherClassAdapter(chain);
        chain = new HnsLocationBarCoordinatorClassAdapter(chain);
        chain = new HnsLocationBarLayoutClassAdapter(chain);
        chain = new HnsLocationBarMediatorClassAdapter(chain);
        chain = new HnsLogoMediatorClassAdapter(chain);
        chain = new HnsMainPreferenceBaseClassAdapter(chain);
        chain = new HnsManageAccountDevicesLinkViewClassAdapter(chain);
        chain = new HnsManageSyncSettingsClassAdapter(chain);
        chain = new HnsTranslateCompactInfoBarBaseClassAdapter(chain);
        chain = new HnsMenuButtonCoordinatorClassAdapter(chain);
        chain = new HnsMimeUtilsClassAdapter(chain);
        chain = new HnsMostVisitedTilesMediatorClassAdapter(chain);
        chain = new HnsNewTabPageClassAdapter(chain);
        chain = new HnsNewTabPageLayoutClassAdapter(chain);
        chain = new HnsNotificationBuilderClassAdapter(chain);
        chain = new HnsNotificationManagerProxyImplClassAdapter(chain);
        chain = new HnsNotificationPermissionRationaleDialogControllerClassAdapter(chain);
        chain = new HnsPasswordSettingsBaseClassAdapter(chain);
        chain = new HnsPermissionDialogDelegateClassAdapter(chain);
        chain = new HnsPermissionDialogModelClassAdapter(chain);
        chain = new HnsPreferenceFragmentClassAdapter(chain);
        chain = new HnsPreferenceKeyCheckerClassAdapter(chain);
        chain = new HnsPrivateTabReauthCoordinatorBaseClassAdapter(chain);
        chain = new HnsPureJavaExceptionReporterClassAdapter(chain);
        chain = new HnsQueryTileSectionClassAdapter(chain);
        chain = new HnsReaderModeManagerClassAdapter(chain);
        chain = new HnsReturnToChromeUtilClassAdapter(chain);
        chain = new HnsSearchEngineAdapterClassAdapter(chain);
        chain = new HnsSearchEnginePreferenceClassAdapter(chain);
        chain = new HnsSettingsLauncherImplClassAdapter(chain);
        chain = new HnsShareDelegateImplClassAdapter(chain);
        chain = new HnsSingleCategorySettingsClassAdapter(chain);
        chain = new HnsSingleWebsiteSettingsClassAdapter(chain);
        chain = new HnsSiteSettingsCategoryClassAdapter(chain);
        chain = new HnsSiteSettingsDelegateClassAdapter(chain);
        chain = new HnsSiteSettingsPreferencesBaseClassAdapter(chain);
        chain = new HnsStartupPaintPreviewHelperClassAdapter(chain);
        chain = new HnsStatusMediatorClassAdapter(chain);
        chain = new HnsTabGroupUiCoordinatorClassAdapter(chain);
        chain = new HnsTabHelpersClassAdapter(chain);
        chain = new HnsTabSwitcherModeTTCoordinatorClassAdapter(chain);
        chain = new HnsTabSwitcherModeTopToolbarClassAdapter(chain);
        chain = new HnsTabUiThemeProviderClassAdapter(chain);
        chain = new HnsTabbedActivityClassAdapter(chain);
        chain = new HnsTabbedRootUiCoordinatorClassAdapter(chain);
        chain = new HnsTabGroupModelFilterClassAdapter(chain);
        chain = new HnsThemeUtilsClassAdapter(chain);
        chain = new HnsTileViewClassAdapter(chain);
        chain = new HnsToolbarLayoutClassAdapter(chain);
        chain = new HnsToolbarManagerClassAdapter(chain);
        chain = new HnsTopToolbarCoordinatorClassAdapter(chain);
        chain = new HnsVariationsSeedFetcherClassAdapter(chain);
        chain = new HnsWebsiteClassAdapter(chain);
        chain = new HnsWebsitePermissionsFetcherClassAdapter(chain);
        return chain;
    }
}
