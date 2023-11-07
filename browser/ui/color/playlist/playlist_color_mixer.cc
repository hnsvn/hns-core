/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/color/playlist/playlist_color_mixer.h"

#include "hns/browser/ui/color/hns_color_id.h"
#include "ui/color/color_mixer.h"
#include "ui/color/color_provider.h"
#include "ui/color/color_recipe.h"

namespace playlist {

void AddThemeColorMixer(ui::ColorProvider* provider,
                        leo::Theme theme,
                        const ui::ColorProviderKey& key) {
  ui::ColorMixer& mixer = provider->AddMixer();
  mixer[kColorHnsPlaylistAddedIcon] = {
      leo::GetColor(leo::Color::kColorSystemfeedbackSuccessIcon, theme)};
  mixer[kColorHnsPlaylistCheckedIcon] = {
      leo::GetColor(leo::Color::kColorIconInteractive, theme)};
  mixer[kColorHnsPlaylistSelectedBackground] = {
      leo::GetColor(leo::Color::kColorContainerInteractive, theme)};
  mixer[kColorHnsPlaylistListBorder] = {
      leo::GetColor(leo::Color::kColorDividerSubtle, theme)};
  mixer[kColorHnsPlaylistMoveDialogDescription] = {
      leo::GetColor(leo::Color::kColorTextSecondary, theme)};
  mixer[kColorHnsPlaylistMoveDialogCreatePlaylistAndMoveTitle] = {
      leo::GetColor(leo::Color::kColorTextPrimary, theme)};
  mixer[kColorHnsPlaylistNewPlaylistDialogNameLabel] = {
      leo::GetColor(leo::Color::kColorTextPrimary, theme)};
  mixer[kColorHnsPlaylistNewPlaylistDialogItemsLabel] = {
      leo::GetColor(leo::Color::kColorTextSecondary, theme)};
}

}  // namespace playlist
