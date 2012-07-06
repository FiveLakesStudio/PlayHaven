PlayHaven SDK 1.10.2
====================
PlayHaven is a mobile game LTV-maximization platform to help you take control of the business of your games.

Acquire, retain, re-engage, and monetize your players with the help of PlayHaven's powerful marketing platform. Integrate once and embrace the flexibility of the web as you build, schedule, deploy, and analyze your in-game promotions and monetization in real-time through PlayHaven's easy-to-use, web-based dashboard. 

An API token and secret is required to use this SDK. These tokens uniquely identify your app to PlayHaven and prevent others from making requests to the API on your behalf. To get a token and secret, please visit the [PlayHaven Dashboard](https://dashboard.playhaven.com).

If you have any questions, visit the [Help Center](http://help.playhaven.com) or contact us at [support@playhaven.com](mailto:support@playhaven.com).  We also recommend reviewing our [Optimization Guides](http://help.playhaven.com/customer/portal/topics/113947-optimization-guides/articles) to learn the best practices and get the most out of your PlayHaven integration.

What's new in 1.10.2
====================
* Bugfixes for issues with canceling requests and a rare crash involving precaching.

1.10.1
======
* Ability to opt out of user data collection at runtime.

1.10.0
======
* In App Purchase tracking and Virtual Good Promotion support. See "Triggering in-app purchases" and "Tracking in-app purchases" in the API Reference section for information on how to integrate this into your app.
* Adds documentation for disabling StoreKit-based features in the SDK.

1.8.2
=====
* PlayHaven now uses OpenUDID for tracking conversions on the device while still allowing for user opt-out.

Integration
-----------
If you are using Unity for your game, please integrate the Unity SDK located here: https://github.com/playhaven/sdk-unity/

1. Add the following from the sdk-ios directory that you downloaded or cloned from github to your project:
  * src directory 
  * Cache directory
1. (optional) Unless you are already using SBJSON, also add the following to your project:
  * JSON directory
  If your project is already using SBJSON, then you may continue to use those classes or exchange them for the classes included with this SDK. Multiple copies of these classes in the same project may cause errors at compile time.
1. (optional) Unless you are already using OpenUDID, also add the following to your project:
  * OpenUDID directory
  If your project is already using OpenUDID, then you may continue to use those classes or exchange them for the classes included with this SDK. Multiple copies of these classes in the same project may cause errors at compile time.
1. Ensure the following frameworks are included with your project, add any missing frameworks in the Build Phases tab for your application's target:
  * UIKit.framework
  * Foundation.framework
  * CoreGraphics.framework
  * SystemConfiguration.framework
  * CFNetwork.framework
  * StoreKit.framework (**see next bullet**)
1. (optional) If you are not using StoreKit.framework in your project, you may disable IAP Tracking and VGP by setting the following preproccessor macro in your project or target's Build Settings.
    PH_USE_STOREKIT=0
    This will make it possible to build the SDK without StoreKit linked to your project.
1. Include the PlayHavenSDK headers in your code wherever you will be using PlayHaven request classes.

    \#import "PlayHavenSDK.h"
1. In your app delegate's -(void)applicationDidBecomeActive:(UIApplication *)application method, record a game open. See the "Recording game opens" section of the API Reference
1. For each of your placements, you will need to send a content request and implement content request delegate methods. See the "Requesting content for your placements" section of the API Reference
1. If you are planning on using a More Games Widget in your game, we recommend also implementing a notification view for any placements that you plan on using More Games Widgets with to improve chart opens by up to 300%! See the "Add a Notification View (Notifier Badge)" of the API Reference

API Reference
-------------
### Device tracking
This release introduces the use of OpenUDID in addition to our own proprietary identification system for the purposes of authenticating API requests and tracking conversions across applications. OpenUDID is a collaborative open-source effort to create a tracking token that can be shared across the device as well as allow for user-initiared opt out of tracking. There is no additional implementation to take advantage of these changes but it does introduce the following pre-processor macros you may choose to use.

NOTE: The "test device" feature of the PlayHaven Dashboard will only work with games that send either OpenUDID or UDIDs.

By default PH_USE_OPENUDID=1 is set, which will send the OpenUDID value for the current device with the open request. If you would like to opt out of OpenUDID collection, set PH_USE_OPENUDID=0 instead. If you opt out of OpenUDID collection, you may also remove the OpenUDID classes from your project.

By default PH_USE_UNIQUE_IDENTIFIER=1 is set, which will send the Apple UDID alongside these new tokens, which will greatly help us preserve device histories throughout this transitional period. Your app may get rejected by Apple if it does not appropriately implement user opt out, see the "User Opt Out" section below.

By default PH_USE_MAC_ADDRESS=1 is set, which will send the device's wifi MAC address alongside these new tokens.
  
#### User Opt-Out
To comply with Apple policies for the use of device information, we've provided a mechanism for your app to opt-out of collection of UDID and MAC addresses. To set the opt out status for your app, use the following method:

    [PHAPIRequest setOptOutStatus:(BOOL)yesOrNo];

You are responsible for providing an appropriate UI for user opt-out. User data is sent by default.

### Recording game opens
Your app must report each time your application comes to the foreground. PlayHaven uses these events to measure the click-through rate of your content units to help optimize the performance of your implementation. This request is asynchronous and may run in the background while your game is loading.

The best place to run this code in your app is in the implementation of the UIApplicationDelegate's -(void)applicationDidBecomeActive:(UIApplication *)application method. This will record a game open each time the app is foregrounded. The following will send a request:

	[[PHPublisherOpenRequest requestForApp:(NSString *)token secret:(NSString *)secret] send]

**NEW**: If you are using an internal identifier to track individual devices in this game, you may use the customUDID
parameter to pass this identifier along to PlayHaven with the open request.
Asynchronously reports a game open to PlayHaven. 

    PHPublisherOpenRequest *request = [PHPublisherOpenRequest requestForApp:MYTOKEN secret:MYSECRET];
    request.customUDID = @"CUSTOM_UDID" //optional, see below.
    [request send];

#### Precaching content templates
PlayHaven will automatically download and store a number of content templates after a successful PHPublisherOpenRequest. This happens automatically in the background after each open request, so there's no integration required to take advantage of this feature.

### Requesting content for your placements
You may request content for your app using your API token, secret, as well as a placement tag to identify the placement you are requesting content for. Implement PHPublisherContentRequestDelegate methods to receive callbacks from this request. Refer to the section below as well as *example/PublisherContentViewController.m* for a sample implementation.

	PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate];
	request.showsOverlayImmediately = YES; //optional, see below.
	[request send];

*NOTE:* You may set placement_ids through the PlayHaven Developer Dashboard.

Optionally, you may choose to show the loading overlay immediately by setting the request object's *showsOverlayImmediately* property to YES. This is useful if you would like keep users from interacting with your UI while the content is loading.

#### Preloading requests (optional)
To make content requests more responsive, you may choose to preload a content unit for a given placement. This will start a request for a content unit without displaying it, preserving the content unit until you call -(void)send on a  content request for the same placement in your app.

    [[PHPublisherContentRequest requestForApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate] preload];

You may set a delegate for your preload if you would like to be informed when a content request is ready to display. See the sections below for more details.

*NOTE:* Preloading only affects the next content request for a given placement. If you are showing the same placement multiple times in your app, you will need to make additional preload requests after displaying that placement's content unit for the first time.

#### Starting a content request
The request is about to attempt to get content from the PlayHaven API. 

	-(void)requestWillGetContent:(PHPublisherContentRequest *)request;

#### Receiving content
The request received some valid content from the PlayHaven API. This will be the last delegate method a preloading request will receive, unless there is an error.

	-(void)requestDidGetContent:(PHPublisherContentRequest *)request;


#### Preparing to show a content view
If there is content for this placement, it will be loaded at this point. An overlay view will appear over your app and a spinner will indicate that the content is loading. Depending on the transition type for your content your view may or may not be visible at this time. If you haven't before, you should mute any sounds and pause any animations in your app. 

	-(void)request:(PHPublisherContentRequest *)request contentWillDisplay:(PHContent *)content;

#### Content view finished loading
The content has been successfully loaded and the user is now interacting with the downloaded content view. 

	-(void)request:(PHPublisherContentRequest *)request contentDidDisplay:(PHContent *)content;

#### Content view dismissing
The content has successfully dismissed and control is being returned to your app. This can happen as a result of the user clicking on the close button or clicking on a link that will open outside of the app. You may restore sounds and animations at this point.

	-(void)request:(PHPublisherContentRequest *)request contentDidDismissWithType:(PHPublisherContentDismissType *)type;

Type may be one of the following constants:

1. PHPublisherContentUnitTriggeredDismiss: a user or a content unit dismissed the content request
1. PHPublisherNativeCloseButtonTriggeredDismiss: the user used the native close button to dismiss the view
1. PHPublisherApplicationBackgroundTriggeredDismiss: iOS 4.0+ only, the content unit was dismissed because the app was sent to the background
1. PHPublisherNoContentTriggeredDismiss: the content unit was dismissed because there was no content assigned to this placement id

#### Content request failing
If for any reason the content request does not successfully return some content to display or fails to load after the overlay view has appears, the request will stop any any visible overlays will be removed.

	-(void)request:(PHPublisherContentRequest *)request didFailWithError:(NSError *)error;

NOTE: -(void)request:contentDidFailWithError: is now deprecared in favor of request:didFailWithError: please update implementations accordingly.

### Cancelling requests
You may now cancel any API request at any time using the -(void)cancel method. This will also cancel any open network connections and clean up any views in the case of content requests. Canceled requests will not send any more messages to their delegates.

Additionally you may cancel all open API requests for a given delegate. This can be useful if you are not keeping references to API request instances you may have created. As with the -(void)cancel method, canceled requests will not send any more messages to delegates. To cancel all requests:

    [PHAPIRequest cancelAllRequestsWithDelegate:(id)delegate];

### Customizing content display
#### Replace close button graphics
Use the following request method to replace the close button image with something that more closely matches your app. Images will be scaled to a maximum size of 40x40.

	-(UIImage *)request:(PHPublisherContentRequest *)request closeButtonImageForControlState:(UIControlState)state content:(PHContent *)content;

### Unlocking rewards with the SDK
PlayHaven allows you to reward users with virtual currency, in-game items, or any other content within your game. If you have configured unlockable rewards for your content units, you will receive unlock events through a delegate method. It is important to handle these unlock events in every placement that has rewards configured.

> \-(void)request:(PHPublisherContentRequest *)request unlockedReward:(PHReward *)reward;

The PHReward object passed through this method has the following helpful properties:

  * __name__: the name of your reward as configured on the dashboard
  * __quantity__: if there is a quantity associated with the reward, it will be an integer value here
  * __receipt__: a unique identifier that is used to detect duplicate reward unlocks, your app should ensure that each receipt is only unlocked once

### Triggering in-app purchases
Using the Virtual Goods Promotion content unit, PlayHaven can now be used to trigger in app purchase requests in your app. You will need to support the new 

> \-(void)request:(PHPublisherContentRequest *)request makePurchase:(PHPurchase *)purchase;
  
The PHPurchase object passed through this method has the following properties:

  * __productIdentifier__: the product identifier for your purchase, used for making a 
  * __quantity__: if there is a quantity associated with this purchase, it will be an integer value here
  * __receipt__: a unique identifier
  
**Note:** You must retain this purchase object throughout your IAP process. You are responsible for making a SKProduct request before initiating the purchase of this item so as to comply with IAP requirements. Once the item has been purchased you will need to inform the content unit of that purchase using the following:

    [purchase reportResolution:(PHPurchaseResolution)resolution];

**This step is important!** Without a call to reportResolution: the content unit will stall, and your users may not be able to come back to your game. Resolution **must** be one of the following values:

  * PHPurchaseResolutionBuy - the item was purchased and delivered successfully
  * PHPurchaseResolutionCancel - the user was prompted for an item, but the user elected to not buy it
  * PHPurchaseResolutionError - an error prevented the purchase or delivery of the item
  
### Tracking in-app purchases
By providing data on your In App Purchases to PlayHaven, you can track your users' overall lifetime value as well as track conversions from your Virtual Goods Promotion content units. This is done using the PHPublisherIAPTrackingRequest class. To report successful purchases use the following either in your SKPaymentQueueObserver instance or after a purchase has been successfully delivered. 

    PHPublisherIAPTrackingRequest *request = [PHPublisherIAPTrackingRequest requestForApp:TOKEN secret:SECRET product:PRODUCT_IDENTIFIER quantity:QUANTITY resolution:PHPurchaseResolutionBuy];
    [request send]; 

Purchases that are canceled or encounter errors should be reported using the following:

    PHPublisherIAPTrackingRequest *request = [PHPublisherIAPTrackingRequest requestForApp:TOKEN secret:SECRET product:PRODUCT_IDENTIFIER quantity:QUANTITY error:ERROR_OBJECT];
    [request send];
    
If the error comes from an SKPaymentTransaction instance's error property, the SDK will automatically select the correct resolution (buy/cancel) based on the NSError object passed in.

### Add a Notification View (Notifier Badge)
Adding a notification view to your "More Games" button will greatly increase the number of More Games Widget opens for your game, by up to 300%. To create a notification view:

    PHNotificationView *notificationView = [[PHNotificationView alloc] initWithApp:MYTOKEN secret:MYSECRET placement:@"more_games"];
    [myView addSubview:notificationView];
    [notificationView release];
    
Add the notification view as a subview somewhere in your view controller's view. Notification views will remain anchored to the center of the position they are placed in the view, even as the size of the badge changes. Adjust the position of the badge by setting the notificationView's center property. 

    notificationView.center = CGPointMake(10,10);

The notification view will query and update itself when its -(void)refresh method is called.

    [notificationView refresh];  

We recommend refreshing the notification view each time it will appear in your UI. See examples/PublisherContentViewController.m for an example.

You will also need to clear any notification view instances when you successfully launch a content unit. You may do this using the -(void)clear method on any notification views you wish to clear.

#### Testing PHNotificationView
Most of the time the API will return an empty response, which means a notification view will not be shown. You can see a sample notification by using -(void)test; wherever you would use -(void)refresh. It has been marked as deprecated to remind you to switch all instances of -(void)test in your code to -(void)refresh;

#### Customizing notification rendering with PHNotificationRenderer
PHNotificationRenderer is a base class that draws a notification view for a given notification data. The base class implements a blank notification view used for unknown notification types. PHNotificationBadgeRenderer renders a iOS default-style notification badge with a given "value" string. You may customize existing notification renderers and register new ones at runtime using the following method on PHNotificationView

	+(void)setRendererClass:(Class)class forType:(NSString *)type;

Your PHNotificationRenderer subclass needs to implement the following methods to draw and size your notification view appropriately:

	-(void)drawNotification:(NSDictionary *)notificationData inRect:(CGRect)rect;

This method will be called inside of the PHNotificationView instance -(void)drawRect: method whenever the view needs to be drawn. You will use specific keys inside of notificationData to draw your badge in the view. If you need access to the graphics context you may use the UIGraphicsGetCurrentContext() function.

	-(CGSize)sizeForNotification:(NSDictionary *)notificationData;

This method will be called to calculate an appropriate frame for the notification badge each time the notification data changes. Using specific keys inside of notificationData, you will need to calculate an appropriate size.
