/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_BASE_HNS_SHIELDS_SERVICE_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_BASE_HNS_SHIELDS_SERVICE_H_

#include <stdint.h>

#include <memory>
#include <string>
// TODO(hns): <mutex> is an unapproved C++11 header
#include <mutex>  // NOLINT

#include "base/files/file_path.h"
#include "base/task/sequenced_task_runner.h"
#include "hns/components/hns_component_updater/browser/hns_component.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"

using hns_component_updater::HnsComponent;

namespace hns_shields {

// The hns shields service in charge of checking hns shields like ad-block,
// tracking protection, etc.
class BaseHnsShieldsService {
 public:
  explicit BaseHnsShieldsService(
      scoped_refptr<base::SequencedTaskRunner> task_runner);
  virtual ~BaseHnsShieldsService();
  bool Start();
  bool IsInitialized() const;
  virtual void ShouldStartRequest(const GURL& url,
                                  blink::mojom::ResourceType resource_type,
                                  const std::string& tab_host,
                                  bool aggressive_blocking,
                                  bool* did_match_rule,
                                  bool* did_match_exception,
                                  bool* did_match_important,
                                  std::string* mock_data_url);

  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner();

 protected:
  virtual bool Init() = 0;

 private:
  void InitShields();

  bool initialized_;
  std::mutex initialized_mutex_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
};

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_BASE_HNS_SHIELDS_SERVICE_H_