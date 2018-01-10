## P2UX MQTT Data Source for iOS/tvOS
This is an adaptation of the MQTT-Client-Framework Library as a P2UXDataSource. This allows for use of MQTT data sources within P2UX based applications on iOS & tvOS devices. Like all P2UX Data Sources, there needs to be an associated Data Source defined in Builder for your application that defines the requests (topics) and the data schemas that go along with the data. See the [P2UX developer site](http://developer.p2ux.com/datasources/mqttdatasource/) for details on how to create an MQTT Data Source in Builder.

UI controls that are linked to a MQTTDataSource Request will subscribe to the topic associated with the request referenced in the data link for the control. If the topic is already subscribed, the subscription will be ignored. Any messages that are received for the specified topic will be sent to associated links and cached in memory during the lifetime of the application. 

Persistence of the data from a MQTT topic is controlled by the request attribute 'persist' which will cache the last value seen until the next message from the MQTT server arrives.
 
A schema is required for the data associated with a topic. The P2UXDataSource will convert any received data into the appropriate type based on the Schema. If the data received doesn't match the schema, it will be rejected.

## Project setup in Xcode
The first requirement is to get your P2UX application setup in Xcode. See the [P2UX Developer Site](http://developer.p2ux.com/ios/quickstart/) for an explanation of how to get started. There is also a [sample application](https://github.com/P2UX/P2UX-Sample-App-iOS) for iOS on GitHub that can be used as a starting point for your application.

### Add MQTT-Client-Framework
Once you have your basic application setup and running. You'll need to get the MQTT-Client-Framework added to your Xcode project. The library can be found [here](https://github.com/novastone-media/MQTT-Client-Framework). The choice of how to integrate the MQTT-Client-Framework into your Xcode project is up to you. The `Installation` instructions provide a few choices for integration. 

### Add P2UX MQTTDataSource code
The last step is to add the P2UX MQTTDataSource code into your project. In Xcode:

* Copy the contents of either the `objective-c` or `swift` directory (depending on your language of choice) into your project folder. (Eventually this will be hosted in a CocoaPods and/or Carthage).
* Add the files to your Xcode project. 
    * Right click on a folder in your Xcode Project and select `Add files to "<project name>". 
    * Select the files you copied to your project directory and add them.
* In the `AppDelegate.m` or `AppDelegate.swift` file for your project, add the line from the example code below to setup the use of the `MQTTDataSource` the class for the `MQTT` data source type. The `setup()` method takes care of registering the class as the Data Source instance for MQTT.

Objective-C
```
#import "AppDelegate.h"
#import "MQTTDataSource.h"
@import P2UXApp;

@interface AppDelegate ()

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    NSString* appKey = @"xxxx.xxxx";
    NSDictionary* opts = nil;
#ifdef DEBUG
    opts = @{P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Recent, P2UXAppCreator_Opt_LogLevel:[NSNumber numberWithInteger:P2UXCoreLogLevelVerbose]};
#endif
    
    // Setup the MQTTDataSource here        
    [MQTTDataSource setup];
    
    // Initialize P2UX Rendering Library
    [P2UXAppCreator createApplicationWithKey:appKey opts:opts resources:nil delegate:self];
    return YES;
}
@end
```

Swift
```
import UIKit
import P2UXApp

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate, P2UXAppCreatorDelegate {

    var window: UIWindow?


    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        
        let appKey = "xxxx.xxxx";

        var opts : [AnyHashable : Any];
#if DEBUG
        opts = [
            P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Stage,
            P2UXAppCreator_Opt_LogLevel: P2UXCoreLogLevel.verbose.rawValue as NSNumber
        ]
#endif
        // Setup the MQTTDataSource here        
        MQTTDataSource.setup();
        
        // Initialize P2UX Rendering Library
        P2UXAppCreator.createApplication(withKey: appKey, opts: opts, resources: nil, delegate: self)
        return true
    }
    
    func createBehavior(forApplication appid: String) -> P2UXAppBehavior {
        return MQTTBehavior();
    }
}
```

For documentation and help, visit [http://developer.p2ux.com](http://developer.p2ux.com)
