/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_P3A_HISTOGRAMS_HNSIZER_H_
#define HNS_COMPONENTS_P3A_HISTOGRAMS_HNSIZER_H_

#include <memory>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/metrics/histogram_base.h"
#include "base/metrics/statistics_recorder.h"

namespace p3a {

class HistogramsHnsizer
    : public base::RefCountedThreadSafe<HistogramsHnsizer> {
 public:
  static scoped_refptr<p3a::HistogramsHnsizer> Create();

  HistogramsHnsizer();

  HistogramsHnsizer(const HistogramsHnsizer&) = delete;
  HistogramsHnsizer& operator=(const HistogramsHnsizer&) = delete;

 private:
  friend class base::RefCountedThreadSafe<HistogramsHnsizer>;
  ~HistogramsHnsizer();

  // Set callbacks for existing Chromium histograms that will be hnstized,
  // i.e. reemitted using a different name and custom buckets.
  void InitCallbacks();

  void DoHistogramHnstization(const char* histogram_name,
                                uint64_t name_hash,
                                base::HistogramBase::Sample sample);

  std::vector<
      std::unique_ptr<base::StatisticsRecorder::ScopedHistogramSampleObserver>>
      histogram_sample_callbacks_;
};

}  // namespace p3a

#endif  // HNS_COMPONENTS_P3A_HISTOGRAMS_HNSIZER_H_
