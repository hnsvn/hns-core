/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/tabs/hns_tab_context_menu_contents.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include "hns/browser/ui/browser_commands.h"
#include "hns/browser/ui/tabs/hns_tab_menu_model.h"
#include "hns/browser/ui/tabs/hns_tab_prefs.h"
#include "hns/browser/ui/tabs/hns_tab_strip_model.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/tabs/hns_browser_tab_strip_controller.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/defaults.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sessions/tab_restore_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/tabs/tab_enums.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/tabs/tab_utils.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "components/sessions/core/tab_restore_service.h"
#include "content/public/browser/web_contents.h"
#include "ui/views/controls/menu/menu_runner.h"

HnsTabContextMenuContents::HnsTabContextMenuContents(
    Tab* tab,
    HnsBrowserTabStripController* controller,
    int index)
    : tab_(tab),
      tab_index_(index),
      browser_(const_cast<Browser*>(controller->browser())),
      controller_(controller) {
  const bool is_vertical_tab =
      base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs) &&
      tabs::utils::ShouldShowVerticalTabs(browser_);

  model_ = std::make_unique<HnsTabMenuModel>(
      this, controller->browser()->tab_menu_model_delegate(),
      controller->model(), index, is_vertical_tab);
  restore_service_ =
      TabRestoreServiceFactory::GetForProfile(browser_->profile());
  menu_runner_ = std::make_unique<views::MenuRunner>(
      model_.get(),
      views::MenuRunner::HAS_MNEMONICS | views::MenuRunner::CONTEXT_MENU);
}

HnsTabContextMenuContents::~HnsTabContextMenuContents() = default;

void HnsTabContextMenuContents::Cancel() {
  controller_ = nullptr;
}

void HnsTabContextMenuContents::RunMenuAt(const gfx::Point& point,
                                            ui::MenuSourceType source_type) {
  menu_runner_->RunMenuAt(tab_->GetWidget(), nullptr,
                          gfx::Rect(point, gfx::Size()),
                          views::MenuAnchorPosition::kTopLeft, source_type);
}

bool HnsTabContextMenuContents::IsCommandIdChecked(int command_id) const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    return ui::SimpleMenuModel::Delegate::IsCommandIdChecked(command_id);
  }

  if (command_id == HnsTabMenuModel::CommandShowVerticalTabs) {
    return tabs::utils::ShouldShowVerticalTabs(browser_);
  }

  return ui::SimpleMenuModel::Delegate::IsCommandIdChecked(command_id);
}

bool HnsTabContextMenuContents::IsCommandIdEnabled(int command_id) const {
  if (IsHnsCommandId(command_id))
    return IsHnsCommandIdEnabled(command_id);

  return controller_->IsCommandEnabledForTab(
      static_cast<TabStripModel::ContextMenuCommand>(command_id), tab_);
}

bool HnsTabContextMenuContents::IsCommandIdVisible(int command_id) const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    return ui::SimpleMenuModel::Delegate::IsCommandIdVisible(command_id);
  }

  if (command_id == HnsTabMenuModel::CommandShowVerticalTabs) {
    return tabs::utils::SupportsVerticalTabs(browser_);
  }

  return ui::SimpleMenuModel::Delegate::IsCommandIdVisible(command_id);
}

bool HnsTabContextMenuContents::GetAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator) const {
  if (IsHnsCommandId(command_id))
    return false;

  int browser_cmd;
  views::Widget* widget =
      BrowserView::GetBrowserViewForBrowser(browser_)->GetWidget();
  return TabStripModel::ContextMenuCommandToBrowserCommand(command_id,
                                                           &browser_cmd) &&
         widget->GetAccelerator(browser_cmd, accelerator);
}

void HnsTabContextMenuContents::ExecuteCommand(int command_id,
                                                 int event_flags) {
  if (IsHnsCommandId(command_id))
    return ExecuteHnsCommand(command_id);

  // Executing the command destroys |this|, and can also end up destroying
  // |controller_|. So stop the highlights before executing the command.
  controller_->ExecuteCommandForTab(
      static_cast<TabStripModel::ContextMenuCommand>(command_id), tab_);
}

bool HnsTabContextMenuContents::IsHnsCommandIdEnabled(
    int command_id) const {
  switch (command_id) {
    case HnsTabMenuModel::CommandRestoreTab:
      return restore_service_ && (!restore_service_->IsLoaded() ||
                                  !restore_service_->entries().empty());
    case HnsTabMenuModel::CommandBookmarkAllTabs:
      if (browser_) {
        return browser_defaults::bookmarks_enabled &&
               chrome::CanBookmarkAllTabs(browser_);
      }
      break;
    case HnsTabMenuModel::CommandToggleTabMuted: {
      auto* model = static_cast<HnsTabStripModel*>(controller_->model());
      for (const auto& index : model->GetTabIndicesForCommandAt(tab_index_)) {
        if (!model->GetWebContentsAt(index)->GetLastCommittedURL().is_empty())
          return true;
      }
      return false;
    }
    case HnsTabMenuModel::CommandShowVerticalTabs:
      return true;
    default:
      NOTREACHED();
      break;
  }

  return false;
}

void HnsTabContextMenuContents::ExecuteHnsCommand(int command_id) {
  switch (command_id) {
    case HnsTabMenuModel::CommandRestoreTab:
      chrome::RestoreTab(browser_);
      return;
    case HnsTabMenuModel::CommandBookmarkAllTabs:
      chrome::BookmarkAllTabs(browser_);
      return;
    case HnsTabMenuModel::CommandShowVerticalTabs: {
      hns::ToggleVerticalTabStrip(browser_);
      BrowserView::GetBrowserViewForBrowser(browser_)->InvalidateLayout();
      return;
    }
    case HnsTabMenuModel::CommandToggleTabMuted: {
      auto* model = static_cast<HnsTabStripModel*>(controller_->model());
      auto indices = model->GetTabIndicesForCommandAt(tab_index_);
      std::vector<content::WebContents*> contentses;
      std::transform(
          indices.begin(), indices.end(), std::back_inserter(contentses),
          [&model](int index) { return model->GetWebContentsAt(index); });

      auto all_muted = model_->all_muted();
      for (auto* contents : contentses) {
        chrome::SetTabAudioMuted(contents, !all_muted,
                                 TabMutedReason::AUDIO_INDICATOR,
                                 /*extension_id=*/std::string());
      }
      return;
    }
    default:
      NOTREACHED();
      return;
  }
}

bool HnsTabContextMenuContents::IsHnsCommandId(int command_id) const {
  return command_id > HnsTabMenuModel::CommandStart &&
         command_id < HnsTabMenuModel::CommandLast;
}
