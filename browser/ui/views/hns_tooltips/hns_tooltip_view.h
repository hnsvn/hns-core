/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_TOOLTIPS_HNS_TOOLTIP_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_TOOLTIPS_HNS_TOOLTIP_VIEW_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "hns/browser/ui/hns_tooltips/hns_tooltip.h"
#include "hns/browser/ui/views/hns_tooltips/hns_tooltip_label_button.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
class Point;
}  // namespace gfx

namespace views {
class ImageView;
class Label;
class LabelButton;
class View;
}  // namespace views

namespace hns_tooltips {

class HnsTooltipPopup;

class HnsTooltipView : public views::View {
 public:
  METADATA_HEADER(HnsTooltipView);

  HnsTooltipView(HnsTooltipPopup* tooltip_popup,
                   const HnsTooltipAttributes& tooltip_attributes);
  ~HnsTooltipView() override;

  HnsTooltipView(const HnsTooltipView&) = delete;
  HnsTooltipView& operator=(const HnsTooltipView&) = delete;

  views::Button* ok_button_for_testing() const { return ok_button_; }
  views::Button* cancel_button_for_testing() const { return cancel_button_; }

  // views::InkDropHostView:
  void GetAccessibleNodeData(ui::AXNodeData* node_data) override;
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnDeviceScaleFactorChanged(float old_device_scale_factor,
                                  float new_device_scale_factor) override;
  void OnThemeChanged() override;

 private:
  void CreateView();

  void Close();

  views::View* CreateHeaderView();

  views::ImageView* CreateIconView();

  views::Label* CreateTitleLabel();

  views::View* CreateButtonView();

  HnsTooltipLabelButton* CreateOkButton();
  void OnOkButtonPressed();

  HnsTooltipLabelButton* CreateCancelButton();
  void OnCancelButtonPressed();

  views::View* CreateBodyView();
  views::Label* CreateBodyLabel();

  void UpdateTitleLabelColors();
  void UpdateBodyLabelColors();
  void UpdateOkButtonColors();
  void UpdateCancelButtonColors();

  raw_ptr<HnsTooltipPopup> tooltip_popup_;
  HnsTooltipAttributes tooltip_attributes_;

  gfx::Point initial_mouse_pressed_location_;
  bool is_dragging_ = false;

  bool is_closing_ = false;

  raw_ptr<views::Label> title_label_ = nullptr;
  raw_ptr<views::Label> body_label_ = nullptr;

  raw_ptr<views::LabelButton> ok_button_ = nullptr;
  raw_ptr<views::LabelButton> cancel_button_ = nullptr;

  std::u16string accessible_name_;
};

}  // namespace hns_tooltips

#endif  // HNS_BROWSER_UI_VIEWS_HNS_TOOLTIPS_HNS_TOOLTIP_VIEW_H_
