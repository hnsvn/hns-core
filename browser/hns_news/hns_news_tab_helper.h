// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_HNS_NEWS_HNS_NEWS_TAB_HELPER_H_
#define HNS_BROWSER_HNS_NEWS_HNS_NEWS_TAB_HELPER_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/observer_list_types.h"
#include "base/scoped_observation.h"
#include "hns/components/hns_news/browser/hns_news_controller.h"
#include "hns/components/hns_news/browser/publishers_controller.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class RenderFrameHost;
}

class HnsNewsTabHelper
    : public content::WebContentsUserData<HnsNewsTabHelper>,
      public content::WebContentsObserver,
      public hns_news::PublishersController::Observer {
 public:
  struct FeedDetails {
    GURL feed_url;
    std::string title;
  };

  class PageFeedsObserver : public base::CheckedObserver {
   public:
    virtual void OnAvailableFeedsChanged(
        const std::vector<FeedDetails>& feeds) = 0;
  };

  HnsNewsTabHelper(const HnsNewsTabHelper&) = delete;
  HnsNewsTabHelper& operator=(const HnsNewsTabHelper&) = delete;

  ~HnsNewsTabHelper() override;

  const std::vector<FeedDetails> GetAvailableFeeds();
  bool IsSubscribed(const FeedDetails& feed_details);
  bool IsSubscribed();

  void ToggleSubscription(const FeedDetails& feed_details);

  void OnReceivedRssUrls(const GURL& site_url, std::vector<GURL> feed_urls);
  void OnFoundFeeds(const GURL& site_url,
                    std::vector<hns_news::mojom::FeedSearchResultItemPtr>);

  void AddObserver(PageFeedsObserver* observer);
  void RemoveObserver(PageFeedsObserver* observer);

  // content::WebContentsObserver:
  void PrimaryPageChanged(content::Page& page) override;
  void DOMContentLoaded(content::RenderFrameHost* rfh) override;

  // hns_news::PublisherController::Observer:
  void OnPublishersUpdated(
      hns_news::PublishersController* controller) override;

 private:
  explicit HnsNewsTabHelper(content::WebContents* contents);

  void AvailableFeedsChanged();

  raw_ptr<hns_news::HnsNewsController> controller_;

  std::vector<FeedDetails> rss_page_feeds_;
  base::ObserverList<PageFeedsObserver> observers_;

  base::ScopedObservation<hns_news::PublishersController,
                          hns_news::PublishersController::Observer>
      publishers_observation_{this};

  base::WeakPtrFactory<HnsNewsTabHelper> weak_ptr_factory_{this};

  friend WebContentsUserData;
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

#endif  // HNS_BROWSER_HNS_NEWS_HNS_NEWS_TAB_HELPER_H_
