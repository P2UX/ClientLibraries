## Introduction


What follows is a tutorial that walks developers through the steps of writing custom code for an iOS or tvOS application using P2UX. Developers should be familiar with the Xcode integrated development environment for MacOS and either the Swift or the Objective-C programming languages for custom coding.

Topics include customizable View Controllers, UI controls, and Screens/Panels.

!!! note "Terminology"
    Throughout the P2UX documentation the “developer” refers to you, the reader who is developing an app with P2UX. The “designer” refers to the individual whose principal job is to create the appearance and style of the app and its visual components. “PRL” refers to the P2UX Rendering Library for iOS, which controls the creation and workflow of content created for the P2UX platform.

!!! note "Coding examples and code additions"
    In the coding examples below and throughout the documentation, changes and additions to be made by the developer are rendered with a blue highlight.

*P2UX versus the standard Xcode environment*

P2UX based applications for iOS are easy to customize and extend. Since the P2UX iOS rendering library is based on the native UIKit and other iOS primitives, writing custom business logic and integrating custom controls works much the same as it does when writing iOS applications using Xcode.
With the P2UX platform, Storyboards and XIB based User Interface (UI) definitions are no longer required. Instead, applications use P2UX Portable UX Bundles (PUB) either as embedded or dynamically accessed resources. Additionally, P2UX provides hooks to developers that allow them access to all of the components of P2UX, as well as the ability to fully customize instances of the application.

!!! note "Create a P2UX enabled project first"
    This document assumes a P2UX enabled Xcode project has already been created. See [Quick Start](quickstart.md) for steps to create a project.


## P2UX and Xcode

*Objective-C or Swift*

P2UX native rendering libraries for iOS can be extended using either Objective-C or Swift. Developers are free to choose one or the other of these coding languages at any stage during the development process. The instructions below provide explicit reference to the proper use of either language in each applicable step that follows.


## Adding Custom Behaviors

If desired, customizable behaviors can be added to the project code.

A subclass of the `P2UXAppBehavior` class is the starting point for customizing the behavior of the application. From this class, all other parts of the application can be customized. (Common customized behaviors include creation of a Custom Control or overriding a particular View Controller.)

In Objective-C, create a header and module file for your project to store the customized behaviors. The easiest way to do is by making a ==New File== in your project –  a ==Cocoa Touch Class==, which in this example is called `SampleBehavior`.  In Xcode, create a new class from the drop down box labeled ==Subclass of:== and enter ==P2UXAppBehavior== in the field. 
![project wizard](images/quickstartios/image9.png)
This will create two files: `SampleBehavior.h` and `SampleBehavior.m`.

``` hl_lines="1"
@import P2UXApp;
@interface SampleBehavior : P2UXAppBehavior
@end
```

Put any customized behaviors within the implementation here. (Behaviors available for customization are found within the P2UXApp framework, under the file `P2UXAppBehavior`.)

In addition, update `AppDelegate.m` to include the custom behaviors.

To do this, `AppDelegate` must include an instance of the method createBehavior from `P2UXAppCreatorDelegate` protocol. (This will override any default behaviors.) Also, import the `SampleBehavior.h` file so that this class can be instantiated.

Edit `AppDelegate.m` to look like this:
``` hl_lines="2 19 20 21 22"
#import "AppDelegate.h"
#import "SampleBehavior.h"
@interface AppDelegate ()
@end
@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    NSString* appKey;
    appKey = @"xxxxx";
    NSDictionary *opts;
#if DEBUG
    opts = @{P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Stage};
#endif    
    NSDictionary *resources = @[@{P2UXApp_PackageAttrib_FormFactor: [NSNumber numberWithInteger:P2UXFormFactor_Phone], P2UXApp_PackageAttrib_Type:P2UXApp_PackageType_Static, P2UXApp_PackageAttrib_Package:@"phone_pub",P2UXApp_PackageAttrib_Update:P2UXApp_PackageUpdate_None}];
   
    [P2UXAppCreator createApplicationWithKey:appKey opts:opts resources:resources delegate:self];
    return YES;
}

- (P2UXAppBehavior*) createBehaviorForApplication:(NSString *)appid
{
    return [[SampleBehavior alloc] init];
}
@end
```

