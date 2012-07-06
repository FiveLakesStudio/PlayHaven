//
//  PHPublisherIAPTrackingRequest.m
//  playhaven-sdk-ios
//
//  Created by Jesus Fernandez on 1/13/12.
//  Copyright (c) 2012 Playhaven. All rights reserved.
//


//  This will ensure the PH_USE_STOREKIT macro is properly set.
#import "PHConstants.h"

#if PH_USE_STOREKIT!=0
#import "PHPublisherIAPTrackingRequest.h"

@interface PHPublisherIAPTrackingRequest(Private)
+(NSMutableDictionary *)allConversionCookies;
-(void)requestProductInformation;
-(void)sendWithPrice:(NSDecimalNumber *)price andLocale:(NSLocale *)priceLocale;
-(void)sendWithError:(NSError *)error;
-(void)sendWithFailure;
@end

@implementation PHPublisherIAPTrackingRequest

+(NSMutableDictionary *)allConversionCookies{
    static NSMutableDictionary *conversionCookies;
    if (conversionCookies == nil) {
        conversionCookies = [[NSMutableDictionary alloc] init];
    }
    
    return conversionCookies;
}

+(void)setConversionCookie:(NSString *)cookie forProduct:(NSString *)product{
    [[self allConversionCookies] setValue:cookie forKey:product];
}

+(NSString *)getConversionCookieForProduct:(NSString *)product{
    NSString *result = PHAgnosticStringValue([[self allConversionCookies] valueForKey:product]);
    [[self allConversionCookies] setValue:nil forKey:product];
    return result;
}

+(id)requestForApp:(NSString *)token secret:(NSString *)secret product:(NSString *)product quantity:(NSInteger)quantity resolution:(PHPurchaseResolutionType)resolution{
    PHPublisherIAPTrackingRequest *result = [PHPublisherIAPTrackingRequest requestForApp:token secret:secret];
    result.product = product;
    result.quantity = quantity;
    result.resolution = resolution;
    return result;
}

+(id)requestForApp:(NSString *)token secret:(NSString *)secret product:(NSString *)product quantity:(NSInteger)quantity error:(NSError *)error{
    PHPublisherIAPTrackingRequest *result = [PHPublisherIAPTrackingRequest requestForApp:token secret:secret];
    result.error = error;
    result.product = product;
    result.quantity = quantity;
    result.resolution = (error.code == SKErrorPaymentCancelled)? PHPurchaseResolutionCancel : PHPurchaseResolutionError;
    return result;
}

@synthesize product = _product;
@synthesize quantity = _quantity;
@synthesize resolution = _resolution;
@synthesize error = _error;

-(void)dealloc{
    [_product release], _product = nil;
    [_request release], _request = nil;
    [_error release], _error = nil;
    [super dealloc];
}

#pragma mark -
#pragma mark PHAPIRequest

-(NSString *)urlPath{
    return PH_URL(/v3/publisher/iap/);
}

-(void)sendWithPrice:(NSDecimalNumber *)price andLocale:(NSLocale *)priceLocale{
    self.additionalParameters = [NSDictionary dictionaryWithObjectsAndKeys:
                                 self.product, @"product",
                                 [NSNumber numberWithInteger: self.quantity], @"quantity",
                                 [PHPurchase stringForResolution:self.resolution], @"resolution",
                                 @"ios", @"store", 
                                 price, @"price",
                                 [priceLocale objectForKey:NSLocaleCurrencyCode], @"price_locale", 
                                 [PHPublisherIAPTrackingRequest getConversionCookieForProduct:self.product], @"cookie", nil];
    [super send];
}

-(void)sendWithError:(NSError *)error{
    self.additionalParameters = [NSDictionary dictionaryWithObjectsAndKeys:
                                 self.product, @"product",
                                 [NSNumber numberWithInteger: self.quantity], @"quantity",
                                 [PHPurchase stringForResolution:PHPurchaseResolutionError], @"resolution",
                                 @"ios", @"store", 
                                 [NSNumber numberWithInteger:error.code], @"error_code",
                                 [PHPublisherIAPTrackingRequest getConversionCookieForProduct:self.product], @"cookie", nil];
    [super send];
}

-(void)sendWithFailure{
    self.additionalParameters = [NSDictionary dictionaryWithObjectsAndKeys:
                                 self.product, @"product",
                                 [NSNumber numberWithInteger: self.quantity], @"quantity",
                                 [PHPurchase stringForResolution:PHPurchaseResolutionFailure], @"resolution",
                                 @"ios", @"store",
                                 [PHPublisherIAPTrackingRequest getConversionCookieForProduct:self.product], @"cookie", nil];
    [super send];
}

-(void)send{
    switch (self.resolution) {
        case PHPurchaseResolutionBuy:
        case PHPurchaseResolutionCancel:
#ifdef TARGET_IPHONE_SIMULATOR
            // IAP requests don't work from the simulator, but it would 
            // be helpful to allow testing the request itself.
            [self sendWithPrice:[NSDecimalNumber decimalNumberWithString:@"0.0"] andLocale:[NSLocale currentLocale]];
#else
            [self requestProductInformation];
#endif
            break;
        
        case PHPurchaseResolutionError:
            [self sendWithError:self.error];
            break;
            
        case PHPurchaseResolutionFailure:
            [self sendWithFailure];
            break;
    }

}

-(void)requestProductInformation{
    if (_request == nil) {        
        NSSet *productSet = [NSSet setWithObject:self.product];
        _request = [[SKProductsRequest alloc] initWithProductIdentifiers:productSet];
        [_request setDelegate:self];
        [_request start];
    }
}

#pragma mark -
#pragma mark SKProductsRequestDelegate
-(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    if ([response.products count] > 0) {
        SKProduct *productInfo = [response.products objectAtIndex:0];
        [self sendWithPrice:productInfo.price andLocale:productInfo.priceLocale];
    } else {
        [self sendWithFailure];
    }
}

-(void)request:(SKRequest *)request didFailWithError:(NSError *)error{
    [self sendWithError:error];
}

@end
#endif
