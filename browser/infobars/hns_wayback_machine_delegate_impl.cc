/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/infobars/hns_wayback_machine_delegate_impl.h"

#include "base/command_line.h"
#include "hns/components/hns_wayback_machine/hns_wayback_machine_infobar_delegate.h"
#include "hns/components/hns_wayback_machine/hns_wayback_machine_tab_helper.h"
#include "hns/components/constants/hns_switches.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/infobar.h"

// static
void HnsWaybackMachineDelegateImpl::AttachTabHelperIfNeeded(
    content::WebContents* web_contents) {
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableHnsWaybackMachineExtension)) {
    HnsWaybackMachineTabHelper::CreateForWebContents(web_contents);
    auto* tab_helper =
        HnsWaybackMachineTabHelper::FromWebContents(web_contents);
    tab_helper->set_delegate(
        std::make_unique<HnsWaybackMachineDelegateImpl>());
  }
}

HnsWaybackMachineDelegateImpl::HnsWaybackMachineDelegateImpl() = default;
HnsWaybackMachineDelegateImpl::~HnsWaybackMachineDelegateImpl() = default;


void HnsWaybackMachineDelegateImpl::CreateInfoBar(
    content::WebContents* web_contents) {
  infobars::ContentInfoBarManager::FromWebContents(web_contents)
      ->AddInfoBar(CreateInfoBarView(
                       std::make_unique<HnsWaybackMachineInfoBarDelegate>(),
                       web_contents),
                   true);
}
