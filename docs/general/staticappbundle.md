#Static Application Bundles
-----

Application developers can choose to include their P2UX application bundle as a static resource contained in their native application binary. Providing the application bundle as a static resource removes the need for connectivity when the application runs for the first time. 

Potential issues to consider with this approach are:

* If dynamic updates are NOT allowed, updates to application bundle have to be made by updating the application binary itself to include the updated bundle. This means resubmitting your application to the respective stores which can take time to get approval.
* If dynamic updates are allowed, a new installation of the application may end up updating its content immediately on first install as the embedded application bundle may be out of date.

Currently, *Builder* doesn't provide a direct mechanism to download the bundle for inclusion in a native application. The *Coordinator* product does provide a way to use application bundles. That application will be released soon.

Check back soon for more information, or contact P2UX support.
<div style="text-align:right"><sub><sup>Last Updated: Nov. 12, 2017<sup><sub></div>
