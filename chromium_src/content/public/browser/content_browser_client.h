/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_CONTENT_BROWSER_CLIENT_H_
#define HNS_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_CONTENT_BROWSER_CLIENT_H_

#include "third_party/blink/public/mojom/loader/referrer.mojom.h"

// Hns-specific: allows the embedder to modify the referrer string
// according to user preferences.
// Allow the embedder to determine the user-agent according to user preferences.
// Allow the embedder to control if access to privileged functions that could
// be used by fingerprinting by a shared worker is allowed.
#define HNS_CONTENT_BROWSER_CLIENT_H                                       \
  virtual void MaybeHideReferrer(                                            \
      BrowserContext* browser_context, const GURL& request_url,              \
      const GURL& document_url, blink::mojom::ReferrerPtr* referrer) {}      \
  virtual std::string GetEffectiveUserAgent(BrowserContext* browser_context, \
                                            const GURL& url);                \
  virtual bool AllowWorkerFingerprinting(const GURL& url,                    \
                                         BrowserContext* browser_context);   \
  virtual uint8_t WorkerGetHnsFarblingLevel(                               \
      const GURL& url, BrowserContext* browser_context);

#include "src/content/public/browser/content_browser_client.h"  // IWYU pragma: export

#undef HNS_CONTENT_BROWSER_CLIENT_H

#endif  // HNS_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_CONTENT_BROWSER_CLIENT_H_
