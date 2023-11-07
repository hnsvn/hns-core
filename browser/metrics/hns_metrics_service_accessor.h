/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_METRICS_HNS_METRICS_SERVICE_ACCESSOR_H_
#define HNS_BROWSER_METRICS_HNS_METRICS_SERVICE_ACCESSOR_H_

#include "components/metrics/metrics_service_accessor.h"

// This class limits and documents access to metrics service helper methods.
// Since these methods are private, each user has to be explicitly declared
// as a 'friend' below.
class HnsMetricsServiceAccessor : public metrics::MetricsServiceAccessor {
 private:
  friend bool ShouldShowCrashReportPermissionAskDialog();

  static bool IsMetricsAndCrashReportingEnabled();
};

#endif  // HNS_BROWSER_METRICS_HNS_METRICS_SERVICE_ACCESSOR_H_
