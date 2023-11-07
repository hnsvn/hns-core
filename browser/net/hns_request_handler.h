/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NET_HNS_REQUEST_HANDLER_H_
#define HNS_BROWSER_NET_HNS_REQUEST_HANDLER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "hns/browser/net/url_context.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/completion_once_callback.h"

class PrefChangeRegistrar;

// Contains different network stack hooks (similar to capabilities of WebRequest
// API).
class HnsRequestHandler {
 public:
  HnsRequestHandler();
  HnsRequestHandler(const HnsRequestHandler&) = delete;
  HnsRequestHandler& operator=(const HnsRequestHandler&) = delete;
  ~HnsRequestHandler();

  bool IsRequestIdentifierValid(uint64_t request_identifier);

  int OnBeforeURLRequest(std::shared_ptr<hns::HnsRequestInfo> ctx,
                         net::CompletionOnceCallback callback,
                         GURL* new_url);

  int OnBeforeStartTransaction(std::shared_ptr<hns::HnsRequestInfo> ctx,
                               net::CompletionOnceCallback callback,
                               net::HttpRequestHeaders* headers);
  int OnHeadersReceived(
      std::shared_ptr<hns::HnsRequestInfo> ctx,
      net::CompletionOnceCallback callback,
      const net::HttpResponseHeaders* original_response_headers,
      scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
      GURL* allowed_unsafe_redirect_url);

  void OnURLRequestDestroyed(std::shared_ptr<hns::HnsRequestInfo> ctx);
  void RunCallbackForRequestIdentifier(uint64_t request_identifier, int rv);

 private:
  void SetupCallbacks();
  void RunNextCallback(std::shared_ptr<hns::HnsRequestInfo> ctx);

  std::vector<hns::OnBeforeURLRequestCallback> before_url_request_callbacks_;
  std::vector<hns::OnBeforeStartTransactionCallback>
      before_start_transaction_callbacks_;
  std::vector<hns::OnHeadersReceivedCallback> headers_received_callbacks_;

  std::map<uint64_t, net::CompletionOnceCallback> callbacks_;

  base::WeakPtrFactory<HnsRequestHandler> weak_factory_{this};
};

#endif  // HNS_BROWSER_NET_HNS_REQUEST_HANDLER_H_
