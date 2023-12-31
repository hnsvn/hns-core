/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_FALLBACK_HOST_H_
#define HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_FALLBACK_HOST_H_

#include <list>
#include <map>
#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_search/common/hns_search_fallback.mojom.h"
#include "url/gurl.h"

namespace network {
class SharedURLLoaderFactory;
class SimpleURLLoader;
}  // namespace network

namespace hns_search {

class HnsSearchFallbackHost final
    : public hns_search::mojom::HnsSearchFallback {
 public:
  HnsSearchFallbackHost(const HnsSearchFallbackHost&) = delete;
  HnsSearchFallbackHost& operator=(const HnsSearchFallbackHost&) = delete;
  explicit HnsSearchFallbackHost(
      scoped_refptr<network::SharedURLLoaderFactory> factory);
  ~HnsSearchFallbackHost() override;

  void FetchBackupResults(const std::string& query_string,
                          const std::string& lang,
                          const std::string& country,
                          const std::string& geo,
                          bool filter_explicit_results,
                          int page_index,
                          FetchBackupResultsCallback callback) override;

  static GURL GetBackupResultURL(const GURL& baseURL,
                                 const std::string& query,
                                 const std::string& lang,
                                 const std::string& country,
                                 const std::string& geo,
                                 bool filter_explicit_results,
                                 int page_index);
  static void SetBackupProviderForTest(const GURL&);

 private:
  using SimpleURLLoaderList =
      std::list<std::unique_ptr<network::SimpleURLLoader>>;
  using URLRequestCallback =
      base::OnceCallback<void(const int,
                              const std::string&,
                              const std::map<std::string, std::string>&)>;

  void OnURLLoaderComplete(
      SimpleURLLoaderList::iterator iter,
      HnsSearchFallbackHost::FetchBackupResultsCallback callback,
      const std::unique_ptr<std::string> response_body);
  SimpleURLLoaderList url_loaders_;
  scoped_refptr<network::SharedURLLoaderFactory> shared_url_loader_factory_;
  base::WeakPtrFactory<HnsSearchFallbackHost> weak_factory_;
};

}  // namespace hns_search

#endif  // HNS_COMPONENTS_HNS_SEARCH_BROWSER_HNS_SEARCH_FALLBACK_HOST_H_