If working in Swift, create a swift file for your project to store new behaviors.

Create a ==New File== in your project – a ==Swift File==, in this example called `SampleBehavior`. In Xcode, create a new class from the drop down box labeled ==Subclass of:== and enter ==P2UXAppBehavior== in the field. 

Edit the `SampleBehavior.swift` file to look like this:
``` hl_lines="2"
import UIKit
import P2UXApp
class SampleBehavior: P2UXAppBehavior {

// override methods here

}
```

Place any customized behaviors within the implementation. Behaviors available for customization are found within the P2UXApp framework, under the file `P2UXAppBehavior`.

Next, update `AppDelegate.swift` to include the custom behaviors. 

To do this, `AppDelegate` must must include an instance of the method `createBehavior` from the `P2UXAppCreatorDelegate` protocol. (This will override any default behaviors.)

Edit `AppDelegate.swift` to look like this:
``` hl_lines="29 30 31"
import UIKit
import P2UXApp
@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate, P2UXAppCreatorDelegate {
    var window: UIWindow?
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        let appKey = "xxxxx";
        var opts : [AnyHashable : Any];
#if DEBUG
        opts = [
            P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Stage,
            P2UXAppCreator_Opt_LogLevel: P2UXCoreLogLevel.verbose.rawValue as  
            NSNumber
        ]
#endif
        var phoneResources: [AnyHashable : Any] = [
            P2UXApp_PackageAttrib_FormFactor: P2UXFormFactor.phone,
            P2UXApp_PackageAttrib_Type: P2UXApp_PackageType_Static,
            P2UXApp_PackageAttrib_Package: "phone_pub",
            P2UXApp_PackageAttrib_Update: P2UXApp_PackageUpdate_None
        ]
       
        var resources : [Any] = [
            phoneResources
        ]
        P2UXAppCreator.createApplication(withKey: appKey, opts: opts,
        resources: resources, delegate: self)
    }
    func createBehavior(forApplication appid: String) -> P2UXAppBehavior {
        return SampleBehavior();
    }
}
```

## Adding Custom View Controllers

Just like writing traditional Storyboard based iOS applications, writing custom code to interact with the User Interface is done using a subclass of `UIViewController`. Instead of subclassing directly from `UIViewController`, P2UX provides the class `P2UXViewController`, which itself is a subclass of `UIViewController` with a few added helper methods that interface with P2UX generated controls and events.

*The `P2UXViewController` subclass*

Since the P2UX UI is not based on Storyboards, connecting custom methods for each button click action is unnecessary. Instead, the user can override the methods of the `P2UXViewController` class to handle button click and other UI events. (In all other respects, writing code for customized Screen and Panel behavior is the same as for Storyboard-based content.)

The name of a chosen View Controller, such as a Screen or Panel, is the value used to identify the item when the P2UX rendering library creates an instance of this object. (To create an instance of a `P2UXViewController` for a Screen, for example, override the `P2UXAppBehavior` method `createViewControllerWithDef`.) The `def` parameter passed to this method contains the member variable `systemType` that identifies the Screen that is being created. From this object, the developer determines the requested Screen and the specific `P2UXViewController` subclass to be instantiated. 

Objective-C example: creating a custom `P2UXViewController`

``` Objective-C
#import "SampleBehavior.h"
#import "SampleViewController.h"
@import P2UXApp;

@implementation SampleBehavior

- (P2UXViewController*) createViewControllerWithDef:(P2UXDefinition*)def
    	 cache:(BOOL)cache index:(id)index data:(id)data
     	 viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate  {
     	 
	// Determine which screen is being requested, the systemType of the P2UXDefinition class has the name of the screen as given in Builder
    if ([def.systemType isEqualToString:@”home”])
	{
    	// This is our custom home screen controller instance
    	return [[MyHomeController alloc] initWithView:def behavior:self cache:cache index:index data:data viewDelegate:viewDelegate];
	}
	// Make sure to call the superclass if a instance is not created
	return [super createViewControllerWithDef:def cache:cache index:index data:data viewDelegate:viewDelegate];
}
@end
```

