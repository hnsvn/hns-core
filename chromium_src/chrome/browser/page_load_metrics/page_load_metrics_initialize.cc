/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define InitializePageLoadMetricsForWebContents \
  InitializePageLoadMetricsForWebContents_Chromium
#include "src/chrome/browser/page_load_metrics/page_load_metrics_initialize.cc"
#undef InitializePageLoadMetricsForWebContents

#include "hns/components/hns_perf_predictor/browser/perf_predictor_page_metrics_observer.h"

namespace chrome {

namespace {

class HnsPageLoadMetricsEmbedder : public chrome::PageLoadMetricsEmbedder {
 public:
  explicit HnsPageLoadMetricsEmbedder(content::WebContents* web_contents);
  HnsPageLoadMetricsEmbedder(const HnsPageLoadMetricsEmbedder&) = delete;
  HnsPageLoadMetricsEmbedder& operator=(const HnsPageLoadMetricsEmbedder&) =
      delete;
  ~HnsPageLoadMetricsEmbedder() override;

 protected:
  // page_load_metrics::PageLoadMetricsEmbedderBase:
  void RegisterEmbedderObservers(
      ::page_load_metrics::PageLoadTracker* tracker) override;
};

HnsPageLoadMetricsEmbedder::HnsPageLoadMetricsEmbedder(
    content::WebContents* web_contents)
    : chrome::PageLoadMetricsEmbedder(web_contents) {}

HnsPageLoadMetricsEmbedder::~HnsPageLoadMetricsEmbedder() = default;

void HnsPageLoadMetricsEmbedder::RegisterEmbedderObservers(
    page_load_metrics::PageLoadTracker* tracker) {
  PageLoadMetricsEmbedder::RegisterEmbedderObservers(tracker);

  tracker->AddObserver(
      std::make_unique<
          hns_perf_predictor::PerfPredictorPageMetricsObserver>());
}

}  // namespace

void InitializePageLoadMetricsForWebContents(
    content::WebContents* web_contents) {
  // TODO(bug https://github.com/hnsvn/hns-browser/issues/7784)
  // change
  // android_webview/browser/page_load_metrics/page_load_metrics_initialize.cc
  // as well to register Page Load Metrics Observers
  page_load_metrics::MetricsWebContentsObserver::CreateForWebContents(
      web_contents,
      std::make_unique<HnsPageLoadMetricsEmbedder>(web_contents));
}

}  // namespace chrome
