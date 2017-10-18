# Introduction

The P2UX platform offers app creators the unique ability to go beyond simple prototyping by allowing them to integrate external data into their applications. Most importantly, this feature makes it possible to include information in the app that resides outside of the user interface. This data can then be “linked” to properties of the app’s UI components to dynamically change the way the app looks or even works.

External app data can come from a number of different sources. It may be statically bundled with the app. It may be data that are read and modified locally on the device while the app is running. It may even be obtained from an external source such as a web-based service.

The P2UX platform handles data-related features through the use of Data Sources. Data Sources provide a powerful mechanism that lets app creators model, bundle and access data within their app. Using P2UX, developers and designers can actively define how data will be represented in their user interfaces. For example, it is possible to make an app that integrates data from an external news feed. When the app runs it would request the list of stories from the feed and then link them to the content of a List Component in the UI. This content could, in turn, be used to display the headline and a short summary of the news story. All of this can be accomplished directly in the P2UX Builder visual design tool using Data Sources.

P2UX includes three main types of Data Sources that can be defined and used in apps created with the design tool. These are: Static App Data for bundling data directly into the app, Local Device Databases for storing and retrieving data on the device during the running of the app and RESTful Web Services for requesting data from and submitting data to remote services. In addition, the platform employs some built-in Data Sources for integrating device data such as media information and location data. Finally, it is possible to create a custom Data Source in code that can be used in the same manner as the system supported ones. This feature makes it possible to integrate custom data, application functions or services that don’t fit into one of the existing Data Source types.

## Data Source Functionality

Most apps will use one or more different Data Sources. For example, an app may use Static data to include some pre-defined set or sets of information. (These might be text for help messages or collections of color values for supporting UI themes.) An app could include a Local Database for storing and retrieving sets of user preferences. Finally, an app might access one or more RESTful Services if the app was, for instance, designed as a front-end to an online service, or if the app creator wanted to include information from third-party services such as Facebook, Twitter or Instagram.

###Requests

*P2UX allows app creators to retrieve information from a Data Source via a “request.”* 

A request specification defines how the data from a particular Data Source inquiry will be retrieved or updated. The request can also define arguments to be supplied to the data when the request is made. Additionally, the details of the request specification allow app creators to define whether retrieved data will persist on the device and for how long. 

App creators should keep in mind that a Data Source can represent multiple collections of information. How the data are retrieved lies in the hands of the creator and in the details of the request specification. For a Static or Local Data Source, each data request maps directly to a different set of data. With a RESTful Data Source, requests are used to define queries or commands within the service.

###Schema

*Requests for data must conform to a specific structure. This structure is defined by use of a “schema.”*

Any data retrieved from a Data Source will be validated against a creator-defined schema. The P2UX Builder tool also uses the schema when linking parts of the requested data to component properties in the UI. The schema can also include filters and transformations for performing rudimentary changes to the data upon retrieval. These transformations can be simple changes in capitalization of strings or mathematical functions on numbers. They can also provide filtering or sorting of data in an Array.

##Static Data Sources

*Static Data Sources are a way to include pre-defined sets of data into your application.*

###How Static Data Works

Each request to a Static Data Source includes the complete data for that particular request. However, the data may be “parameterized,” meaning that distinct sets of data can be retrieved depending on the supplied arguments in the request. 

###Common Uses

App creators frequently use Static data to provide a complex data set that can be queried by the application and, in turn, used to show results to the end user. Common examples of this include help data or catalogs. Another typical use for Static data is as a “placeholder” model for other, more dynamic, sources of data accessed during the prototyping phase. 

##Local Data Sources

*Local Data Sources work like a small database that resides on a particular device.*

###How Local Data Works

The creator can retrieve, replace, update or delete local data for a particular request by supplying commands as arguments to the request. (In some cases, an app creator may also include initial “seed” data in a request specification.) In the case of data based on an Array schema type, the request may include arguments for inserting, appending or removing an entry within the Array of data.