Swift example: creating a custom `P2UXViewController`

``` Swift
override func createViewController(withDef def: P2UXDefinition!, cache: Bool, index: Any!, data: Any!, viewDelegate: P2UXViewContainerDelegate!) -> P2UXViewController! {
        
    // Determine which screen is being requested, the systemType of the P2UXDefinition class has the name of the screen as given in Builder
    if def.systemType == "home" {
        // This is our custom home screen controller instance
        return MyHomeController(view: def, behavior: self, cache: cache, index: index, data: data, viewDelegate: viewDelegate)
    }        
    // Make sure to call the superclass if a instance is not created
    return super.createViewController(withDef: def, cache: cache, index: index, data: data, viewDelegate: viewDelegate);
}
```

The `P2UXViewController` view instance managed by your controller is created automatically based on the `P2UXDefinition`.

Just as Screens can have custom `UIViewController` instances to override their behavior, so too can Panels. The `UIViewController` based subclass `P2UXPanelController` manages Panel instances. To create a custom `P2UXPanelController` instance, override the `P2UXAppBehavior` method `createPanelControllerWithDef` and return an instance of a `P2UXPanelController` subclass.

Objective-C example: creating a custom P2UXPanelController

``` Objective-C
- (P2UXViewController*) createPanelControllerWithDef:(P2UXDefinition*)def
      cache:(BOOL)cache index:(id)index data:(id)data
      viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate {

   // Determine which screen is being requested, the systemType of the P2UXDefinition class has the name of the screen as given in Builder
   if ([def.systemType isEqualToString:@”userprofile”]) {
      // This is our custom panel controller instance
      return [[UserProfilePanelController alloc] initWithView:def behavior:self cache:cache index:index data:data viewDelegate:viewDelegate];
   }
   // Make sure to call the superclass if a instance is not created
   return [super createPanelControllerWithDef:def cache:cache 
      index:index data:data viewDelegate:viewDelegate];
}
```

Swift example: creating a custom P2UXPanelController

``` Swift
override func createPanelController(withDef def: P2UXDefinition!, 
      cache: Bool, index: Any!, data: Any!, 
      viewDelegate: P2UXViewContainerDelegate!) -> P2UXViewController! {
        
   // Determine which screen is being requested, the systemType of the
   // P2UXDefinition class has the name of the screen as given in Builder
   if def.systemType == "userprofile" {
       // This is our custom panel controller instance
       return UserProfilePanelController(view: def, behavior: self, cache: cache, index: index, data: data, viewDelegate: viewDelegate)
   }
        
   return super.createPanelController(withDef: def, cache: cache, index: index, data: data, viewDelegate: viewDelegate);
}
```

##Accessing Control Instances from `P2UXViewController`

To access instances of controls, `P2UXViewController` provides a helper method, `controlWithElementSystemTypeOrId`. This method takes the name or ID of the desired control and returns access to the control instance.

Upon creating a new control, such as a new Button, Builder assigns it a “system type.” (Builder generates the system type from the designer’s name for the Button.) Later, the designer might decide to create an alternative variation of the Button. To prevent altering any existing controls, the developer should refer to the new version of the Button by its system type to eliminate overwriting or rewriting any code.

Objective-C example: accessing a control in a `P2UXViewController`

``` Objective-C
-(void) viewDidLoad {
   [super viewDidLoad];

   UIButton* btn = (UIButton*)[self controlWithElementSystemTypeOrId:@”show btn”];
   if (btn != nil) {
      btn.enabled = NO;
   }
}
```

Swift example: accessing a control in a `P2UXViewController`

``` Swift
override func viewDidLoad() {
   super.viewDidLoad()
 
   if let btn = self.control(withElementSystemTypeOrId: "show btn") as? UIButton {
      btn.isEnabled = false;
   }
}
```

##Handling Events from P2UXViewController

End-users of a completed app will interact with controls in a Screen or Panel to generate events. These interactions will, in turn, trigger actions. Typically the PRL handles events directly through event interactions set up in Builder. In some cases, additional custom code may be needed to trigger actions not directly supported by P2UX.

