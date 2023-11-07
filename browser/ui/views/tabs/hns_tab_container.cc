/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/tabs/hns_tab_container.h"

#include <algorithm>
#include <vector>

#include "base/check_is_test.h"
#include "hns/browser/ui/tabs/hns_tab_prefs.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/tabs/hns_tab_group_header.h"
#include "hns/browser/ui/views/tabs/hns_tab_strip.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/tabs/tab_style.h"
#include "chrome/browser/ui/ui_features.h"
#include "chrome/browser/ui/views/tabs/tab_drag_controller.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/gfx/canvas.h"
#include "ui/views/view_utils.h"

HnsTabContainer::HnsTabContainer(
    TabContainerController& controller,
    TabHoverCardController* hover_card_controller,
    TabDragContextBase* drag_context,
    TabSlotController& tab_slot_controller,
    views::View* scroll_contents_view)
    : TabContainerImpl(controller,
                       hover_card_controller,
                       drag_context,
                       tab_slot_controller,
                       scroll_contents_view),
      drag_context_(static_cast<TabDragContext*>(drag_context)),
      tab_style_(TabStyle::Get()) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return;

  auto* browser = tab_slot_controller_->GetBrowser();
  if (!browser) {
    CHECK_IS_TEST();
    return;
  }

  if (!tabs::utils::SupportsVerticalTabs(browser)) {
    return;
  }

  auto* prefs = browser->profile()->GetOriginalProfile()->GetPrefs();
  show_vertical_tabs_.Init(
      hns_tabs::kVerticalTabsEnabled, prefs,
      base::BindRepeating(&HnsTabContainer::UpdateLayoutOrientation,
                          base::Unretained(this)));
  vertical_tabs_floating_mode_enabled_.Init(
      hns_tabs::kVerticalTabsFloatingEnabled, prefs,
      base::BindRepeating(&HnsTabContainer::UpdateLayoutOrientation,
                          base::Unretained(this)));
  vertical_tabs_collapsed_.Init(
      hns_tabs::kVerticalTabsCollapsed, prefs,
      base::BindRepeating(&HnsTabContainer::UpdateLayoutOrientation,
                          base::Unretained(this)));

  UpdateLayoutOrientation();
}

HnsTabContainer::~HnsTabContainer() {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return;

  // When the last tab is closed and tab strip is being destructed, the
  // animation for the last removed tab could have been scheduled but not
  // finished yet. In this case, stop the animation before checking if all
  // closed tabs were cleaned up from OnTabCloseAnimationCompleted().
  CancelAnimation();
  DCHECK(closing_tabs_.empty()) << "There are dangling closed tabs.";
  DCHECK(!layout_locked_)
      << "The lock returned by LockLayout() shouldn't outlive this object";
}

base::OnceClosure HnsTabContainer::LockLayout() {
  DCHECK(base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
      << "This method should be called only when the flag is on.";

  DCHECK(!layout_locked_) << "LockLayout() doesn't allow reentrance";
  layout_locked_ = true;
  return base::BindOnce(&HnsTabContainer::OnUnlockLayout,
                        base::Unretained(this));
}

gfx::Size HnsTabContainer::CalculatePreferredSize() const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return TabContainerImpl::CalculatePreferredSize();

  // Note that we check this before checking currently we're in vertical tab
  // strip mode. We might be in the middle of changing orientation.
  if (layout_locked_)
    return {};

  if (!tabs::utils::ShouldShowVerticalTabs(
          tab_slot_controller_->GetBrowser())) {
    return TabContainerImpl::CalculatePreferredSize();
  }

  const int tab_count = tabs_view_model_.view_size();
  int height = 0;
  if (bounds_animator_.IsAnimating() && tab_count &&
      !drag_context_->GetDragController()->IsActive()) {
    // When removing a tab in the middle of tabs, the last tab's current bottom
    // could be greater than ideal bounds bottom.
    height = tabs_view_model_.view_at(tab_count - 1)->bounds().bottom();
  }

  if (!closing_tabs_.empty()) {
    // When closing trailing tabs, the last tab's current bottom could be
    // greater than ideal bounds bottom. Note that closing tabs are not in
    // tabs_view_model_ so we have to check again here.
    for (auto* tab : closing_tabs_)
      height = std::max(height, tab->bounds().bottom());
  }

  const auto slots_bounds = layout_helper_->CalculateIdealBounds(
      available_width_callback_.is_null() ||
              base::FeatureList::IsEnabled(features::kScrollableTabStrip)
          ? absl::nullopt
          : absl::optional<int>(available_width_callback_.Run()));
  height =
      std::max(height, slots_bounds.empty() ? 0 : slots_bounds.back().bottom());

  if (tab_count) {
    if (Tab* last_tab = tabs_view_model_.view_at(tab_count - 1);
        last_tab->group().has_value() &&
        !controller_->IsGroupCollapsed(*last_tab->group())) {
      height += HnsTabGroupHeader::kPaddingForGroup;
    }

    // Both containers for pinned tabs and unpinned tabs should have margin
    height += tabs::kMarginForVerticalTabContainers;
  }

  return gfx::Size(tab_style_->GetStandardWidth(), height);
}

