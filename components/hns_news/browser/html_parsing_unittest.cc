// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/hns_news/browser/html_parsing.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace hns_news {

namespace {

std::string GetHTML() {
  return R"(
    <script data-schema="Organization" type="application/ld+json">
            {"name":"The Guardian","url":"http://www.theguardian.com/","logo":{"@type":"ImageObject","url":"https://uploads.guim.co.uk/2018/01/31/TheGuardian_AMP.png","width":190,"height":60},"sameAs":["https://www.facebook.com/theguardian","https://twitter.com/guardian","https://www.youtube.com/user/TheGuardian","https://www.wikidata.org/wiki/Q11148"],"@type":"Organization","@context":"http://schema.org"}
        </script>

        <script data-schema="WebPage" type="application/ld+json">
            {"@id":"https://www.theguardian.com/us","potentialAction":{"@type":"ViewAction","target":"android-app://com.guardian/https/www.theguardian.com/us"},"@type":"WebPage","@context":"http://schema.org"}
        </script>






        < link extra="hi" rel="alternate" href="ios-app://409128287/gnmguardian/us?contenttype=front&amp;source=google" />



    <meta name="robots" content="max-image-preview:large">




    <meta name="twitter:url" content="https://www.theguardian.com/us" />
    <meta property="og:title" content="News, sport and opinion from the Guardian's US edition | The Guardian" />
    <link rel="alternate" href="https://www.theguardian.com/us/rss" title="RSS" type="application/rss+xml" >








    <style class="webfont js-loggable">
    @font-face { font-family: "GH Guardian Headline"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-Light.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Light.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Light.ttf) format("truetype"); font-weight: 300; font-style: normal; font-display: swap; } @font-face { font-family: "Guardian Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-Light.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Light.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Light.ttf) format("truetype"); font-weight: 300; font-style: normal; font-display: swap; } @font-face { font-family: "GH Guardian Headline"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-LightItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-LightItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-LightItalic.ttf) format("truetype"); font-weight: 300; font-style: italic; font-display: swap; } @font-face { font-family: "Guardian Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-LightItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-LightItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-LightItalic.ttf) format("truetype"); font-weight: 300; font-style: italic; font-display: swap; } @font-face { font-family: "GH Guardian Headline"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-Medium.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-Medium.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Medium.ttf) format("truetype"); font-weight: 500; font-style: normal; font-display: swap; } @font-face { font-family: "Guardian Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-Medium.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-Medium.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Medium.ttf) format("truetype"); font-weight: 500; font-style: normal; font-display: swap; } @font-face { font-family: "GH Guardian Headline"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-MediumItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-MediumItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-MediumItalic.ttf) format("truetype"); font-weight: 500; font-style: italic; font-display: swap; } @font-face { font-family: "Guardian Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-MediumItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-MediumItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-MediumItalic.ttf) format("truetype"); font-weight: 500; font-style: italic; font-display: swap; } @font-face { font-family: "GH Guardian Headline"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-Bold.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Bold.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Bold.ttf) format("truetype"); font-weight: 700; font-style: normal; font-display: swap; } @font-face { font-family: "Guardian Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-Bold.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Bold.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-Bold.ttf) format("truetype"); font-weight: 700; font-style: normal; font-display: swap; } @font-face { font-family: "GH Guardian Headline"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-BoldItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-BoldItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-BoldItalic.ttf) format("truetype"); font-weight: 700; font-style: italic; font-display: swap; } @font-face { font-family: "Guardian Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/noalts-not-hinted/GHGuardianHeadline-BoldItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-BoldItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-headline/latin1-not-hinted/GHGuardianHeadline-BoldItalic.ttf) format("truetype"); font-weight: 700; font-style: italic; font-display: swap; } @font-face { font-family: "GuardianTextEgyptian"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-Regular.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-Regular.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-Regular.ttf) format("truetype"); font-weight: 400; font-style: normal; font-display: swap; } @font-face { font-family: "Guardian Text Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-Regular.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-Regular.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-Regular.ttf) format("truetype"); font-weight: 400; font-style: normal; font-display: swap; } @font-face { font-family: "GuardianTextEgyptian"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-RegularItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-RegularItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-RegularItalic.ttf) format("truetype"); font-weight: 400; font-style: italic; font-display: swap; } @font-face { font-family: "Guardian Text Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-RegularItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-RegularItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-RegularItalic.ttf) format("truetype"); font-weight: 400; font-style: italic; font-display: swap; } @font-face { font-family: "GuardianTextEgyptian"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-Bold.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-Bold.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-Bold.ttf) format("truetype"); font-weight: 700; font-style: normal; font-display: swap; } @font-face { font-family: "Guardian Text Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-Bold.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-Bold.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-Bold.ttf) format("truetype"); font-weight: 700; font-style: normal; font-display: swap; } @font-face { font-family: "GuardianTextEgyptian"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-BoldItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-BoldItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-BoldItalic.ttf) format("truetype"); font-weight: 700; font-style: italic; font-display: swap; } @font-face { font-family: "Guardian Text Egyptian Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/noalts-not-hinted/GuardianTextEgyptian-BoldItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-BoldItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textegyptian/latin1-not-hinted/GuardianTextEgyptian-BoldItalic.ttf) format("truetype"); font-weight: 700; font-style: italic; font-display: swap; } @font-face { font-family: "GuardianTextSans"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-Regular.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-Regular.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-Regular.ttf) format("truetype"); font-weight: 400; font-style: normal; font-display: swap; } @font-face { font-family: "Guardian Text Sans Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-Regular.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-Regular.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-Regular.ttf) format("truetype"); font-weight: 400; font-style: normal; font-display: swap; } @font-face { font-family: "GuardianTextSans"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-RegularItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-RegularItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-RegularItalic.ttf) format("truetype"); font-weight: 400; font-style: italic; font-display: swap; } @font-face { font-family: "Guardian Text Sans Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-RegularItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-RegularItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-RegularItalic.ttf) format("truetype"); font-weight: 400; font-style: italic; font-display: swap; } @font-face { font-family: "GuardianTextSans"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-Bold.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-Bold.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-Bold.ttf) format("truetype"); font-weight: 700; font-style: normal; font-display: swap; } @font-face { font-family: "Guardian Text Sans Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-Bold.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-Bold.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-Bold.ttf) format("truetype"); font-weight: 700; font-style: normal; font-display: swap; } @font-face { font-family: "GuardianTextSans"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-BoldItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-BoldItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-BoldItalic.ttf) format("truetype"); font-weight: 700; font-style: italic; font-display: swap; } @font-face { font-family: "Guardian Text Sans Web"; src: url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/noalts-not-hinted/GuardianTextSans-BoldItalic.woff2) format("woff2"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-BoldItalic.woff) format("woff"), url(https://assets.guim.co.uk/static/frontend/fonts/guardian-textsans/latin1-not-hinted/GuardianTextSans-BoldItalic.ttf) format("truetype"); font-weight: 700; font-style: italic; font-display: swap; }
    </style>

    <link rel="petemill" href="everywhere" />
    <link rel="service.feed" type="application/json" href="/rss-relative-test" />


    <style class="js-loggable">
        .svg .i,
        .svg .caption:before,
        .svg figcaption:before,
        .svg blockquote.quoted:before {
            background-image: none;
        }
        .is-updating {
            background-image: url(https://assets.guim.co.uk/images/f2465f9293ea046b91128035ecf4cdd5/auto-update-activity.gif);
        }
        .is-updating--dark {
            background-image: url(https://assets.guim.co.uk/images/614abf55ccbb65d1bb691aa8bf64fd7f/auto-update-activity-dark.gif);
        }
        .tweet__user-name:before {
            background-image: url(https://assets.guim.co.uk/images/twitter/ca807f
    )";
}

}  // namespace

TEST(HnsNewsHTMLParsing, GetFeedUrlsFromHTML) {
  GURL site_url = GURL("https://www.example.com/page");
  auto feed_urls = GetFeedURLsFromHTMLDocument("utf8", GetHTML(), site_url);
  // There are 3 <link> elements in the sample.
  // But only 2 should be treated as valid pointers to potential feeds.
  ASSERT_EQ(feed_urls.size(), 2u);
  ASSERT_EQ(feed_urls[0].spec(), "https://www.theguardian.com/us/rss");
  ASSERT_EQ(feed_urls[1].spec(), "https://www.example.com/rss-relative-test");
}

}  // namespace hns_news
