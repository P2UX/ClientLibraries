# Dynamic Application Bundles
The default behavior of the *P2UX Rendering Library* (PRL) is to use the application bundle created by *Builder* as a dynamic resource. In this mode, there are no resources bundled into the native application itself. Instead, the first time the application is run, the *PRL* will contact *Builder* and retrieve the application bundle for the specified environment (see [Application Environments](envandpublish)) before the application starts. This experience allows for application content to be completely dynamic and easily updatable without need to have a heavy native application binary.

!!! note "Connectivity Required for First Run"
    Using dynamic application bundles requires connectivity for the first run of the application so that the application can retrieve its content. In the case where the user runs an application for the first time without connectivity, the *PRL* will show an error dialog prompting the user to connect to the internet. If connectivity isn't required, use [Static application bundles](staticbundle)
    
## Customizing the P2UX Startup Screen

When using application bundles as live dynamic resources, the will show a loading screen the first time your application runs and doesn't have an application bundle. This loading screen shows a P2UX logo, a message and the loading progress. This screen can be customized by the developer to use customized assets and messaging.
During this time, the PRL displays a loading screen to let the user know its getting started. When using dynamic application bundles, the startup screen is shown only once to the user. It is shown the first time the application is run after the application has been installed. After this point, all updates of the application bundle are performed in the background allowing the user to continue using the application.

## Auto Update
Each time the application is restarted by the user, the *PRL* will compare the version of the application bundle it has against the version for the specified environment on *Builder*.
