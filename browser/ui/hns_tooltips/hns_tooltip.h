/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_H_
#define HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_H_

#include <string>
#include <utility>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "hns/browser/ui/hns_tooltips/hns_tooltip_attributes.h"
#include "hns/browser/ui/hns_tooltips/hns_tooltip_delegate.h"

namespace hns_tooltips {

class HnsTooltip {
 public:
  // Create a new tooltip with an |id| and |attributes|.  |delegate|
  // will influence the behaviour of this tooltip and receives events
  // on its behalf. The delegate may be omitted
  HnsTooltip(const std::string& id,
               const HnsTooltipAttributes& attributes,
               base::WeakPtr<HnsTooltipDelegate> delegate);
  virtual ~HnsTooltip();

  HnsTooltip(const HnsTooltip&) = delete;
  HnsTooltip& operator=(const HnsTooltip&) = delete;

  const std::string& id() const { return id_; }

  const HnsTooltipAttributes& attributes() const { return attributes_; }
  void set_attributes(const HnsTooltipAttributes& attributes) {
    attributes_ = attributes;
  }

  std::u16string accessible_name() const;

  HnsTooltipDelegate* delegate() const { return delegate_.get(); }

  void set_delegate(base::WeakPtr<HnsTooltipDelegate> delegate) {
    DCHECK(!delegate_);
    delegate_ = std::move(delegate);
  }

  virtual void PerformOkButtonAction() {}
  virtual void PerformCancelButtonAction() {}

 protected:
  std::string id_;
  HnsTooltipAttributes attributes_;

 private:
  base::WeakPtr<HnsTooltipDelegate> delegate_;
};

}  // namespace hns_tooltips

#endif  // HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_H_