void HnsTabContainer::UpdateClosingModeOnRemovedTab(int model_index,
                                                      bool was_active) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    TabContainerImpl::UpdateClosingModeOnRemovedTab(model_index, was_active);
    return;
  }

  // Don't shrink vertical tab strip's width
  if (tabs::utils::ShouldShowVerticalTabs(tab_slot_controller_->GetBrowser())) {
    return;
  }

  TabContainerImpl::UpdateClosingModeOnRemovedTab(model_index, was_active);
}

gfx::Rect HnsTabContainer::GetTargetBoundsForClosingTab(
    Tab* tab,
    int former_model_index) const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    return TabContainerImpl::GetTargetBoundsForClosingTab(tab,
                                                          former_model_index);
  }

  if (!tabs::utils::ShouldShowVerticalTabs(
          tab_slot_controller_->GetBrowser())) {
    return TabContainerImpl::GetTargetBoundsForClosingTab(tab,
                                                          former_model_index);
  }

  gfx::Rect target_bounds = tab->bounds();
  if (tab->data().pinned) {
    target_bounds.set_width(0);
  } else {
    target_bounds.set_y(
        (former_model_index > 0)
            ? tabs_view_model_.ideal_bounds(former_model_index - 1).bottom()
            : 0);
    target_bounds.set_height(0);
  }
  return target_bounds;
}

void HnsTabContainer::EnterTabClosingMode(absl::optional<int> override_width,
                                            CloseTabSource source) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    TabContainerImpl::EnterTabClosingMode(override_width, source);
    return;
  }

  // Don't shrink vertical tab strip's width
  if (tabs::utils::ShouldShowVerticalTabs(tab_slot_controller_->GetBrowser())) {
    return;
  }

  TabContainerImpl::EnterTabClosingMode(override_width, source);
}

bool HnsTabContainer::ShouldTabBeVisible(const Tab* tab) const {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
    return TabContainerImpl::ShouldTabBeVisible(tab);

  // We don't have to clip tabs out of bounds. Scroll view will handle it.
  if (tabs::utils::ShouldShowVerticalTabs(tab_slot_controller_->GetBrowser())) {
    return true;
  }

  return TabContainerImpl::ShouldTabBeVisible(tab);
}

