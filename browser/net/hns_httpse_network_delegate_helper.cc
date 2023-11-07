/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/hns_httpse_network_delegate_helper.h"

#include <algorithm>
#include <memory>
#include <string>

#include "base/threading/scoped_blocking_call.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/hns_shields/hns_shields_web_contents_observer.h"
#include "hns/components/hns_shields/browser/https_everywhere_service.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/net_errors.h"

using hns_shields::HnsShieldsWebContentsObserver;
using content::BrowserThread;

namespace hns {

void OnBeforeURLRequest_HttpseFileWork(
    base::WeakPtr<hns_shields::HTTPSEverywhereService::Engine> engine,
    std::shared_ptr<HnsRequestInfo> ctx) {
  base::ScopedBlockingCall scoped_blocking_call(FROM_HERE,
                                                base::BlockingType::WILL_BLOCK);
  DCHECK_NE(ctx->request_identifier, 0U);
  if (engine)
    engine->GetHTTPSURL(&ctx->request_url, ctx->request_identifier,
                        &ctx->new_url_spec);
}

void OnBeforeURLRequest_HttpsePostFileWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (!ctx->new_url_spec.empty() &&
    ctx->new_url_spec != ctx->request_url.spec()) {
    hns_shields::HnsShieldsWebContentsObserver::DispatchBlockedEvent(
        ctx->request_url, ctx->frame_tree_node_id,
        hns_shields::kHTTPUpgradableResources);
  }

  next_callback.Run();
}

int OnBeforeURLRequest_HttpsePreFileWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  // Don't try to overwrite an already set URL by another delegate (adblock/tp)
  if (!ctx->new_url_spec.empty() || !ctx->mock_data_url.empty()) {
    return net::OK;
  }

  if (ctx->tab_origin.is_empty() || ctx->allow_http_upgradable_resource ||
      !ctx->allow_hns_shields) {
    return net::OK;
  }

  bool is_valid_url = true;
  is_valid_url = ctx->request_url.is_valid();
  std::string scheme = ctx->request_url.scheme();
  if (scheme.length()) {
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), ::tolower);
    if ("http" != scheme && "https" != scheme) {
      is_valid_url = false;
    }
  }

  if (is_valid_url) {
    if (!g_hns_browser_process->https_everywhere_service()
             ->GetHTTPSURLFromCacheOnly(&ctx->request_url,
                                        ctx->request_identifier,
                                        &ctx->new_url_spec)) {
      g_hns_browser_process->https_everywhere_service()
          ->GetTaskRunner()
          ->PostTaskAndReply(
              FROM_HERE,
              base::BindOnce(
                  OnBeforeURLRequest_HttpseFileWork,
                  g_hns_browser_process->https_everywhere_service()->engine(),
                  ctx),
              base::BindOnce(
                  base::IgnoreResult(&OnBeforeURLRequest_HttpsePostFileWork),
                  next_callback, ctx));
      return net::ERR_IO_PENDING;
    } else {
      if (!ctx->new_url_spec.empty()) {
        hns_shields::HnsShieldsWebContentsObserver::DispatchBlockedEvent(
            ctx->request_url, ctx->frame_tree_node_id,
            hns_shields::kHTTPUpgradableResources);
      }
    }
  }

  return net::OK;
}

}  // namespace hns
