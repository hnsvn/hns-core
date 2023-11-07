/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_BROWSER_REWARDS_PROTOCOL_HANDLER_H_
#define HNS_COMPONENTS_HNS_REWARDS_BROWSER_REWARDS_PROTOCOL_HANDLER_H_

#include "content/public/browser/web_contents.h"
#include "ui/base/page_transition_types.h"
#include "url/gurl.h"

namespace hns_rewards {

bool IsRewardsProtocol(const GURL& url);

void HandleRewardsProtocol(const GURL& url,
                           content::WebContents::OnceGetter web_contents_getter,
                           ui::PageTransition page_transition);

}  // namespace hns_rewards

#endif  // HNS_COMPONENTS_HNS_REWARDS_BROWSER_REWARDS_PROTOCOL_HANDLER_H_
