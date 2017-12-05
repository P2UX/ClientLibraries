#Static Application Bundles
-----

Application developers can choose to include their P2UX application bundle as a static resource contained in their native application binary. Providing the application bundle as a static resource removes the need for connectivity when the application runs for the first time. 

Potential issues to consider with this approach are:

* If dynamic updates are NOT allowed, updates to application bundle have to be made by updating the application binary itself to include the updated bundle. This means resubmitting your application to the respective stores which can take time to get approval.
* If dynamic updates are allowed, a new installation of the application may end up updating its content immediately on first install as the embedded application bundle may be out of date.

!!! important
    Currently *Builder* doesn't provide a direct mechanism to download the bundle for inclusion in a native application. The *Coordinator* product does provide a way to use application bundles. As of this writting, that application has not been released for public use. Check back soon for more information, or contact P2UX support.

The following example sets up an application that supports both *Phone* and *Tablet* form factors to use embedded resources. The resources have been unzipped into subdirectories in the project named `phone` and `tablet` to hold the different resources.

!!! note
    If your project resources are exactly the same for phone and tablet form factors, you can setup the resource usage to point to the same subdirectory in your project. Otherwise they should be in their own subdirectories.
    
## iOS/tvOS

### Objective-C 
In `AppDelegate.m`:
``` Objective-C
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    NSString* appKey = @"xxxxx";

	// Define the resources for both Phone and Tablet
	// for this case, both are using static resources in sub directories named
	// "phone" and "tablet"
    NSArray* resources = @[
    	@{P2UXApp_PackageAttrib_FormFactor:@0,  // Phone 
    	  P2UXApp_PackageAttrib_Update:P2UXApp_PackageUpdate_Auto,
    	  P2UXApp_PackageAttrib_Type:P2UXApp_PackageType_Static,
          P2UXApp_PackageAttrib_Package:@"phone"},
    	@{P2UXApp_PackageAttrib_FormFactor:@1,  // Tablet 
    	  P2UXApp_PackageAttrib_Update:P2UXApp_PackageUpdate_Auto,
    	  P2UXApp_PackageAttrib_Type:P2UXApp_PackageType_Static,
          P2UXApp_PackageAttrib_Package:@"tablet"}    
    ];
                
    NSDictionary* opts;
    // For Debug builds, use the Stage Environment. That way we can test things before
    // release
#ifdef DEBUG
    opts = @{P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Stage};
#endif
    [P2UXAppCreator createApplicationWithKey:appKey opts:opts resources:resources delegate:self];

    return [super application:application didFinishLaunchingWithOptions:launchOptions];
}
```

### Swift
In `AppDelegate.swift`:
``` Swift
...
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        let appKey = "xxxxx";

		// Define the resources for both Phone and Tablet
		// for this case, both are using static resources in sub directories named
		// "phone" and "tablet"
        let resources = [
     		[P2UXApp_PackageAttrib_FormFactor:0,  // Phone 
    		 P2UXApp_PackageAttrib_Update:P2UXApp_PackageUpdate_Auto,
    		 P2UXApp_PackageAttrib_Type:P2UXApp_PackageType_Static,
             P2UXApp_PackageAttrib_Package:"phone"],
    		[P2UXApp_PackageAttrib_FormFactor:1,  // Tablet 
    		 P2UXApp_PackageAttrib_Update:P2UXApp_PackageUpdate_Auto,
    		 P2UXApp_PackageAttrib_Type:P2UXApp_PackageType_Static,
             P2UXApp_PackageAttrib_Package:"tablet"]    
        ]
        
        var opts : [AnyHashable : Any];
        // For Debug builds, use the Stage Environment. That way we can test things before
        // release
#if DEBUG
        opts = [
            P2UXAppCreator_Opt_Env: P2UXAppCreator_Opt_Env_Stage
        ]
#endif
        
        P2UXAppCreator.createApplication(withKey: appKey, opts: opts, resources: resources, delegate: self)
		return true
    }
...
```

## Android
In `MainActivity.java`:
``` Java

public class MainActivity extends P2UXAppFragmentActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        mAppKey = "0006.6ad72f39bc0b6181f522b51595e0280d2c1505bd7862c702d500744ae4d705fd";
        if (savedInstanceState == null) {
            savedInstanceState = new Bundle();
        }

		// Define the resources for both Phone and Tablet
		// for this case, both are using static resources in sub directories named
		// "phone" and "tablet"
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // Phone form factor
        HashMap<String, Object> formFactor = new HashMap<>();
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_FormFactor, 0);
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_Type, P2UXAppTypes.P2UXApp_PackageType_Static);
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_Package, "phone");
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_Update, P2UXAppTypes.P2UXApp_PackageUpdate_Auto);
        resources.add(formFactor);
        // Tablet form factor
        formFactor = new HashMap<>();
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_FormFactor, 1);
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_Type, P2UXAppTypes.P2UXApp_PackageType_Static);
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_Package, "tablet");
        formFactor.put(P2UXAppTypes.P2UXApp_PackageAttrib_Update, P2UXAppTypes.P2UXApp_PackageUpdate_Auto);
        resources.add(formFactor);
        setResources(resources);

		// Setup options
        savedInstanceState.putString(P2UXAppCreator.P2UXAppCreator_Opt_Env, P2UXAppCreator.P2UXAppCreator_Opt_Env_Recent);

        // Setup where data sources are persisted to
        savedInstanceState.putString(P2UXAppCreator.P2UXAppCreator_Opt_PersistData, P2UXAppCreator.P2UXAppCreator_Opt_PersistData_Internal);
        super.onCreate(savedInstanceState);
        applyTranslucentStatusBar();
    }
}
```

<div style="text-align:right"><sub><sup>Last Updated: Dec. 3, 2017<sup><sub></div>
