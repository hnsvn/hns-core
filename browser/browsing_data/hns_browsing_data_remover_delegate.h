/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_BROWSING_DATA_HNS_BROWSING_DATA_REMOVER_DELEGATE_H_
#define HNS_BROWSER_BROWSING_DATA_HNS_BROWSING_DATA_REMOVER_DELEGATE_H_

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/browsing_data/chrome_browsing_data_remover_delegate.h"

namespace base {
class Process;
}  // namespace base

namespace content_settings {
class HnsPrefProvider;
}  // namespace content_settings

class Profile;

class HnsBrowsingDataRemoverDelegate
    : public ChromeBrowsingDataRemoverDelegate {
 public:
  explicit HnsBrowsingDataRemoverDelegate(
      content::BrowserContext* browser_context);
  ~HnsBrowsingDataRemoverDelegate() override;

  HnsBrowsingDataRemoverDelegate(
      const HnsBrowsingDataRemoverDelegate&) = delete;
  HnsBrowsingDataRemoverDelegate operator=(
      const HnsBrowsingDataRemoverDelegate&) = delete;

 private:
  FRIEND_TEST_ALL_PREFIXES(HnsBrowsingDataRemoverDelegateTest,
                           ShieldsSettingsClearTest);

  // ChromeBrowsingDataRemoverDelegate overrides:
  void RemoveEmbedderData(const base::Time& delete_begin,
                          const base::Time& delete_end,
                          uint64_t remove_mask,
                          content::BrowsingDataFilterBuilder* filter_builder,
                          uint64_t origin_type_mask,
                          base::OnceCallback<void(uint64_t)> callback)
                          override;

  void ClearShieldsSettings(base::Time begin_time, base::Time end_time);
#if BUILDFLAG(ENABLE_IPFS)
  void ClearIPFSCache();
  void WaitForIPFSRepoGC(base::Process process);
#endif

  raw_ptr<Profile> profile_ = nullptr;
  base::WeakPtrFactory<HnsBrowsingDataRemoverDelegate> weak_ptr_factory_{
      this};
};

#endif  // HNS_BROWSER_BROWSING_DATA_HNS_BROWSING_DATA_REMOVER_DELEGATE_H_
