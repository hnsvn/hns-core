/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/memory/raw_ptr.h"
#include "base/strings/utf_string_conversions.h"
#include "hns/components/hns_sync/hns_sync_prefs.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/bookmarks/bookmark_model_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"

// This test to mainly testing whether migration only runs once,
// granular testing for is in BookmarkModelTest

namespace {

void CreateOtherBookmarksFolder(bookmarks::BookmarkModel* model) {
  const bookmarks::BookmarkNode* other_node_folder = model->AddFolder(
      model->bookmark_bar_node(), model->bookmark_bar_node()->children().size(),
      model->other_node()->GetTitledUrlNodeTitle());
  model->AddFolder(other_node_folder, 0, u"A");
}

void CreateHnsSyncV1MetaInfo(bookmarks::BookmarkModel* model) {
  const bookmarks::BookmarkNode* node = model->AddURL(
      model->bookmark_bar_node(), model->bookmark_bar_node()->children().size(),
      u"Hns", GURL("https://hns.com"));
  model->SetNodeMetaInfo(node, "object_id", "object_id_value");
}

}  // namespace

class HnsBookmarkModelLoadedObserverBrowserTest
    : public InProcessBrowserTest {
 public:
  void SetUpOnMainThread() override {
    profile_ = browser()->profile();
    bookmark_model_ = BookmarkModelFactory::GetForBrowserContext(profile_);
  }

  PrefService* prefs() { return profile_->GetPrefs(); }

  bookmarks::BookmarkModel* bookmark_model() { return bookmark_model_; }

 private:
  raw_ptr<Profile> profile_ = nullptr;
  raw_ptr<bookmarks::BookmarkModel> bookmark_model_ = nullptr;
};

IN_PROC_BROWSER_TEST_F(HnsBookmarkModelLoadedObserverBrowserTest,
                       PRE_OtherBookmarksMigration) {
  prefs()->SetBoolean(kOtherBookmarksMigrated, false);

  CreateOtherBookmarksFolder(bookmark_model());
}

IN_PROC_BROWSER_TEST_F(HnsBookmarkModelLoadedObserverBrowserTest,
                       OtherBookmarksMigration) {
  EXPECT_TRUE(prefs()->GetBoolean(kOtherBookmarksMigrated));

  ASSERT_EQ(bookmark_model()->other_node()->children().size(), 1u);
  ASSERT_EQ(bookmark_model()->bookmark_bar_node()->children().size(), 0u);
}

IN_PROC_BROWSER_TEST_F(HnsBookmarkModelLoadedObserverBrowserTest,
                       PRE_NoOtherBookmarksMigration) {
  prefs()->SetBoolean(kOtherBookmarksMigrated, true);

  CreateOtherBookmarksFolder(bookmark_model());
}

IN_PROC_BROWSER_TEST_F(HnsBookmarkModelLoadedObserverBrowserTest,
                       NoOtherBookmarksMigration) {
  EXPECT_TRUE(prefs()->GetBoolean(kOtherBookmarksMigrated));

  ASSERT_EQ(bookmark_model()->other_node()->children().size(), 0u);
  ASSERT_EQ(bookmark_model()->bookmark_bar_node()->children().size(), 1u);
}

IN_PROC_BROWSER_TEST_F(HnsBookmarkModelLoadedObserverBrowserTest,
                       PRE_ClearSyncV1MetaInfo) {
  hns_sync::Prefs hns_sync_prefs(prefs());
  hns_sync_prefs.SetSyncV1MetaInfoCleared(false);

  CreateHnsSyncV1MetaInfo(bookmark_model());
}

IN_PROC_BROWSER_TEST_F(HnsBookmarkModelLoadedObserverBrowserTest,
                       ClearSyncV1MetaInfo) {
  hns_sync::Prefs hns_sync_prefs(prefs());
  EXPECT_TRUE(hns_sync_prefs.IsSyncV1MetaInfoCleared());

  const bookmarks::BookmarkNode* node =
      bookmark_model()->bookmark_bar_node()->children()[0].get();
  std::string object_id;
  ASSERT_EQ(node->GetMetaInfoMap(), nullptr);
  ASSERT_FALSE(node->GetMetaInfo("object_id", &object_id));
  ASSERT_TRUE(object_id.empty());
}

IN_PROC_BROWSER_TEST_F(HnsBookmarkModelLoadedObserverBrowserTest,
                       PRE_NoClearSyncV1MetaInfo) {
  hns_sync::Prefs hns_sync_prefs(prefs());
  hns_sync_prefs.SetSyncV1MetaInfoCleared(true);

  CreateHnsSyncV1MetaInfo(bookmark_model());
}

IN_PROC_BROWSER_TEST_F(HnsBookmarkModelLoadedObserverBrowserTest,
                       NoClearSyncV1MetaInfo) {
  hns_sync::Prefs hns_sync_prefs(prefs());
  EXPECT_TRUE(hns_sync_prefs.IsSyncV1MetaInfoCleared());

  const bookmarks::BookmarkNode* node =
      bookmark_model()->bookmark_bar_node()->children()[0].get();
  std::string object_id;
  ASSERT_NE(node->GetMetaInfoMap(), nullptr);
  ASSERT_TRUE(node->GetMetaInfo("object_id", &object_id));
  ASSERT_EQ(object_id, "object_id_value");
}
