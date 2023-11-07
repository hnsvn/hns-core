/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_BOOKMARKS_BOOKMARK_MODEL_LISTENER_IOS_H_
#define HNS_IOS_BROWSER_API_BOOKMARKS_BOOKMARK_MODEL_LISTENER_IOS_H_

#import <Foundation/Foundation.h>

#include <set>

#include "hns/ios/browser/api/bookmarks/hns_bookmarks_observer.h"
#include "components/bookmarks/browser/bookmark_model_observer.h"

@interface BookmarkModelListenerImpl : NSObject <BookmarkModelListener>
- (instancetype)init:(id<BookmarkModelObserver>)observer
       bookmarkModel:(void*)bookmarkModel;
@end

namespace hns {
namespace ios {
class BookmarkModelListener : public bookmarks::BookmarkModelObserver {
 public:
  explicit BookmarkModelListener(id<BookmarkModelObserver> observer,
                                 bookmarks::BookmarkModel* model);
  ~BookmarkModelListener() override;

 private:
  // BookmarksModelListener implementation
  void BookmarkModelLoaded(bookmarks::BookmarkModel* model,
                           bool ids_reassigned) override;
  void BookmarkModelBeingDeleted(bookmarks::BookmarkModel* model) override;
  void BookmarkNodeMoved(bookmarks::BookmarkModel* model,
                         const bookmarks::BookmarkNode* old_parent,
                         size_t old_index,
                         const bookmarks::BookmarkNode* new_parent,
                         size_t new_index) override;
  void BookmarkNodeAdded(bookmarks::BookmarkModel* model,
                         const bookmarks::BookmarkNode* parent,
                         size_t index,
                         bool added_by_user) override;
  void BookmarkNodeRemoved(bookmarks::BookmarkModel* model,
                           const bookmarks::BookmarkNode* parent,
                           size_t old_index,
                           const bookmarks::BookmarkNode* node,
                           const std::set<GURL>& removed_urls) override;
  void BookmarkNodeChanged(bookmarks::BookmarkModel* model,
                           const bookmarks::BookmarkNode* node) override;
  void BookmarkNodeFaviconChanged(bookmarks::BookmarkModel* model,
                                  const bookmarks::BookmarkNode* node) override;
  void BookmarkNodeChildrenReordered(
      bookmarks::BookmarkModel* model,
      const bookmarks::BookmarkNode* node) override;
  void BookmarkAllUserNodesRemoved(bookmarks::BookmarkModel* model,
                                   const std::set<GURL>& removed_urls) override;

  __strong id<BookmarkModelObserver> observer_;
  bookmarks::BookmarkModel* model_;  // NOT OWNED
};

}  // namespace ios
}  // namespace hns

#endif  // HNS_IOS_BROWSER_API_BOOKMARKS_BOOKMARK_MODEL_LISTENER_IOS_H_
