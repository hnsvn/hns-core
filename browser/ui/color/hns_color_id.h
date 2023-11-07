/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_COLOR_HNS_COLOR_ID_H_
#define HNS_BROWSER_UI_COLOR_HNS_COLOR_ID_H_

#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "chrome/browser/ui/color/chrome_color_id.h"

// clang-format off

#define HNS_COMMON_COLOR_IDS                  \
    E_CPONLY(kColorForTest)                     \
    E_CPONLY(kColorIconBase)                    \
    E_CPONLY(kColorMenuItemSubText)             \
    E_CPONLY(kColorBookmarkBarInstructionsText) \
    E_CPONLY(kColorLocationBarFocusRing)        \
    E_CPONLY(kColorDialogDontAskAgainButton)    \
    E_CPONLY(kColorDialogDontAskAgainButtonHovered) \
    E_CPONLY(kColorWebDiscoveryInfoBarBackground)   \
    E_CPONLY(kColorWebDiscoveryInfoBarMessage)      \
    E_CPONLY(kColorWebDiscoveryInfoBarLink)         \
    E_CPONLY(kColorWebDiscoveryInfoBarNoThanks)     \
    E_CPONLY(kColorWebDiscoveryInfoBarClose)        \
    E_CPONLY(kColorHnsDownloadToolbarButtonActive)

#define HNS_SEARCH_CONVERSION_COLOR_IDS                             \
    E_CPONLY(kColorSearchConversionCloseButton)                       \
    E_CPONLY(kColorSearchConversionBannerTypeBackgroundBorder)        \
    E_CPONLY(kColorSearchConversionBannerTypeBackgroundBorderHovered) \
    E_CPONLY(kColorSearchConversionBannerTypeBackgroundGradientFrom)  \
    E_CPONLY(kColorSearchConversionBannerTypeBackgroundGradientTo)    \
    E_CPONLY(kColorSearchConversionBannerTypeDescText)                \
    E_CPONLY(kColorSearchConversionButtonTypeBackgroundNormal)        \
    E_CPONLY(kColorSearchConversionButtonTypeBackgroundHovered)       \
    E_CPONLY(kColorSearchConversionButtonTypeDescNormal)              \
    E_CPONLY(kColorSearchConversionButtonTypeDescHovered)             \
    E_CPONLY(kColorSearchConversionButtonTypeInputAppend)

#define HNS_SIDEBAR_COLOR_IDS                               \
    E_CPONLY(kColorSidebarAddBubbleBackground)                \
    E_CPONLY(kColorSidebarAddBubbleHeaderText)                \
    E_CPONLY(kColorSidebarAddBubbleItemTextBackgroundHovered) \
    E_CPONLY(kColorSidebarAddBubbleItemTextHovered)           \
    E_CPONLY(kColorSidebarAddBubbleItemTextNormal)            \
    E_CPONLY(kColorSidebarAddButtonDisabled)                  \
    E_CPONLY(kColorSidebarArrowBackgroundHovered)             \
    E_CPONLY(kColorSidebarArrowDisabled)                      \
    E_CPONLY(kColorSidebarArrowNormal)                        \
    E_CPONLY(kColorSidebarButtonBase)                         \
    E_CPONLY(kColorSidebarItemBackgroundHovered)              \
    E_CPONLY(kColorSidebarItemDragIndicator)                  \
    E_CPONLY(kColorSidebarSeparator)

#if BUILDFLAG(ENABLE_SPEEDREADER)
#define HNS_SPEEDREADER_COLOR_IDS      \
  E_CPONLY(kColorSpeedreaderIcon)        \
  E_CPONLY(kColorSpeedreaderToggleThumb) \
  E_CPONLY(kColorSpeedreaderToggleTrack) \
  E_CPONLY(kColorSpeedreaderToolbarBackground) \
  E_CPONLY(kColorSpeedreaderToolbarBorder) \
  E_CPONLY(kColorSpeedreaderToolbarForeground) \
  E_CPONLY(kColorSpeedreaderToolbarButtonHover) \
  E_CPONLY(kColorSpeedreaderToolbarButtonActive) \
  E_CPONLY(kColorSpeedreaderToolbarButtonBorder)
