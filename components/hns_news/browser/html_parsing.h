// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_NEWS_BROWSER_HTML_PARSING_H_
#define HNS_COMPONENTS_HNS_NEWS_BROWSER_HTML_PARSING_H_

#include <string>
#include <vector>

class GURL;

namespace hns_news {

std::vector<GURL> GetFeedURLsFromHTMLDocument(const std::string& charset,
                                              const std::string& html_body,
                                              const GURL& html_url);

}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_BROWSER_HTML_PARSING_H_
