/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_BOOKMARKS_EXPORTER_HNS_BOOKMARKS_EXPORTER_H_
#define HNS_IOS_BROWSER_API_BOOKMARKS_EXPORTER_HNS_BOOKMARKS_EXPORTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, HnsBookmarksExporterState) {
  HnsBookmarksExporterStateCompleted,
  HnsBookmarksExporterStateStarted,
  HnsBookmarksExporterStateCancelled,
  HnsBookmarksExporterStateErrorCreatingFile,
  HnsBookmarksExporterStateErrorWritingHeader,
  HnsBookmarksExporterStateErrorWritingNodes
};

@class IOSBookmarkNode;

OBJC_EXPORT
@interface HnsBookmarksExporter : NSObject
- (instancetype)init;

- (void)exportToFile:(NSString*)filePath
        withListener:(void (^)(HnsBookmarksExporterState))listener;

- (void)exportToFile:(NSString*)filePath
           bookmarks:(NSArray<IOSBookmarkNode*>*)bookmarks
        withListener:(void (^)(HnsBookmarksExporterState))listener;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_BOOKMARKS_EXPORTER_HNS_BOOKMARKS_EXPORTER_H_