void HnsTabContainer::StartInsertTabAnimation(int model_index) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    TabContainerImpl::StartInsertTabAnimation(model_index);
    return;
  }

  // Note that we check this before checking currently we're in vertical tab
  // strip mode. We might be in the middle of changing orientation.
  if (layout_locked_)
    return;

  if (!tabs::utils::ShouldShowVerticalTabs(
          tab_slot_controller_->GetBrowser())) {
    TabContainerImpl::StartInsertTabAnimation(model_index);
    return;
  }

  ExitTabClosingMode();

  auto* new_tab = GetTabAtModelIndex(model_index);
  gfx::Rect bounds = new_tab->bounds();
  bounds.set_height(tabs::kVerticalTabHeight);
  const auto tab_width = new_tab->data().pinned
                             ? tabs::kVerticalTabMinWidth
                             : tab_style_->GetStandardWidth();
  bounds.set_width(tab_width);
  bounds.set_x(-tab_width);
  bounds.set_y((model_index > 0)
                   ? tabs_view_model_.ideal_bounds(model_index - 1).bottom()
                   : 0);
  GetTabAtModelIndex(model_index)->SetBoundsRect(bounds);

  // Animate in to the full width.
  AnimateToIdealBounds();
}

void HnsTabContainer::RemoveTab(int index, bool was_active) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    TabContainerImpl::RemoveTab(index, was_active);
    return;
  }

  if (tabs::utils::ShouldShowVerticalTabs(tab_slot_controller_->GetBrowser())) {
    closing_tabs_.insert(tabs_view_model_.view_at(index));
  }

  TabContainerImpl::RemoveTab(index, was_active);
}

void HnsTabContainer::OnTabCloseAnimationCompleted(Tab* tab) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    TabContainerImpl::OnTabCloseAnimationCompleted(tab);
    return;
  }

  if (tabs::utils::ShouldShowVerticalTabs(tab_slot_controller_->GetBrowser())) {
    closing_tabs_.erase(tab);
  }

  TabContainerImpl::OnTabCloseAnimationCompleted(tab);

  // we might have to hide this container entirely
  if (!tabs_view_model_.view_size())
    PreferredSizeChanged();
}

void HnsTabContainer::UpdateLayoutOrientation() {
  DCHECK(base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
      << "This method should be called only when the flag is on.";

  layout_helper_->set_use_vertical_tabs(
      tabs::utils::ShouldShowVerticalTabs(tab_slot_controller_->GetBrowser()));
  layout_helper_->set_tab_strip(
      static_cast<HnsTabStrip*>(base::to_address(tab_slot_controller_)));
  InvalidateLayout();
}

void HnsTabContainer::OnUnlockLayout() {
  DCHECK(base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs))
      << "This method should be called only when the flag is on.";

  layout_locked_ = false;

  InvalidateIdealBounds();
  PreferredSizeChanged();
  CompleteAnimationAndLayout();
}

void HnsTabContainer::CompleteAnimationAndLayout() {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    TabContainerImpl::CompleteAnimationAndLayout();
    return;
  }

  // Note that we check this before checking currently we're in vertical tab
  // strip mode. We might be in the middle of changing orientation.
  if (layout_locked_)
    return;

  TabContainerImpl::CompleteAnimationAndLayout();
}

void HnsTabContainer::OnPaintBackground(gfx::Canvas* canvas) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    TabContainerImpl::OnPaintBackground(canvas);
    return;
  }

  if (!tabs::utils::ShouldShowVerticalTabs(
          tab_slot_controller_->GetBrowser())) {
    TabContainerImpl::OnPaintBackground(canvas);
    return;
  }

  canvas->DrawColor(GetColorProvider()->GetColor(kColorToolbar));
}

void HnsTabContainer::PaintChildren(const views::PaintInfo& paint_info) {
  if (!base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    TabContainerImpl::PaintChildren(paint_info);
    return;
  }

  // Exclude tabs that own layer.
  std::vector<ZOrderableTabContainerElement> orderable_children;
  for (views::View* child : children()) {
    if (!ZOrderableTabContainerElement::CanOrderView(child)) {
      continue;
    }
    if (child->layer()) {
      continue;
    }

    orderable_children.emplace_back(child);
  }

  std::stable_sort(orderable_children.begin(), orderable_children.end());

  for (const ZOrderableTabContainerElement& child : orderable_children) {
    child.view()->Paint(paint_info);
  }
}

BEGIN_METADATA(HnsTabContainer, TabContainerImpl)
END_METADATA