The PRL provides a chain of event handlers that propagate an event through different components. Handling events at any point in the propagation chain provides a way to trigger custom code and also (optionally) to stop the propagation of the event at any specific handler.

Event propagation follows a set pattern.

First, events are relayed to the `P2UXViewController` for the current Screen or Panel. Next, they pass to `P2UXAppBehavior`. Finally, events are handled by the custom instance of the `UIView` itself. If none of these components halt event propagation, the PRL will then handle any interactions defined for the event in Builder.

The `P2UXViewController` contains helper methods that handle a variety of events when they occur. These methods give the developer a chance to integrate any custom code needed from the basic event level without triggering any custom actions.

Most standard control events are routed through the `P2UXViewController` instance. Handling these events is a matter of overriding the appropriate event method in your `P2UXViewController` subclass. Event propagation can be allowed to continue or not depending on the return value from the event handler methods.

Objective-C example: handling a Button press event in `P2UXViewController` subclass

``` Objective-C
- (BOOL) handleButtonClick:(id)sender element:(P2UXElementInstance *)element {
    if ([element.systemType isEqualToString:@"okbtn"]) {
        // take some custom action here for the "okbtn"
            
        // return YES to indicate the event was handled and propagation 
        // shouldn't continue
        return YES;
    }
    return NO;
}
```

Swift example: handling a Button press event in `P2UXViewController` subclass

``` Swift
override func handleButtonClick(_ sender: Any!, element: P2UXElementInstance!) -> Bool {
    if element.systemType == "okbtn" {
        // take some custom action here for the "okbtn"
            
        // return true to indicate the event was handled and propagation 
        // shouldn't continue
        return true;
    }
    return false;
}
```

##Adding Custom UI Controls
UI Controls are the visual components of a Screen or Panel created in the Builder tool. These can be interactive, like Buttons or Sliders, or non-interactive, like Shapes or Text. Although P2UX offers many different UI Controls directly creatable and usable in Builder, a specific application may require developer-defined UI controls. Builder provides a Custom Control component that can be added to a Screen or Panel and then replaced using custom code.

Any `UIView` based control can be injected into your Screens and Panels via a Custom Control. This allows for existing custom controls or third party custom controls to be used with your P2UX based applications.

The PRL manages the size and position of any custom controls defined using Builder. To provide an instance of a custom control, override the method `createControlWithType` in your `P2UXAppBehavior` subclass and return the `UIView` subclassed instance of your control. The first parameter passed to this method, type, is the name of the control set when you created the custom control in Builder. P2UX will pass you the initial size to use with your Custom Control as well as a few other components that can be used to access any custom information created with your control in Builder.

The `elementInstance` parameter contains all the parameters specified using Builder for the control via its `itemSpec` member. Even though the parent `UIView` is passed to the create method, you should not add the instance of your `UIView` control to the parent as this is managed by P2UX when you return the instance of the control.

Objective-C example: creating a custom control in `P2UXAppBehavior` subclass:

``` Objective-C 
- (UIView*) createControlWithType:(NSString *)type elementInstance:(P2UXElementInstance *)elementInstance frame:(CGRect)frame viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate index:(id)index data:(id)data parent:(UIView *)parent {

    if ([type isEqualToString:@"graphcontrol"]) {
        
        CustomGraph* graph = [[CustomGraph alloc] initWithFrame:frame];
        NSDictionary* graphOpts = [elementInstance.itemSpec objectForKey:@"graphopts"];
        if (graphOpts != nil) {
            // handle extra parameters here, for this example we defined an object
            // in the itemspec of the control
            // in Builder named 'graphopts' which has information specific to 
            // our graph object
            graph.opts = graphOpts;
        }
        return graph;
    }
    return [super createControlWithType:type elementInstance:elementInstance frame:frame viewDelegate:viewDelegate index:index data:data parent:parent];
}
```

Swift example: creating a custom control in `P2UXAppBehavior` subclass:

