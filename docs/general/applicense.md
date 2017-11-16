# Application License

All applications using the *P2UX Rendering Library (PRL)* are required to initialize the *PRL* with an application license. This license is used to identify the application and authenticate with *Builder* when fetching application bundles. 

## Online Use
The default method of initializing the *PRL* is to provide the application license as a string to the `P2UXAppCreator` at startup. When providing the license as a string, the application must contact Builder the first time the application is run to authenticate the license and download the details of the license. The license details are stored securely on the device and provide the *PRL* the information needed to get an application started. Once its fetched the license successfully, it doesn't need to authenticate again.

To retrieve the application license in *Builder*:

* From the ==Publish== drop down menu, click the ==Get App License== menu item.
![project wizard](../images/applicense/builderlicense.png)
* A dialog will show with your app’s license. Copy the license.

!!! note "*Coordinator* for macOS"
    If you have created your application with the P2UX *Coordinator* application for macOS, this step is not required as the license string is automatically fetched and inserted into your code when the project is created.

Once the license string is copied, it can be used in the native application code to initialize the *PRL*

### iOS/tvOS

#### Objective-C
In `AppDelegate.m`:
``` Objective-C hl_lines="4"
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    // Include your application license string here
    NSString* appKey = @"000x.xxxxxxxx";
    NSDictionary* opts = nil;
#ifdef DEBUG
    opts = @{P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Stage, P2UXAppCreator_Opt_LogLevel:[NSNumber numberWithInteger:P2UXCoreLogLevelVerbose]};
#endif

    [P2UXAppCreator createApplicationWithKey:appKey opts:opts resources:nil delegate:self];
    return YES;
}
```

#### Swift
In `AppDelegate.swift`:
``` Swift hl_lines="4"
func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
       
    // Include your application license string here  
    let appKey = "000x.xxxxxxxx";

    var opts : [AnyHashable : Any];
#if DEBUG
    opts = [
            P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Stage,
            P2UXAppCreator_Opt_LogLevel: P2UXCoreLogLevel.verbose.rawValue as NSNumber
    ]
#endif
        
    P2UXAppCreator.createApplication(withKey: appKey, opts: opts, resources: nil, delegate: self)
    return true
}
```

### Android
In `MainActivity.java`:
``` Java hl_lines="5"
@Override
public void onCreate(Bundle savedInstanceState)
{
    // Include your application license string here  
    mAppKey = "000x.xxxxxxxx";
    
    if ((getApplicationInfo().flags & ApplicationInfo.FLAG_DEBUGGABLE) == ApplicationInfo.FLAG_DEBUGGABLE)
    {
        if (savedInstanceState == null)
        {
            savedInstanceState = new Bundle();
        }
        savedInstanceState.putString(P2UXAppCreator.P2UXAppCreator_Opt_Env, P2UXAppCreator.P2UXAppCreator_Opt_Env_Stage);
        savedInstanceState.putLong(P2UXAppCreator.P2UXAppCreator_Opt_LogLevel, P2UXLog.P2UXCoreLogFlagVerbose);
    }
    super.onCreate(savedInstanceState);
    applyTranslucentStatusBar();
}
```

### Errors with the License
If the license fails to authenticate, the PRL will display an error message at startup. This error message screen can be customized by the developer in native code (See Customizing Error Screens).

![license error](../images/applicense/error.png)

**Possible Errors:**

| Message | Description |
| --- | --- |
| Invalid application license | The provided license string is not valid. Check the value with the license string provided by *Builder* |
| Unable to retrieve license | The *PRL* is unable to contact *Builder* to validate the license. Check the connectivity of the device |


## Offline Use
For the 
