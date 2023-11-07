/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_QR_CODE_QR_CODE_GENERATOR_H_
#define HNS_IOS_BROWSER_API_QR_CODE_QR_CODE_GENERATOR_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, HnsQRCodeGeneratorLocatorStyle) {
  HnsQRCodeGeneratorLocatorStyleDefaultSquare,
  HnsQRCodeGeneratorLocatorStyleRounded
} NS_SWIFT_NAME(QRCodeGenerator.LocatorStyle);

typedef NS_ENUM(NSInteger, HnsQRCodeGeneratorModuleStyle) {
  HnsQRCodeGeneratorModuleStyleDefaultSquares,
  HnsQRCodeGeneratorModuleStyleCircles
} NS_SWIFT_NAME(QRCodeGenerator.LocatorStyle);

typedef NS_ENUM(NSInteger, HnsQRCodeGeneratorError) {
  HnsQRCodeGeneratorErrorNone,
  HnsQRCodeGeneratorErrorInputTooLong,
  HnsQRCodeGeneratorErrorUnknown,
} NS_SWIFT_NAME(QRCodeGenerator.Error);

OBJC_EXPORT
NS_SWIFT_NAME(QRCodeGenerator.Options)
@interface HnsQRCodeGeneratorOptions : NSObject
- (instancetype)initWithData:(NSString*)data
                shouldRender:(bool)shouldRender
          renderLogoInCenter:(bool)renderLogoInCenter
           renderModuleStyle:(HnsQRCodeGeneratorModuleStyle)renderModuleStyle
          renderLocatorStyle:
              (HnsQRCodeGeneratorLocatorStyle)renderLocatorStyle;
@end

// Structure for returning QR Code image data.
OBJC_EXPORT
NS_SWIFT_NAME(QRCodeGenerator.Result)
@interface HnsQRCodeGeneratorResult : NSObject
// Return code stating success or failure.
@property(nonatomic, readonly) HnsQRCodeGeneratorError errorCode;
// Image data for generated QR code. May be null on error, or if rendering
// was not requested.
@property(nullable, nonatomic, readonly) UIImage* image;
// QR Code data.
@property(nonatomic, readonly) NSData* data;
// 2-D size of |data| in elements. Note |image| will be upscaled, so this
// does not represent the returned image size.
@property(nonatomic, readonly) CGSize dataSize;

- (instancetype)init NS_UNAVAILABLE;
@end

OBJC_EXPORT
NS_SWIFT_NAME(QRCodeGenerator)
@interface HnsQRCodeGenerator : NSObject
- (HnsQRCodeGeneratorResult*)generateQRCode:
    (HnsQRCodeGeneratorOptions*)request;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_QR_CODE_QR_CODE_GENERATOR_H_
