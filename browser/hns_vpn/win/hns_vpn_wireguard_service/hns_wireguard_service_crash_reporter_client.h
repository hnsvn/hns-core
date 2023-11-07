/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_HNS_WIREGUARD_SERVICE_CRASH_REPORTER_CLIENT_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_HNS_WIREGUARD_SERVICE_CRASH_REPORTER_CLIENT_H_

#include <string>

#include "base/files/file_path.h"
#include "components/crash/core/app/crash_reporter_client.h"

class HnsWireguardCrashReporterClient
    : public crash_reporter::CrashReporterClient {
 public:
  static void InitializeCrashReportingForProcess(
      const std::string& process_type);

  HnsWireguardCrashReporterClient();

  HnsWireguardCrashReporterClient(const HnsWireguardCrashReporterClient&) =
      delete;
  HnsWireguardCrashReporterClient& operator=(
      const HnsWireguardCrashReporterClient&) = delete;

  ~HnsWireguardCrashReporterClient() override;

  // crash_reporter::CrashReporterClient:
  bool ShouldCreatePipeName(const std::wstring& process_type) override;
  bool GetAlternativeCrashDumpLocation(std::wstring* crash_dir) override;
  void GetProductNameAndVersion(const std::wstring& exe_path,
                                std::wstring* product_name,
                                std::wstring* version,
                                std::wstring* special_build,
                                std::wstring* channel_name) override;
  bool ShouldShowRestartDialog(std::wstring* title,
                               std::wstring* message,
                               bool* is_rtl_locale) override;
  bool AboutToRestart() override;
  bool GetIsPerUserInstall() override;
  bool GetShouldDumpLargerDumps() override;
  int GetResultCodeRespawnFailed() override;
  bool GetCrashDumpLocation(std::wstring* crash_dir) override;
  bool GetCrashMetricsLocation(std::wstring* metrics_dir) override;
  bool IsRunningUnattended() override;
  bool GetCollectStatsConsent() override;
  bool GetCollectStatsInSample() override;
  bool ReportingIsEnforcedByPolicy(bool* enabled) override;
  bool ShouldMonitorCrashHandlerExpensively() override;
  bool EnableBreakpadForProcess(const std::string& process_type) override;
};

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_HNS_WIREGUARD_SERVICE_CRASH_REPORTER_CLIENT_H_
