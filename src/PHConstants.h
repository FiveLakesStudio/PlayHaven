//
//  PHConstants.h
//  playhaven-sdk-ios
//
//  Created by Jesus Fernandez on 4/14/11.
//  Copyright 2011 Playhaven. All rights reserved.
//

#import <UIKit/UIImage.h>

// Constants
#define PH_SDK_VERSION @"1.10.2"

#ifndef PH_BASE_URL
#define PH_BASE_URL @"http://api2.playhaven.com"
#endif

#ifndef PH_CONTENT_ADDRESS
#define PH_CONTENT_ADDRESS @"http://media.playhaven.com"
#endif


// PHContentView notification that a callback is ready for processing
//
#define PHCONTENTVIEW_CALLBACK_NOTIFICATION  @"PHContentViewPHCallbackNotification"


// PH_DISPATCH_PROTOCOL_VERSION
// Defines characteristics of the requests that get sent from content units to
// native code. See content-templates:src/js/playhaven.js for template impl.
//
//   1: GET request with dispatch parameter keys and values in query string
//   2: GET request with dispatch parameters encoded as a single JSON string 
//      in query string. Rewards support requires this setting.
//   3: Unknown dispatches are ignored instead of throwing an error
// * 4: ph://launch dispatches no longer create native spinner views
#define PH_DISPATCH_PROTOCOL_VERSION 5

// PH_REQUEST_TIMEOUT
// Defines the maximum amount of time that an API request will wait for a 
// response from the server.
#define PH_REQUEST_TIMEOUT 10

// PH_USE_CONTENT_VIEW_RECYCLING
// Recycles content view instances to reduce the number of allocations.
// Behavior of the SDK without this define has not been tested. 
#define PH_USE_CONTENT_VIEW_RECYCLING

// PH_DISMISS_CONTENT_REQUEST_WHEN_BACKGROUNDED
// By default, content requests are dismissed when the app is backgrounded.
// Set PH_DONT_DISMISS_WHEN_BACKGROUNDED as a preprocessor macro to disable this behavior.
#ifndef PH_DONT_DISMISS_WHEN_BACKGROUNDED
#define PH_DISMISS_WHEN_BACKGROUNDED
#endif

// PH_USE_STOREKIT
// By default, PlayHaven will require the StoreKit framework. Builds that don't need 
// IAP tracking features may define PH_USE_STOREKIT to be 0.
#ifndef PH_USE_STOREKIT
#define PH_USE_STOREKIT 1
#endif

// PH_NAMESPACE_LIBS
// The Unity3D plugin requires namespaced classes so that they may co-exist 
// alongside libraries that use the same classes. However, source builds that are 
// also using these classes should be able to use the existing implementations.
#ifdef PH_NAMESPACE_LIBS
#define PH_NAMESPACE_SBJSON 1
#define PH_NAMESPACE_SDURLCACHE 1
#define PH_NAMESPACE_OPENUDID 1
#endif

// PH_NAMESPACE_SBJSON
// The Unity3D plugin requires namespaced SBJSON classes so that they may co-exist 
// alongside libraries that use SBJSON. However, source builds that are also using 
// SBJSON should be able to use the SBJSON classes already in their project. This 
// allows the SDK to accommodate both.
#ifndef PH_NAMESPACE_SBJSON
#define PH_SBJSONBASE_CLASS SBJsonBase
#define PH_SBJSONPARSER_CLASS SBJsonParser
#define PH_SBJSONWRITER_CLASS SBJsonWriter
#define PH_SBJSONERRORDOMAIN_CONST SBJSONErrorDomain
#else
#define PH_SBJSONBASE_CLASS SBJsonBasePH
#define PH_SBJSONPARSER_CLASS SBJsonParserPH
#define PH_SBJSONWRITER_CLASS SBJsonWriterPH
#define PH_SBJSONERRORDOMAIN_CONST SBJSONErrorDomainPH
#endif

