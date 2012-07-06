//
//  PHPublisherIAPTrackingRequest.h
//  playhaven-sdk-ios
//
//  Created by Jesus Fernandez on 1/13/12.
//  Copyright (c) 2012 Playhaven. All rights reserved.
//

#if PH_USE_STOREKIT!=0
#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "PHAPIRequest.h"
#import "PHPurchase.h"

@interface PHPublisherIAPTrackingRequest : PHAPIRequest<SKProductsRequestDelegate>{
    NSString *_product;
    NSInteger _quantity;
    PHPurchaseResolutionType _resolution;
    SKProductsRequest *_request;
    NSError *_error;
}

+(void)setConversionCookie:(NSString *)cookie forProduct:(NSString *)product;
+(NSString *)getConversionCookieForProduct:(NSString *)product;

+(id)requestForApp:(NSString *)token secret:(NSString *)secret product:(NSString *)product quantity:(NSInteger)quantity resolution:(PHPurchaseResolutionType)resolution;

+(id)requestForApp:(NSString *)token secret:(NSString *)secret product:(NSString *)product quantity:(NSInteger)quantity error:(NSError *)error;

@property (nonatomic, copy) NSString *product;
@property (nonatomic, assign) NSInteger quantity;
@property (nonatomic, retain) NSError *error;
@property (nonatomic, assign) PHPurchaseResolutionType resolution;

@end
#endif