``` Swift
override func createControl(withType type: String!, elementInstance: P2UXElementInstance!, frame: CGRect, viewDelegate: P2UXViewContainerDelegate!, index: Any!, data: Any!, parent: UIView!) -> UIView! {
        
    if type == "graphcontrol" {
        let graph : CustomGraph = CustomGraph(frame: frame);
        if let graphOpts = elementInstance.itemSpec["graphopts"] as? NSDictionary {
            // handle extra parameters here, for this example we defined an object 
            // in the itemspec of the control
            // in Builder named 'graphopts' which has information specific to 
            // our graph object
            graph.opts = graphOpts;
        }
        return graph;
    }
    return super.createControl(withType: type, elementInstance: elementInstance, frame: frame, viewDelegate: viewDelegate, index: index, data: data, parent: parent);
}
```

##Adding Custom Screens/Panels

In addition to using a `UIViewController` based class to override behavior for an application Screen or Panel, the actual Screen or Panel instance can also be overridden to provide any custom code needed for the hosted `UIView` itself.

P2UX will request an instance from `P2UXAppBehavior` when the Screen or Panel is created. If no instance is provided, P2UX will generate a standard instance of the Screen or Panel.

To provide a custom Screen instance, override the method `createScreenWithDef` from a `P2UXAppBehavior` subclass and return an instance of `P2UXScreen`. To provide a custom Panel instance, override the method `createPanelWithDef` and return an instance of `P2UXPanel`. (Both `P2UXScreen` and `P2UXPanel` are `UIView` based subclasses that add some additional helper methods. These methods make it easier to access screen components.)

Objective-C example: creating a custom Screen or Panel instance in P2UXAppBehavior subclass

``` Objective-C
// For custom Screen instances
-(P2UXScreen*) createScreenWithDef:(P2UXDefinition *)def rect:(CGRect)rect index:(id)index data:(id)data viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate {

    // If the screen “systemType” matches the name of the screen, return
    // the custom screen instance
    if ([def.systemType isEqualToString:@"home"]) {
        // Here we return an instance of HomeScreen which is a 
        // subclass of P2UXScreen
        return [[HomeScreen alloc] initWithFrame:rect contents:def viewDelegate:viewDelegate index:index data:data];
    }
    return [super createScreenWithDef:def rect:rect index:index data:data viewDelegate:viewDelegate];
}

// For custom panel instances
- (P2UXPanel*) createPanelWithDef:(P2UXDefinition *)def rect:(CGRect)rect index:(id)index data:(id)data viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate {
    
    // if this is the userinfo panel, return the custom UserInfoPanel instance
    if ([def.systemType isEqualToString:@"userinfo"]) {
        return [[UserInfoPanel alloc] initWithFrame:rect contents:def viewDelegate:viewDelegate index:index data:data];
    }
    return [super createPanelWithDef:def rect:rect index:index data:data viewDelegate:viewDelegate];
}
```

Swift example: creating a custom Screen or Panel instance in `P2UXAppBehavior` subclass

``` Swift
// For custom Screen instances
override func createScreen(withDef def: P2UXDefinition!, rect: CGRect, index: Any!, data: Any!, viewDelegate: P2UXViewContainerDelegate!) -> P2UXScreen! {
        
    // If the screen “systemType” matches the name of the screen, return
    // the custom screen instance.
    if def.systemType == "home" {   
       // Here we return an instance of HomeScreen which is a 
       // subclass of P2UXScreen
       return HomeScreen(frame: rect, contents: def, viewDelegate: viewDelegate, index: index, data: data);
    }
    return super.createScreen(withDef: def, rect: rect, index: index, data: data, viewDelegate: viewDelegate);
}

// For custom panel instances
override func createPanel(withDef def: P2UXDefinition!, rect: CGRect, index: Any!, data: Any!, viewDelegate: P2UXViewContainerDelegate!) -> P2UXPanel! {
        
    // If the panel “systemType” matches the name of the screen, return
    // the custom panel instance.
    if def.systemType == "userinfo" {
           // Here we return an instance of HomeScreen which is a 
           // subclass of P2UXScreen
           return UserInfoPanel(frame: rect, contents: def, viewDelegate: viewDelegate, index: index, data: data);
    }
    return super.createPanel(withDef: def, rect: rect, index: index, data: data, viewDelegate: viewDelegate);
}
```