// PH_NAMESPACE_SDURLCACHE
#ifndef PH_NAMESPACE_SDURLCACHE
#define PH_SDURLCACHE_CLASS SDURLCache
#define PH_SDCACHEDURLRESPONSE_CLASS SDCachedURLResponse
#else
#define PH_SDURLCACHE_CLASS SDURLCachePH
#define PH_SDCACHEDURLRESPONSE_CLASS SDCachedURLResponsePH
#endif

// PH_NAMESPACE_OPENUDID
#ifndef PH_NAMESPACE_OPENUDID
#define PH_OPENUDID_CLASS OpenUDID
#else
#define PH_OPENUDID_CLASS OpenUDIDPH
#endif


// PH_USE_UNIQUE_IDENTIFIER
// This will lead to rejection from the app store very soon, but will help PlayHaven
// correlate Apple UDIDs with OpenUDIDs. To disable, set to 0.
#ifndef PH_USE_UNIQUE_IDENTIFIER
#define PH_USE_UNIQUE_IDENTIFIER 1
#endif

// PH_USE_MAC_ADDRESS
// This may lead to rejection from the app store at some point, but will help PlayHaven
// reliably track devices and conversion. To disable, set to 0.
#ifndef PH_USE_MAC_ADDRESS
#define PH_USE_MAC_ADDRESS 1
#endif

// PH_USE_OPENUDID
// We support the use of OpenUDID as an optional supplemental device identifier.
// To disable sending OpenUDIDs set PH_USE_OPENUDID=0
#ifndef PH_USE_OPENUDID
#define PH_USE_OPENUDID 1
#endif

// Macros
#define PH_URL(PATH) [PH_BASE_URL stringByAppendingString:@#PATH]
#define PH_URL_FMT(PATH,FMT) [PH_BASE_URL stringByAppendingFormat:@#PATH, FMT]

#ifndef PH_LOG
#define PH_LOG(COMMENT,...) NSLog(@"[PlayHaven-%@] %@",PH_SDK_VERSION, [NSString stringWithFormat:COMMENT,__VA_ARGS__])
#endif

#ifndef PH_NOTE
#define PH_NOTE(COMMENT) NSLog(@"[PlayHaven-%@] %@",PH_SDK_VERSION, COMMENT)
#endif

#define PH_MULTITASKING_SUPPORTED [[UIDevice currentDevice] respondsToSelector:@selector(isMultitaskingSupported)] && [[UIDevice currentDevice] isMultitaskingSupported]

NSString *PHGID(void);
void PHClearGID(void);

// Errors
typedef enum{
    PHAPIResponseErrorType,
    PHRequestResponseErrorType,
    PHOrientationErrorType,
    PHLoadContextErrorType,
    PHWindowErrorType,
    PHIAPTrackingSimulatorErrorType,
} PHErrorType;

NSError *PHCreateError(PHErrorType errorType);


// PHNetworkStatus
// Determines the status of the device's connectivity. Returns:
// 
// 0: No connection
// 1: Cellular data, 3G/EDGE
// 2: WiFi
int PHNetworkStatus(void);

NSString *PHAgnosticStringValue(id object);

// Caching constant definitions
//
#define PH_PREFETCH_URL_PLIST @"prefetchCache.plist"

#define PH_MAX_CONCURRENT_OPERATIONS    2

#define PH_MAX_SIZE_MEMORY_CACHE        1024*1024          // 1MB mem cache
#define PH_MAX_SIZE_FILESYSTEM_CACHE    1024*1024*10       // 10MB disk cache

//
// Play Haven default images
//
typedef struct{
    int width;
    int height;
    int length;
    char data[];
    
} playHavenImage;

//
// Play Haven default image helper functions
//
UIImage *convertByteDataToUIImage(playHavenImage *phImage);

// Return true if the device has a retina display, false otherwise. Use this to load @2x images
#define IS_RETINA_DISPLAY() [[UIScreen mainScreen] respondsToSelector:@selector(scale)] && [[UIScreen mainScreen] scale] == 2.0f

extern const playHavenImage badge_image;
extern const playHavenImage badge_2x_image;
extern const playHavenImage close_image;
extern const playHavenImage close_active_image;

NSString *PHGID();
void PHClearGID();
