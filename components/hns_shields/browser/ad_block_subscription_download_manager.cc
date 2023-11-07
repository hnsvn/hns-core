/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_shields/browser/ad_block_subscription_download_manager.h"

#include <memory>
#include <utility>

#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/metrics/histogram_functions.h"
#include "base/uuid.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "build/build_config.h"
#include "components/download/public/background_service/background_download_service.h"
#include "net/traffic_annotation/network_traffic_annotation.h"

namespace hns_shields {

namespace {

const net::NetworkTrafficAnnotationTag
    kHnsShieldsAdBlockSubscriptionTrafficAnnotation =
        net::DefineNetworkTrafficAnnotation(
            "hns_shields_ad_block_subscription",
            R"(
        semantics {
          sender: "Hns Shields"
          description:
            "Hns periodically downloads updates to third-party filter lists "
            "added by users on hns://adblock."
          trigger:
            "After being registered in hns://adblock, any enabled filter "
            "list subscriptions will be updated in accordance with their "
            "`Expires` field if present, or daily otherwise. A manual refresh "
            "for a particular list can also be triggered in hns://adblock."
          data: "The URL endpoint provided by the user in hns://adblock to "
            "fetch list updates from. No user information is sent."
          destination: HNS_OWNED_SERVICE
        }
        policy {
          cookies_allowed: NO
          setting:
            "This request cannot be disabled in settings. However it will "
            "never be made if the corresponding entry is removed from the "
            "hns://adblock page's custom list subscription section."
          policy_exception_justification: "Not yet implemented."
        })");

}  // namespace

AdBlockSubscriptionDownloadManager::AdBlockSubscriptionDownloadManager(
    download::BackgroundDownloadService* download_service,
    scoped_refptr<base::SequencedTaskRunner> background_task_runner)
    : download_service_(download_service),
      is_available_for_downloads_(true),
      background_task_runner_(background_task_runner) {}

AdBlockSubscriptionDownloadManager::~AdBlockSubscriptionDownloadManager() =
    default;

void AdBlockSubscriptionDownloadManager::StartDownload(const GURL& download_url,
                                                       bool from_ui) {
  download::DownloadParams download_params;
  download_params.client = download::DownloadClient::CUSTOM_LIST_SUBSCRIPTIONS;
  download_params.guid = base::Uuid::GenerateRandomV4().AsLowercaseString();
  download_params.callback = base::BindRepeating(
      &AdBlockSubscriptionDownloadManager::OnDownloadStarted, AsWeakPtr(),
      download_url);
  download_params.traffic_annotation = net::MutableNetworkTrafficAnnotationTag(
      kHnsShieldsAdBlockSubscriptionTrafficAnnotation);
  download_params.request_params.url = download_url;
  download_params.request_params.method = "GET";
  if (from_ui) {
    // This triggers a high priority download with no network restrictions to
    // provide status feedback as quickly as possible.
    download_params.scheduling_params.priority =
        download::SchedulingParams::Priority::UI;
    download_params.scheduling_params.battery_requirements =
        download::SchedulingParams::BatteryRequirements::BATTERY_INSENSITIVE;
    download_params.scheduling_params.network_requirements =
        download::SchedulingParams::NetworkRequirements::NONE;
  } else {
    download_params.scheduling_params.priority =
        download::SchedulingParams::Priority::NORMAL;
    download_params.scheduling_params.battery_requirements =
        download::SchedulingParams::BatteryRequirements::BATTERY_INSENSITIVE;
    download_params.scheduling_params.network_requirements =
        download::SchedulingParams::NetworkRequirements::OPTIMISTIC;
  }

  download_service_->StartDownload(std::move(download_params));
}

void AdBlockSubscriptionDownloadManager::CancelAllPendingDownloads() {
  for (const std::pair<std::string, GURL> pending_download :
       pending_download_guids_) {
    const std::string& pending_download_guid = pending_download.first;
    download_service_->CancelDownload(pending_download_guid);
  }
}

bool AdBlockSubscriptionDownloadManager::IsAvailableForDownloads() const {
  return is_available_for_downloads_;
}

void AdBlockSubscriptionDownloadManager::Shutdown() {
  is_available_for_downloads_ = false;
  CancelAllPendingDownloads();
  // notify
}

void AdBlockSubscriptionDownloadManager::OnDownloadServiceReady(
    const std::set<std::string>& pending_download_guids,
    const std::map<std::string, base::FilePath>& successful_downloads) {
  // Ignore any pending guids because they will just retry automatically
  // and we we don't have the url to map them to
}

void AdBlockSubscriptionDownloadManager::OnDownloadServiceUnavailable() {
  is_available_for_downloads_ = false;
}

void AdBlockSubscriptionDownloadManager::OnDownloadStarted(
    const GURL download_url,
    const std::string& guid,
    download::DownloadParams::StartResult start_result) {
  if (start_result == download::DownloadParams::StartResult::ACCEPTED) {
    pending_download_guids_.insert(
        std::pair<std::string, GURL>(guid, download_url));
  }
}

void AdBlockSubscriptionDownloadManager::OnDownloadFailed(
    const std::string& guid) {
  auto it = pending_download_guids_.find(guid);
  if (it == pending_download_guids_.end()) {
    return;
  }
  GURL download_url = it->second;
  pending_download_guids_.erase(guid);

  base::UmaHistogramBoolean(
      "HnsShields.AdBlockSubscriptionDownloadManager.DownloadSucceeded",
      false);

  on_download_failed_callback_.Run(download_url);
}

bool EnsureDirExists(const base::FilePath& destination_dir) {
  return base::CreateDirectory(destination_dir);
}

void AdBlockSubscriptionDownloadManager::OnDownloadSucceeded(
    const std::string& guid,
    base::FilePath downloaded_file) {
  auto it = pending_download_guids_.find(guid);
  if (it == pending_download_guids_.end()) {
    return;
  }
  GURL download_url = it->second;
  pending_download_guids_.erase(guid);

  base::UmaHistogramBoolean(
      "HnsShields.AdBlockSubscriptionDownloadManager.DownloadSucceeded",
      true);

  background_task_runner_->PostTaskAndReplyWithResult(
      FROM_HERE,
      base::BindOnce(&EnsureDirExists,
                     subscription_path_callback_.Run(download_url)),
      base::BindOnce(&AdBlockSubscriptionDownloadManager::OnDirCreated,
                     AsWeakPtr(), downloaded_file, download_url));
}

void AdBlockSubscriptionDownloadManager::OnDirCreated(
    base::FilePath downloaded_file,
    const GURL& download_url,
    bool created) {
  if (!created) {
    on_download_failed_callback_.Run(download_url);
    return;
  }

  base::FilePath list_path = subscription_path_callback_.Run(download_url)
                                 .Append(kCustomSubscriptionListText);

  background_task_runner_->PostTaskAndReplyWithResult(
      FROM_HERE,
      base::BindOnce(&base::ReplaceFile, downloaded_file, list_path, nullptr),
      base::BindOnce(&AdBlockSubscriptionDownloadManager::ReplaceFileCallback,
                     AsWeakPtr(), download_url));
}

void AdBlockSubscriptionDownloadManager::ReplaceFileCallback(
    const GURL& download_url,
    bool success) {
  if (!success) {
    on_download_failed_callback_.Run(download_url);
    return;
  }

  // this should send the data to subscription manager
  on_download_succeeded_callback_.Run(download_url);
}

}  // namespace hns_shields