Much like app variables in the P2UX platform, changes to Local Data Source requests persist across multiple application runs. However, they provide a mechanism for the addition of more rich and flexible data types.

###Common Uses

A typical use for Local data is to store user preferences within an app. These might be stored based on user selections the first time the app runs and then retrieved and used on each subsequent invocation to retain the user’s chosen settings. Another common use is for storing data while an app is offline in order to send it to a service at a later time.

##RESTful Data Sources

*RESTful Data Sources allow access to data provided by web-based services.*

###How RESTful Data Works

A request within a RESTful Data Source models one of the standard REST method types (GET, PUT, POST, DELETE).  Currently the only supported format for RESTful data on the P2UX platform is JSON. 

Depending on the method type and the request specification, arguments to a REST request may serve to construct the URL for the request. The arguments may also act as query parameters or be included as “post” data supplied in the body of the request. Numerous other options can be configured for a RESTful request, including mechanisms for retry and polling requests. 

###Common Uses
For an app that acts as the front-end UI to a service, a custom RESTful Data Source would be defined that included requests for all of the queries and operations of that service. These operations would then be linked to the app UI when, for example, submitting form-type data or displaying service information on a screen.

The Builder tool also features a number of pre-made RESTful Data Sources for use with common sites including Facebook, Twitter, Instagram and Google services.

###Authentication

*One key feature unique to RESTful Data Sources is their ability to require authentication.*

Authentication allows for the integration of services that require credentialed access. Defined at the Data Source level, authentication works across all requests. However, specific aspects of authentication may be overridden on a per-request basis. P2UX recognizes three primary authentication methods: Basic, Login and OAuth (complying to the OAuth2 authorization protocol).

####Basic Authentication

This form of authentication supports the HTTP Basic Authentication protocol. The specification can include a username and password for testing, but typically this information would be embedded in custom business logic code. Using basic authentication methods will cause the “Authorization” header to be included on each request.

####Login Authentication

Login Authentication is for traditional user/password submission that uses an HTTP POST method for login. The fields used for login, as well as those returned in the response, can be customized via a schema. The developer would then supply the credentials for the login authentication via custom business logic. 

Once login is completed, this method typically relies on session data (i.e. cookies) to retain the authorization. (An advantage of using session data is that the P2UX platform need not add anything additional to the request.)

####OAuth

Oauth is the most complex and flexible method of configuring authentication on a RESTful Data Source.

The OAuth method complies with the [OAuth2 Standard](https://oauth.net/2/). To be precise, OAuth is a collection of authentication methods each defined by a “grant type.” The P2UX platform supports grant types of AuthCode, Password and Client. Each of these uses a different underlying mechanism to obtain an Authorization Token. The token is then sent along with the individual RESTful requests. With OAuth, client credentials such as “client id” and “client secret” are generally managed via static application values or custom business logic.

#####AuthCode Grant Type

*AuthCode is the authorization grant type used primarily for third-party authorization.*

For example, the AuthCode Grant Type would be used to integrate the Facebook API into your application. In such a case, P2UX will provide an embedded web-browser for that is directed to the facebook URL as supplied in the Data Source specification. The user will then authorize access to their Facebook account. Then, the platform re-directs an authorization code back to the application for obtaining the Authorization Token.

#####Client Grant Type

*Client Grant Type uses a supplied client id and client secret to authorize application access to the service and obtain the Authorization Token.*

Generally, Client Grant Type is used to access public APIs or public portions of APIs where the application makes queries directly, rather than on behalf of any user. 

#####Password Grant Type

*Password Grant Type provides a way to directly authenticate a user via username and password to acquire the Authorization Token.*

This grant type allows authorization of the client application as well. Depending on the services involved, these credentials may be embedded in the custom business logic or they may be obtained via P2UX UI elements that are designed to fit directly into the app’s user interface.
