/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/omnibox/hns_omnibox_view_views.h"

#include <utility>

#include "hns/app/hns_command_ids.h"
#include "hns/browser/hns_browser_features.h"
#include "hns/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "chrome/grit/generated_resources.h"
#include "components/omnibox/browser/omnibox_edit_model.h"

namespace {
void HnsUpdateContextMenu(ui::SimpleMenuModel* menu_contents, GURL url) {
  if (!url.SchemeIsHTTPOrHTTPS())
    return;
  absl::optional<size_t> copy_position =
      menu_contents->GetIndexOfCommandId(views::Textfield::kCopy);
  if (!copy_position)
    return;
  menu_contents->InsertItemWithStringIdAt(
      copy_position.value() + 1, IDC_COPY_CLEAN_LINK, IDS_COPY_CLEAN_LINK);
}
}  // namespace

HnsOmniboxViewViews::~HnsOmniboxViewViews() = default;

absl::optional<GURL> HnsOmniboxViewViews::GetURLToCopy() {
  GURL url;
  bool write_url = false;
  std::u16string selected_text = GetSelectedText();
  model()->AdjustTextForCopy(GetSelectedRange().GetMin(), &selected_text, &url,
                             &write_url);
  if (!write_url) {
    return absl::nullopt;
  }
  return url;
}

bool HnsOmniboxViewViews::SelectedTextIsURL() {
  return GetURLToCopy().has_value();
}

void HnsOmniboxViewViews::CleanAndCopySelectedURL() {
  auto url_to_copy = GetURLToCopy();
  if (!url_to_copy.has_value()) {
    return;
  }
  CopySanitizedURL(url_to_copy.value());
}

void HnsOmniboxViewViews::CopySanitizedURL(const GURL& url) {
  OnBeforePossibleChange();
  hns::CopySanitizedURL(chrome::FindLastActive(), url);
  OnAfterPossibleChange(true);
}

#if BUILDFLAG(IS_WIN)
bool HnsOmniboxViewViews::AcceleratorPressed(
    const ui::Accelerator& accelerator) {
  if (!base::FeatureList::IsEnabled(features::kHnsCopyCleanLinkByDefault)) {
    return OmniboxViewViews::AcceleratorPressed(accelerator);
  }

  ui::KeyEvent event(
      accelerator.key_state() == ui::Accelerator::KeyState::PRESSED
          ? ui::ET_KEY_PRESSED
          : ui::ET_KEY_RELEASED,
      accelerator.key_code(), accelerator.modifiers());
  auto command = GetCommandForKeyEvent(event);
  auto url_to_copy = GetURLToCopy();
  if ((GetTextInputType() != ui::TEXT_INPUT_TYPE_PASSWORD) &&
      (command != ui::TextEditCommand::COPY)) {
    return OmniboxViewViews::AcceleratorPressed(accelerator);
  }
  if (!url_to_copy.has_value()) {
    return OmniboxViewViews::AcceleratorPressed(accelerator);
  }
  CopySanitizedURL(url_to_copy.value());
  return true;
}
#endif  // BUILDFLAG(IS_WIN)

bool HnsOmniboxViewViews::GetAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator) const {
  if (base::FeatureList::IsEnabled(features::kHnsCopyCleanLinkByDefault)) {
    bool is_url = const_cast<HnsOmniboxViewViews*>(this)->SelectedTextIsURL();
    if (is_url) {
      if (command_id == kCopy) {
        return false;
      }
      if (command_id == IDC_COPY_CLEAN_LINK) {
        *accelerator = ui::Accelerator(ui::VKEY_C, ui::EF_PLATFORM_ACCELERATOR);
        return true;
      }
    }
  }
  return OmniboxViewViews::GetAcceleratorForCommandId(command_id, accelerator);
}

#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_MAC)
void HnsOmniboxViewViews::ExecuteTextEditCommand(
    ui::TextEditCommand command) {
  if (base::FeatureList::IsEnabled(features::kHnsCopyCleanLinkByDefault)) {
    auto url_to_copy = GetURLToCopy();
    if (command == ui::TextEditCommand::COPY && url_to_copy.has_value()) {
      CopySanitizedURL(url_to_copy.value());
      return;
    }
  }
  OmniboxViewViews::ExecuteTextEditCommand(command);
}
#endif  // BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_MAC)

void HnsOmniboxViewViews::UpdateContextMenu(
    ui::SimpleMenuModel* menu_contents) {
  OmniboxViewViews::UpdateContextMenu(menu_contents);
  auto url_to_copy = GetURLToCopy();
  if (url_to_copy.has_value()) {
    HnsUpdateContextMenu(menu_contents, url_to_copy.value());
  }
}
