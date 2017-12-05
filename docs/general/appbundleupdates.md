# Application Bundle Updates
---
One of the benefits of using P2UX application bundles is its ability to update the application content without having to update the native application binary. Application bundles used by native applications can be updated in a few different ways.

## Auto Update
The default update policy for P2UX applications is *Auto Update*. Each time the application is restarted by the user, the *PRL* will compare the version of the application bundle it has against the version for the specified environment on *Builder*. 

!!! note "Application Environments"
	Application Environments are used to separate content into different different testing/usage environments. See [Application Environments](./envandpublish) for details on use of different environments.

If a new version of the application bundle is found, the *PRL* will prompt the user to let them know there is a new version of the application available. Clicking the OK button from that dialog will restart the application workflow with the new content downloaded from *Builder*. The default prompt is a native dialog for the platform.

## Manual Update
One option for updating application content is to create a manual action triggered by the user or by some other event. Using the *Manual* update policy, the *PRL* will never check for updates, but can do when requested by a *Check for Updates* action in the content. This gives the developer control as to when an update check will happen.

If a new version of the application bundle is found, the *PRL* will prompt the user to let them know there is a new version of the application available. Clicking the OK button from that dialog will restart the application workflow with the new content downloaded from *Builder*. The default prompt is a native dialog for the platform.

## No Update
If the application content will never be updated dynamically, the developer can opt to tell the *PRL* to never allow for updates. The only way to update the content is to use *Static* application content bundles that are built into the native application binaries. (See [Static Application Bundle](./staticappbundle)) In this mode, any requests to check for updates are ignored by the *PRL*. 

##Specifying Resources

Its the responsibility of the application developer to let the *PRL* know how the application bundle resources should be used. On startup the application should create an array of objects (one object for each form factor) that define how the resources for the form factor should be used. 

Form factors are indicated by a numeric value:

| Value | Form Factor |
| --- | --- |
| 0 | Phone |
| 1 | Tablet | 
| 4 | TV |

Below is an example of an application that uses *None* as the update policy for the *Phone* form factor. It will never check for updates.

### iOS/tvOS

#### Objective-C

In `AppDelegate.m`:
``` Objective-C 
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    NSString* appKey = @"xxxxx";

    NSArray* resources = @[@{P2UXApp_PackageAttrib_FormFactor:@0, P2UXApp_PackageAttrib_Update:P2UXApp_PackageUpdate_None}];
    NSDictionary* opts;
#ifdef DEBUG
    opts = @{P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Stage};
#endif
    [P2UXAppCreator createApplicationWithKey:appKey opts:opts resources:resources delegate:self];

    return [super application:application didFinishLaunchingWithOptions:launchOptions];
}
```

#### Swift

In `AppDelegate.swift`:
``` Swift
...

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        let appKey = "xxxxx";

        let resources = [
            [P2UXApp_PackageAttrib_FormFactor:0, P2UXApp_PackageAttrib_Update: P2UXApp_PackageUpdate_None]
        ]
        
        var opts : [AnyHashable : Any];
#if DEBUG
        opts = [
            P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Recent,
            P2UXAppCreator_Opt_LogLevel: P2UXCoreLogLevel.verbose.rawValue as NSNumber
        ]
#endif
        
        P2UXAppCreator.createApplication(withKey: appKey, opts: opts, resources: resources, delegate: self)
		return true
    }
...
```

### Android

In `MainActivity.java`: 
``` Java
public class MainActivity extends P2UXAppFragmentActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        mAppKey = "xxxxxx";
        if (savedInstanceState == null) {
            savedInstanceState = new Bundle();
        }
        
        // Build a array of Form Factor objects to setup the resource usage
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        HashMap<String, Object> formFactor = new HashMap<>();
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_FormFactor, 0);
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_Update, P2UXAppTypes.P2UXApp_PackageUpdate_None);
        resources.add(formFactor);
        setResources(resources);
        
        super.onCreate(savedInstanceState);
    }
}
```