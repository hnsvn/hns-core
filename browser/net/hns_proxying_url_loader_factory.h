/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NET_HNS_PROXYING_URL_LOADER_FACTORY_H_
#define HNS_BROWSER_NET_HNS_PROXYING_URL_LOADER_FACTORY_H_

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/unique_ptr_adapters.h"
#include "base/functional/callback.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/raw_ref.h"
#include "base/memory/ref_counted_delete_on_sequence.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "hns/browser/net/resource_context_data.h"
#include "hns/browser/net/url_context.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "net/base/completion_once_callback.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/mojom/early_hints.mojom-forward.h"
#include "services/network/public/mojom/network_context.mojom.h"
#include "services/network/public/mojom/url_loader.mojom.h"
#include "services/network/public/mojom/url_loader_factory.mojom.h"
#include "services/network/public/mojom/url_response_head.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"

namespace content {
class BrowserContext;
class RenderFrameHost;
}  // namespace content

// Cargoculted from WebRequestProxyingURLLoaderFactory and
// signin::ProxyingURLLoaderFactory
class HnsProxyingURLLoaderFactory
    : public network::mojom::URLLoaderFactory {
 public:
  using DisconnectCallback =
      base::OnceCallback<void(HnsProxyingURLLoaderFactory*)>;

  class InProgressRequest : public network::mojom::URLLoader,
                            public network::mojom::URLLoaderClient {
   public:
    InProgressRequest(
        HnsProxyingURLLoaderFactory& factory,
        uint64_t request_id,
        int32_t network_service_request_id,
        int render_process_id,
        int frame_tree_node_id,
        uint32_t options,
        const network::ResourceRequest& request,
        content::BrowserContext* browser_context,
        const net::MutableNetworkTrafficAnnotationTag& traffic_annotation,
        mojo::PendingReceiver<network::mojom::URLLoader> loader_receiver,
        mojo::PendingRemote<network::mojom::URLLoaderClient> client,
        scoped_refptr<base::SequencedTaskRunner>
            navigation_response_task_runner);
    InProgressRequest(const InProgressRequest&) = delete;
    InProgressRequest& operator=(const InProgressRequest&) = delete;
    ~InProgressRequest() override;

    void Restart();

    // network::mojom::URLLoader:
    void FollowRedirect(
        const std::vector<std::string>& removed_headers,
        const net::HttpRequestHeaders& modified_headers,
        const net::HttpRequestHeaders& modified_cors_exempt_headers,
        const absl::optional<GURL>& new_url) override;
    void SetPriority(net::RequestPriority priority,
                     int32_t intra_priority_value) override;
    void PauseReadingBodyFromNet() override;
    void ResumeReadingBodyFromNet() override;

    // network::mojom::URLLoaderClient:
    void OnReceiveEarlyHints(
        network::mojom::EarlyHintsPtr early_hints) override;
    void OnReceiveResponse(
        network::mojom::URLResponseHeadPtr response_head,
        mojo::ScopedDataPipeConsumerHandle body,
        absl::optional<mojo_base::BigBuffer> cached_metadata) override;
    void OnReceiveRedirect(
        const net::RedirectInfo& redirect_info,
        network::mojom::URLResponseHeadPtr response_head) override;
    void OnUploadProgress(int64_t current_position,
                          int64_t total_size,
                          OnUploadProgressCallback callback) override;
    void OnTransferSizeUpdated(int32_t transfer_size_diff) override;
    void OnComplete(const network::URLLoaderCompletionStatus& status) override;

   private:
    // These two methods combined form the implementation of Restart().
    void UpdateRequestInfo();
    void RestartInternal();

    void ContinueToBeforeSendHeaders(int error_code);
    void ContinueToSendHeaders(int error_code);
    void ContinueToStartRequest(int error_code);
    void ContinueToResponseStarted(int error_code);
    void ContinueToBeforeRedirect(const net::RedirectInfo& redirect_info,
                                  int error_code);
    void HandleResponseOrRedirectHeaders(
        net::CompletionOnceCallback continuation);
    void OnRequestError(const network::URLLoaderCompletionStatus& status);
    void HandleBeforeRequestRedirect();

    base::TimeTicks start_time_;

    // TODO(iefremov): Get rid of shared_ptr, we should clearly own the pointer.
    std::shared_ptr<hns::HnsRequestInfo> ctx_;
    const raw_ref<HnsProxyingURLLoaderFactory> factory_;
    network::ResourceRequest request_;
    const uint64_t request_id_;
    const int32_t network_service_request_id_;

    const int render_process_id_;
    const int frame_tree_node_id_;
    const uint32_t options_;

    raw_ptr<content::BrowserContext> browser_context_ = nullptr;
    const net::MutableNetworkTrafficAnnotationTag traffic_annotation_;

    // This is our proxy's receiver that will talk to the original client. It
    // will take over the passed in PendingReceiver.
    mojo::Receiver<network::mojom::URLLoader> proxied_loader_receiver_;
    // This is the original client.
    mojo::Remote<network::mojom::URLLoaderClient> target_client_;

    // This is our proxy's client that will talk to originally targeted loader.
    mojo::Receiver<network::mojom::URLLoaderClient> proxied_client_receiver_;
    // This is the original receiver the original client meant to talk to.
    mojo::Remote<network::mojom::URLLoader> target_loader_;

    // NOTE: This is state which ExtensionWebRequestEventRouter needs to have
    // persisted across some phases of this request -- namely between
    // |OnHeadersReceived()| and request completion or restart. Pointers to
    // these fields are stored in a |BlockedRequest| (created and owned by
    // ExtensionWebRequestEventRouter) through much of the request's lifetime.
    // That code supports both Network Service and non-Network Service behavior,
    // which is why this weirdness exists here.
    absl::optional<mojo_base::BigBuffer> cached_metadata_;
    network::mojom::URLResponseHeadPtr current_response_head_;
    mojo::ScopedDataPipeConsumerHandle current_response_body_;
    scoped_refptr<net::HttpResponseHeaders> override_headers_;
    GURL redirect_url_;

    bool request_completed_ = false;

    // This stores the parameters to FollowRedirect that came from
    // the client. That way we can combine it with any other changes that
    // extensions made to headers in their callbacks.
    struct FollowRedirectParams {
      FollowRedirectParams();
      FollowRedirectParams(const FollowRedirectParams&) = delete;
      FollowRedirectParams& operator=(const FollowRedirectParams&) = delete;
      ~FollowRedirectParams();
      std::vector<std::string> removed_headers;
      net::HttpRequestHeaders modified_headers;
      net::HttpRequestHeaders modified_cors_exempt_headers;
      absl::optional<GURL> new_url;
    };
    std::unique_ptr<FollowRedirectParams> pending_follow_redirect_params_;

    // A task runner that should be used for the request when non-null. Non-null
    // when this was created for a navigation request.
    scoped_refptr<base::SequencedTaskRunner> navigation_response_task_runner_;

    base::WeakPtrFactory<InProgressRequest> weak_factory_;
  };

  // Constructor public for testing purposes. New instances should be created
  // by calling MaybeProxyRequest().
  HnsProxyingURLLoaderFactory(
      HnsRequestHandler& request_handler,
      content::BrowserContext* browser_context,
      int render_process_id,
      int frame_tree_node_id,
      mojo::PendingReceiver<network::mojom::URLLoaderFactory> receiver,
      mojo::PendingRemote<network::mojom::URLLoaderFactory> target_factory,
      scoped_refptr<RequestIDGenerator> request_id_generator,
      DisconnectCallback on_disconnect,
      scoped_refptr<base::SequencedTaskRunner> navigation_response_task_runner);

  HnsProxyingURLLoaderFactory(const HnsProxyingURLLoaderFactory&) = delete;
  HnsProxyingURLLoaderFactory& operator=(
      const HnsProxyingURLLoaderFactory&) = delete;

  ~HnsProxyingURLLoaderFactory() override;

  static bool MaybeProxyRequest(
      content::BrowserContext* browser_context,
      content::RenderFrameHost* render_frame_host,
      int render_process_id,
      mojo::PendingReceiver<network::mojom::URLLoaderFactory>* factory_receiver,
      scoped_refptr<base::SequencedTaskRunner> navigation_response_task_runner);

  // network::mojom::URLLoaderFactory:
  void CreateLoaderAndStart(
      mojo::PendingReceiver<network::mojom::URLLoader> loader_receiver,
      int32_t request_id,
      uint32_t options,
      const network::ResourceRequest& request,
      mojo::PendingRemote<network::mojom::URLLoaderClient> client,
      const net::MutableNetworkTrafficAnnotationTag& traffic_annotation)
      override;
  void Clone(mojo::PendingReceiver<network::mojom::URLLoaderFactory>
                 loader_receiver) override;

 private:
  friend class base::DeleteHelper<HnsProxyingURLLoaderFactory>;
  friend class base::RefCountedDeleteOnSequence<HnsProxyingURLLoaderFactory>;

  void OnTargetFactoryError();
  void OnProxyBindingError();
  void RemoveRequest(InProgressRequest* request);

  void MaybeRemoveProxy();

  const raw_ref<HnsRequestHandler> request_handler_;
  raw_ptr<content::BrowserContext> browser_context_ = nullptr;
  const int render_process_id_;
  const int frame_tree_node_id_;

  mojo::ReceiverSet<network::mojom::URLLoaderFactory> proxy_receivers_;
  mojo::Remote<network::mojom::URLLoaderFactory> target_factory_;

  std::set<std::unique_ptr<InProgressRequest>, base::UniquePtrComparator>
      requests_;

  scoped_refptr<RequestIDGenerator> request_id_generator_;

  DisconnectCallback disconnect_callback_;

  // A task runner that should be used for requests when non-null. Non-null when
  // this was created for a navigation request.
  scoped_refptr<base::SequencedTaskRunner> navigation_response_task_runner_;

  base::WeakPtrFactory<HnsProxyingURLLoaderFactory> weak_factory_;
};

#endif  // HNS_BROWSER_NET_HNS_PROXYING_URL_LOADER_FACTORY_H_