#else
#define HNS_SPEEDREADER_COLOR_IDS
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#define HNS_VPN_COLOR_IDS                        \
    E_CPONLY(kColorHnsVpnButtonErrorBorder)      \
    E_CPONLY(kColorHnsVpnButtonBorder)           \
    E_CPONLY(kColorHnsVpnButtonText)    \
    E_CPONLY(kColorHnsVpnButtonTextError)    \
    E_CPONLY(kColorHnsVpnButtonIconConnected)    \
    E_CPONLY(kColorHnsVpnButtonIconDisconnected) \
    E_CPONLY(kColorHnsVpnButtonIconError) \
    E_CPONLY(kColorHnsVpnButtonBackgroundNormal) \
    E_CPONLY(kColorHnsVpnButtonBackgroundHover)  \
    E_CPONLY(kColorHnsVpnButtonErrorBackgroundNormal) \
    E_CPONLY(kColorHnsVpnButtonErrorBackgroundHover)  \
    E_CPONLY(kColorHnsVpnButtonIconInner) \
    E_CPONLY(kColorHnsVpnButtonIconErrorInner)
#else
#define HNS_VPN_COLOR_IDS
#endif

// Unfortunately, we can't have a defined(TOOLKIT_VIEWS) guard here
// as hns_color_mixer depends on this without deps to //ui/views:flags.
// But it's safe have without the guard as this file is included only when
// !is_android.
#define HNS_VERTICAL_TAB_COLOR_IDS                    \
    E_CPONLY(kColorHnsVerticalTabSeparator)           \
    E_CPONLY(kColorHnsVerticalTabActiveBackground)    \
    E_CPONLY(kColorHnsVerticalTabInactiveBackground)  \
    E_CPONLY(kColorHnsVerticalTabHeaderButtonColor)   \
    E_CPONLY(kColorHnsVerticalTabNTBIconColor)        \
    E_CPONLY(kColorHnsVerticalTabNTBTextColor)        \
    E_CPONLY(kColorHnsVerticalTabNTBShortcutTextColor)

#define HNS_PLAYLIST_COLOR_IDS                                      \
    E_CPONLY(kColorHnsPlaylistAddedIcon)                            \
    E_CPONLY(kColorHnsPlaylistCheckedIcon)                          \
    E_CPONLY(kColorHnsPlaylistSelectedBackground)                   \
    E_CPONLY(kColorHnsPlaylistListBorder)                           \
    E_CPONLY(kColorHnsPlaylistMoveDialogDescription)                \
    E_CPONLY(kColorHnsPlaylistMoveDialogCreatePlaylistAndMoveTitle) \
    E_CPONLY(kColorHnsPlaylistNewPlaylistDialogNameLabel)           \
    E_CPONLY(kColorHnsPlaylistNewPlaylistDialogItemsLabel)

#define HNS_COLOR_IDS               \
    HNS_COMMON_COLOR_IDS            \
    HNS_SEARCH_CONVERSION_COLOR_IDS \
    HNS_SIDEBAR_COLOR_IDS           \
    HNS_SPEEDREADER_COLOR_IDS       \
    HNS_VPN_COLOR_IDS               \
    HNS_VERTICAL_TAB_COLOR_IDS      \
    HNS_PLAYLIST_COLOR_IDS

#include "ui/color/color_id_macros.inc"

enum HnsColorIds : ui::ColorId {
  kHnsColorsStart = kChromeColorsEnd,

  HNS_COLOR_IDS

  kHnsColorsEnd,
};

#include "ui/color/color_id_macros.inc"  // NOLINT

// clang-format on

#endif  // HNS_BROWSER_UI_COLOR_HNS_COLOR_ID_H_
