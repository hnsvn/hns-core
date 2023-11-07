/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_SIDE_PANEL_HNS_SIDE_PANEL_RESIZE_WIDGET_H_
#define HNS_BROWSER_UI_VIEWS_SIDE_PANEL_HNS_SIDE_PANEL_RESIZE_WIDGET_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "base/scoped_multi_source_observation.h"
#include "ui/views/view_observer.h"
#include "ui/views/widget/widget_delegate.h"

class HnsSidePanel;
class HnsBrowserView;

namespace views {
class ResizeAreaDelegate;
class Widget;
}  // namespace views

// Transparent widget that includes resize area only on side panel.
// Need widget to get proper event on the webview of side panel.
// HnsSidePanel owns this widget.
class SidePanelResizeWidget : public views::ViewObserver,
                              public views::WidgetDelegate {
 public:
  SidePanelResizeWidget(HnsSidePanel* panel,
                        HnsBrowserView* browser_view,
                        views::ResizeAreaDelegate* resize_area_delegate);
  ~SidePanelResizeWidget() override;
  SidePanelResizeWidget(const SidePanelResizeWidget&) = delete;
  SidePanelResizeWidget& operator=(const SidePanelResizeWidget&) = delete;

  // views::ViewObserver overrides:
  void OnViewBoundsChanged(views::View* observed_view) override;
  void OnViewVisibilityChanged(views::View* observed_view,
                               views::View* starting_view) override;
  void OnViewIsDeleting(views::View* observed_view) override;

 private:
  raw_ptr<HnsSidePanel> panel_ = nullptr;
  std::unique_ptr<views::Widget> widget_;
  base::ScopedMultiSourceObservation<views::View, views::ViewObserver>
      observations_{this};
};

#endif  // HNS_BROWSER_UI_VIEWS_SIDE_PANEL_HNS_SIDE_PANEL_RESIZE_WIDGET_H_
