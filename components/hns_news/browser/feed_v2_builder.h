// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_NEWS_BROWSER_FEED_V2_BUILDER_H_
#define HNS_COMPONENTS_HNS_NEWS_BROWSER_FEED_V2_BUILDER_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/scoped_refptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_news/browser/channels_controller.h"
#include "hns/components/hns_news/browser/feed_fetcher.h"
#include "hns/components/hns_news/browser/publishers_controller.h"
#include "hns/components/hns_news/browser/signal_calculator.h"
#include "hns/components/hns_news/browser/suggestions_controller.h"
#include "hns/components/hns_news/common/hns_news.mojom.h"
#include "components/history/core/browser/history_service.h"
#include "components/prefs/pref_service.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace hns_news {

using BuildFeedCallback = mojom::HnsNewsController::GetFeedV2Callback;
using GetSignalsCallback = mojom::HnsNewsController::GetSignalsCallback;

class FeedV2Builder {
 public:
  FeedV2Builder(
      PublishersController& publishers_controller,
      ChannelsController& channels_controller,
      SuggestionsController& suggestions_controller,
      PrefService& prefs,
      history::HistoryService& history_service,
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);
  FeedV2Builder(const FeedV2Builder&) = delete;
  FeedV2Builder& operator=(const FeedV2Builder&) = delete;
  ~FeedV2Builder();

  void Build(BuildFeedCallback callback);
  void GetSignals(GetSignalsCallback callback);

 private:
  void FetchFeed();
  void OnFetchedFeed(FeedItems items, ETags etags);

  void CalculateSignals();
  void OnCalculatedSignals(Signals signals);

  void GetSuggestedPublisherIds();
  void OnGotSuggestedPublisherIds(
      const std::vector<std::string>& suggested_ids);

  void BuildFeedFromArticles();

  void NotifyBuildCompleted(BuildFeedCallback callback);

  raw_ref<PublishersController> publishers_controller_;
  raw_ref<ChannelsController> channels_controller_;
  raw_ref<SuggestionsController> suggestions_controller_;
  raw_ref<PrefService> prefs_;

  FeedFetcher fetcher_;
  SignalCalculator signal_calculator_;

  FeedItems raw_feed_items_;
  Signals signals_;
  std::vector<std::string> suggested_publisher_ids_;

  bool is_building_ = false;
  std::vector<BuildFeedCallback> pending_callbacks_;

  base::WeakPtrFactory<FeedV2Builder> weak_ptr_factory_{this};
};

}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_BROWSER_FEED_V2_BUILDER_H_
