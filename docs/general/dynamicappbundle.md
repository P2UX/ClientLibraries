#Dynamic Application Bundle

-----

The default behavior of the *P2UX Rendering Library* (PRL) is to use the application bundle created by *Builder* as a dynamic resource. In this mode, there are no resources bundled into the native application itself. Instead, the first time the application is run, the *PRL* will contact *Builder* and retrieve the application bundle for the specified environment (see [Application Environments](envandpublish)) before the application starts. This experience allows for application content to be completely dynamic and easily updatable without need to have a heavy native application binary.

!!! note "Connectivity Required for First Run"
    Using dynamic application bundles requires connectivity for the first run of the application so that the application can retrieve its content. In the case where the user runs an application for the first time without connectivity, the *PRL* will show an error dialog prompting the user to connect to the internet. If connectivity for first run is an issue, use [Static Application Bundles](staticappbundle)
    
## Customizing the Startup Screen

When using application bundles as live dynamic resources, the *PRL* will show a loading screen the first time your application runs. The default loading screen shows a P2UX logo, a message and the loading progress. This screen can be customized by the developer to use customized assets and messaging to match your application.

!!! note
    After the first time the application bundle is successfully downloaded and unpacked, all updates of the application bundle are performed in the background allowing the user to continue using the application. (See [Application Bundle Updates](appbundleupdates) for more options on updating) It's only the first run of the application where the Startup Screen is shown.

### iOS/tvOS

The startup screen can be customized by providing an instance of the `P2UXAppLoadingViewController` class via the `createLoadingViewController` method of the `P2UXAppCreatorDelegate` protocol. Typically this is handled via your `AppDelegate` instance by having it implement the `P2UXAppCreatorDelegate` protocol. Since the class `P2UXAppLoadingViewController` is just an instance of UIViewController, the contents can be customized using any standard iOS/tvOS APIs. 

There are helper methods in the `P2UXAppLoadingViewController` class that provide access to the default components of the view.

!!! note 
	For Xcode projects created using the P2UX *Coordinator* application for macOS, the `AppDelegate` class is already setup as the `P2UXAppCreatorDelegate` delegate to `P2UXAppCreator`.

#### Objective-C
In `AppDelegate.m`
``` Objective-C
...
// Return an instance of a P2UXAppLoadingViewController derived class
- (P2UXAppLoadingViewController*) createLoadingViewController {
	return [[CustomLoadingViewController alloc] init];
}
```

In the custom class (in this example `CustomLoadingViewController`):
``` Objective-C
...
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // for this example, we'll just override the logo shown in the loading dialog.
    // load the UIImage instance of the custom logo from the native app bundle.
    UIImage* logo = [UIImage imageNamed:@"customlogo"];
    // set this image as the current logo
    // this is a method of the base class P2UXAppLoadingViewController
    [self setLogoImage: logo];    
}
```

#### Swift
In `AppDelegate.swift`
``` Swift
...
// Return an instance of a P2UXAppLoadingViewController derived class
func createLoadingViewController() -> P2UXAppLoadingViewController {
        return CustomLoadingViewController()
}
```

In the custom class (in this example `CustomLoadingViewController`):
``` Swift
...
override func viewDidLoad() {
    super.viewDidLoad()

 	// for this example, we'll just override the logo shown in the loading dialog
    // load the UIImage instance of the customlogo from the native app bundle.
    let image = UIImage.init(named: "customlogo");
  	// set this image as the current logo
    // this is a method of the base class P2UXAppLoadingViewController
    self.setLogoImage(image);
}
```

### Android
In the `MainActivity.java`
``` Java
// Coming Soon
```

## Customizing the Error Screen
Customizing the error screen shown when the *PRL* is unable to load the application content is done much in the same way as customizing the loading screen.

### iOS/tvOS
The error screen can be customized by providing an instance of the `P2UXAppErrorViewController` class via the `createErrorViewController` method of the `P2UXAppCreatorDelegate` protocol. Typically this is handled via your `AppDelegate` instance by having it implement the `P2UXAppCreatorDelegate` protocol. Since the class `P2UXAppErrorViewController` is just an instance of UIViewController, the contents can be customized using any standard iOS/tvOS APIs. 

There are helper methods in the `P2UXAppErrorViewController` class that provide access to the default components of the view.
#### Objective-C
In `AppDelegate.m`
``` Objective-C
...
// Return an instance of a P2UXAppLoadingViewController derived class
- (P2UXAppLoadingViewController*) createErrorViewController {
	return [[CustomErrorViewController alloc] init];
}
```

In the custom class (in this example `CustomErrorViewController`):
``` Objective-C
...
- (void)viewDidLoad {
    [super viewDidLoad];
    
 	// for this example, we'll just change the background color
 	[self.view setBackgroundColor:[UIColor blueColor]];
}
```

#### Swift
In `AppDelegate.swift`
``` Swift
...
// Return an instance of a P2UXAppErrorViewController derived class
func createErrorViewController() -> P2UXAppErrorViewController {
        return CustomErrorViewController()
}
```

In the custom class (in this example `CustomErrorViewController`):
``` Swift
...
override func viewDidLoad() {
    super.viewDidLoad()

 	// for this example, we'll just change the background color
 	self.view.backgroundColor = UIColor.blue;
}
```

### Android
```
Coming soon
```
<div style="text-align:right"><sub><sup>Last Updated: Nov. 12, 2017<sup><sub></div>
