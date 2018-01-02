# RESTful Data Sources
RESTful Data Sources are collections of data are fetched via HTTP/HTTPS. Since it follows the standard Data Source protocol, data from the repository can be used directly in UI. Data can be persisted locally when fetched from a REST service to provide an offline experience. Data persisted can also be expired based on a time validity period.

!!! note
    As of the last version of this document, Data Sources can only be defined using a JSON editor. It is assumed that the reader understands the basics of JSON structure to be able to edit the data source. A better targeted Data Source editor is being developed to make creation of Data Sources simpiler. 

## InitData Object Attributes
The `initdata` object for RESTful Data Sources includes the following attributes:

| Attribute | Type | Description |
| --- | --- | --- |
| baseurl | string | The a URL that is to provide a basis for each of the requests in this datasource. Requests that use this simply provide a path element to append to this base URL. Alternatively, each request can supply its own full requesturl attribute. |
| auth | object | The authentication method and details necessary to authorize a client for access to the requests defined in this datasource. The details of the auth object are specified according to the value of the type attribute. See [Authentication](#authentication) for more information |

## Request Object Attributes
In addition to the standard [Request Object Attributes](datasource.md#request-object-attributes) provided by Data Sources, RESTful Data Sources have a additional attributes used for data and parameters.

| Attribute | Type | Description |
| --- | --- | --- |
| attributes | object | (OPTIONAL) - An object that defines additional attributes for the request. See [Attributes Object](#attributes-object) for more information |
| method | string | This is the HTTP method to perform for this request. Valid values are "GET", "PUT", "POST", "DELETE" and "PATCH" |
| path | string | This attribute defines a path extension to add to the baseurl rather than replacing it completely with the url. This path may contain "parameterized" path elements. These elements are begin with a ":" character and will be replaced by associated values supplied in the args passed to the request. For example the path "/users/:id" would have ":id" replaced with the value from the field "id" passed in the args.<br><br>Note: `path` and `url` are mutually exclusive. |
| url | string | This attribute defines a fully qualified URL for this request rather than using the baseurl and path. This url itself may contain "parameterized" path elements. These elements are begin with a ":" character and will be replaced by associated values supplied in the args passed to the request. For example the url "https://servername/users/:user" would have ":user" replaced with the value from the field "user" passed in the args.<br><br>Note: `path` and `url` are mutually exclusive. |
| args | object | This is an args object as described in the general datasource attributes. For a RESTful datasource these are processed in a few ways after merging these with the args passed in from either the event action or data link. The args fields will first be matched against any "parameterized" path elements in the constructed request URL (see path and url above). Any remaining fields not matched in the URL path will either be used to construct query parameters for a GET or DELETE request or will be posted into the body of the request for a PUT or POST request. |
| postschema | object | This is a `schema` object as described in the general datasource specification. This schema is used to validate the type and structure of the data that will be included in the body of a POST or PUT request. |
| failFields | object | If a request results in a failure indicated by its schema this object can contain a message and a detail field that will be used to construct an error object that may be used in place of the actual data results. Each of these fields is a string value that contains a "dotted-path" to a field within the returned data. For example the message may be found in ".response.body.error.msg". | 
| errorSchema | object | If the request results in failure due to the HTTP method returning an error code. This object may contain a schema in the standard datasource schema format that will be used to validate and parse any data returned in the error result. |
| errorFields | object | If the request results in a failure due to the HTTP method returning an error code and an errorSchema is provided, this object may contain a message and a detail field that will be used to construct an error object. Each of these fields is a string value that contains a "dotted-path" to a field within the returned data. For example the detail may be found in ".error.description". |
| async | boolean | If this field is present and set to TRUE then it indicates that this request may utilize asynchronous polling for results. If the reqest results in a "partial" value as indicated by its schema then a TRUE value here will cause the request to poll for a complete result if there is an associated polling object as well. |
| polling | object | This is an object that defines how the polling for results will be performed. It is primarily controlled by a `delay` field that specifies a number of seconds between issuing subsequent poll requests for a full result. It can also have a `count` field which specifies the maximum number of poll attempts it will make. If the request continues to return partial (i.e. non-success or failure) results then the polling will continue until it reaches the count. The polling object may also contain overrides for many of the other attributes in the initial request including the `method`, `path`, `url`, `postschema`, or the `schema`. Finally, it is possible to have the `url` for polling come from the results via a `urlfield` attribute that specifies a "dotted-path" into the results. |

### Attributes Object
The `request` object can have additional attributes that control how its data is handled.

| Attribute | Type | Description |
| --- | --- | --- |
| persist | boolean | (OPTIONAL) - Whether or not data retrieved from the REST service for a request is persisted locally. If this attribute not specified, no data will be persisted and network connectivity will be required to use any data from the request. |
| validity | number | (OPTIONAL) - When data is persisted, this value indicates how long that data should be considered valid. This allows for requests to be defined where it will trigger a new request if the data no longer valid versus always fetching data from the service.<br><br>This value is the number of seconds to consider the data valid from the time it was persisted |
| refresh | number | (OPTIONAL) - This value will create a polling type of request once a the request has been triggered by an event in the client. Use this value only in the case where the client needs to poll a request on a service based on a time interval.<br><br>This value is the number of seconds to wait between requests. |
| encrypt | boolean | (OPTIONAL) - When `persist` is set to `true`, setting `encrypt` to `true` will encrypt the data persisted on the client device. |

## Authentication
The `auth` object defines the form of authentication to use (if any) when accessing the data source requests. Most attributes of the `auth` object are dependent on the type of auth. However, the following attributes are defined at the top level.

| Attribute | Type | Description |
| --- | --- | --- |
| type | string | This is a string that defines the authentication method for access to the requests defined in this datasource. The details of many of the fields in the auth object are defined by the value of the `type` attribute.  (See [Auth Types](#auth-types) for more information)|
| url | string | (OPTIONAL) - This attribute defines a full URL that will override the baseurl construction and be used directly as the URL for authentication. Certain auth types may require this element.
| path | string | (OPTIONAL) - This attribute defines a path extension that is appended to the baseURL rather than replacing it completely with the auth `url`.
| servicename | string | (OPTIONAL) - This is a string identifying the name of the service that will be used for this authorization option. It may be necessary for the client to have pre-registered with this service and to have information such as a username, password or client_id. |
| args | object | (OPTIONAL) - Defines arguments to be passed to the authentication request. These are used depending on the type of the authentication. Args can be static values or data links. Args will be used to populate values in a `loginschema` or as parameters to a `url`. |

The remainder of the attributes in the auth object are dependent on the type of auth object that is specified.

### Auth Types
These are the current authentication types supported by RESTful Data Sources. Additional types can be created by supplying a custom string for the `type` and then by creating custom subclasses of the `P2UXRestDataSource` class in client code.

| Type | Description |
| --- | --- |
| Basic | The authentication credentials are provided using the standard HTTP Basic authentication protocol specified in RFC 2617 with the credentials supplied in the HTTP “Authorization” header field. |
| Login | The authentication credentials are provided by “login” credentials. The default is username and password, but the exact format is defined via a standard REST POST request specification and can include other parameters. |
| TokenLogin | The authentication credentials are provided by "login" credentials and the result of the login is a token that can be used with request data for authentication. This is similar to OAuth, but is more freeform to accomodate specific server implementations. |
| OAuth | The authentication credentials are provided in accordance with RFC 6749 (aka the OAuth 2.0 specification). The specific type of authorization is determined by the `granttype` attribute.

### Basic Auth

| Attribute | Type | Description |
| --- | --- | --- |
| username | string | The username to be sent in the Authorization header field. If this is not present, the application will need to supply this information from another source. Typically this is defined as a data link to prompt the user. Its the responsibility of the application using the data source to respond to the request for the information by triggering UI and responding with the provided information. (See [Data Source - Requesting User Information](datasource.md)) |
| password | string | The password to be sent in the Authorization header field. If this is not present, the application will need to supply this information from another source. Typically this is defined as a data link to prompt the user. Its the responsibility of the application using the data source to respond to the request for the information by triggering UI and responding with the provided information. (See [Data Source - Requesting User Information](datasource.md)) |
| realm | string | (OPTIONAL) - The authentication “realm” to use for this server. This should contain the value that would normally be included in the WWW-Authenticate HTTP header from a web server in a 401 response message. |

**Example Basic Auth:**
```
{
  "initdata": {
    "baseurl": "http://www.testme.com",
    "auth": {
      "type": "Basic",
      "path": "login",
      "username": "thatguy",
      "password": "123Happy"
    }
  }
  ...
}
```

### Login Auth

| Attribute | Type | Description |
| --- | --- | --- |
| request | string | A standard REST request object specifying the format, method and either path or full requesturl. See REST request specification below. In most cases the request will be a “POST” method. |
| loginschema | object | The schema specification in the REST `schema` format defined below for the data required to log into the service. The login auth request may contain alias fields if the submitted data in the request has different names than the fields with semantic meaning to the application.<br><br>**Supported Attributes:**<br><table><thead><tr><th>Attribute</th><th>Type</th><th>Description</th></thead><tbody><tr><td>username</td><td>string</td><td>Tells the client which field (if any) represents the login username. If there is a “username” field in the schema this is not needed.</td></tr><tr><td>password</td><td>string</td><td>Tells the client which field (if any) represents the login password. If there is a “password” field in the schema this is not needed</td></tr></tbody></table> |
| schema | object | Contains a schema specification in the REST schema format defined below. It specifies the format of the data that is returned from the login request. |
| persist | array | (OPTIONAL) - defines data from the "schema" that should be persisted for further use in subsequent requests. The values returned from the successful login are stored encrypted and then injected into requests depending on the `use` defined for each parameter. Values that don't need to be used as part of requests don't need to be included in the persist array. If no data needs to be persisted for use, then this object can be omitted entirely.<br><br>Each item defined in the persist array corresponds to a value in the schema for the login and how that value should be used for subsequent requests. All of the specified values are required. (See [Persist Attributes](#persist-attributes))|

**Example Login Auth**
```
{
  "initdata": {
    "baseurl": "http://sampleservice.com/",
    "auth": {
      "type": "Login",
      "request": {
        "format": "JSON",
        "method": "POST",
        "path": "login"
      },
      "loginschema": {
        "type": "Object",
        "item": {
          "username": "name",
          "name": {
            "type": "String"
          },
          "password": {
            "type": "String"
          }
        }
      },
      "schema": {
        "type": "Object",
        "item": {
          "result": {
            "type": "Number"
          }
        },
        "success": {
          "result": 0
        }
      }
    }
  }
  ...
}
```

### TokenLogin Auth
TokenLogin uses standard login credentials but can handle a token returned when the login is successful. This token is applied to requests depending on the server requirements. It also can support refreshing the token if that is supported.

| Attribute | Type | Description |
| --- | --- | --- |
| refresh | object | An object that defines how to refresh the token when it expires. The format of this follows the `auth` object format with some additional parameters (See [Refresh Attributes](#refresh-attributes))
| loginschema | object | The schema specification in the REST `schema` format defined below for the data required to log into the service. The login auth request may contain alias fields if the submitted data in the request has different names than the fields with semantic meaning to the application.<br><br>**Supported Attributes:**<br><table><thead><tr><th>Attribute</th><th>Type</th><th>Description</th></thead><tbody><tr><td>username</td><td>string</td><td>Tells the client which field (if any) represents the login username. If there is a “username” field in the schema this is not needed.</td></tr><tr><td>password</td><td>string</td><td>Tells the client which field (if any) represents the login password. If there is a “password” field in the schema this is not needed</td></tr></tbody></table> |
| schema | object | Contains a schema specification in the REST schema format defined below. It specifies the format of the data that is returned from the login request. |
| persist | array | (OPTIONAL) - defines data from the "schema" that should be persisted for further use in subsequent requests. The values returned from the successful login are stored encrypted and then injected into requests depending on the `use` defined for each parameter. Values that don't need to be used as part of requests don't need to be included in the persist array. If no data needs to be persisted for use, then this object can be omitted entirely.<br><br>Each item defined in the persist array corresponds to a value in the schema for the login and how that value should be used for subsequent requests. All of the specified values are required. (See [Persist Attributes](#persist-attributes))|

**Example TokenLogin Auth**<br>
This example shows using token login authentication with a *Firebase* application. The `refresh_token` in the `refresh` object refers to cached authentication values via a link of type 16 (auth value). The `key` refers to a static value in the application named `authkey`. The args in the `refresh` object are used to populate the `loginschema` for the refresh request.
```
{
  "initdata": {
    "auth": {
      "refresh": {
        "args": {
          "key": {
            "var": "authkey",
            "type": 14
          },
          "refresh_token": {
            "var": "refreshToken",
            "type": 16
          },
          "grant_type": "refresh_token"
        },
        "persistpaths": {
          "refresh_token": "refreshToken",
          "id_token": "idToken",
          "expires_in": "expiresIn"
        },
        "loginschema": {
          "type": "Object",
          "item": {
            "grant_type": {
              "type": "String"
            },
            "refresh_token": {
              "type": "String"
            }
          }
        },
        "schema": {
          "type": "Object",
          "item": {
            "expires-in": {
              "type": "String"
            },
            "refresh_token": {
              "type": "String"
            },
            "project_id": {
              "type": "String"
            },
            "user_id": {
              "type": "String"
            },
            "id_token": {
              "type": "String"
            },
            "token_type": {
              "type": "String"
            }
          }
        },
        "url": "https://securetoken.googleapis.com/v1/token?key=:key"
      },
      "type": "TokenLogin",
      "servicename": "sample",
      "url": "https://www.googleapis.com/identitytoolkit/v3/relyingparty/verifyPassword?key=:key",
      "request": {
        "format": "JSON",
        "method": "POST"
      },
      "persist": [
        {
          "path": "appId",
          "field": "X-HTTP-AppId",
          "use": "header",
          "val": {
            "var": "authkey",
            "type": 14
          }
        },
        {
          "method": "POST",
          "field": "idToken",
          "path": "idToken",
          "use": "payload"
        },
        {
          "method": "GET",
          "field": "auth",
          "path": "idToken",
          "use": "parameter"
        },
        {
          "path": "refreshToken",
          "use": "refreshtoken"
        },
        {
          "path": "expiresIn",
          "use": "expires"
        },
        {
          "path": "idToken",
          "use": "accesstoken"
        }
      ],
      "args": {
        "key": {
          "var": "authkey",
          "type": 14
        },
        "email": {
          "var": "email",
          "type": 15
        },
        "password": {
          "var": "password",
          "type": 15
        },
        "returnSecureToken": true
      },
      "loginschema": {
        "type": "Object",
        "item": {
          "returnSecureToken": {
            "type": "Boolean"
          },
          "password": {
            "type": "String"
          },
          "email": {
            "type": "String"
          }
        }
      },
      "schema": {
        "type": "Object",
        "item": {
          "expiresIn": {
            "type": "String"
          },
          "refreshToken": {
            "type": "String"
          },
          "registered": {
            "type": "Boolean"
          },
          "idToken": {
            "type": "String"
          },
          "displayName": {
            "type": "String"
          },
          "email": {
            "type": "String"
          },
          "localId": {
            "type": "String"
          },
          "kind": {
            "type": "String"
          }
        }
      }
    },
    "baseurl": "https://sampleapp.firebaseio.com/"
  }
  ...
}
```

### OAuth Auth
The OAuth authentication mode can provide a variety of different ways to authenticate the user depending on the service requirements and client capabilities. The specific authentication mechanism is controlled by a "granttype" setting. For OAuth the auth url (or baseurl + auth path) is a mandatory attribute. The authorization is performed by submitting these parameters to the auth url in exchange for an “Authorization Token” which will be sent to the service when each of the requests are made. The URL will have query parameters appended to it by the client based on the specific granttype protocol being performed. Some of these required parameters may come from the client having to be registered with a specific service. These values will need to be embedded into the client code or obtained through application-specific business logic. 

For certain OAuth granttype values, the client will utilize an embedded web-browser as the first step in the protocol before submitting the token request directly over HTTP. When this is the case, it is usually because the client is being authenticated on behalf of a user on a third-party service (e.g Facebook). In this case, a separate code_url is used to construct the URL sent to the browser. Along with the required parameters this includes a redirect_uri where the service will redirect the browser once the user authorizes the client. This url should generally be a "localhost" url as the client will use this to retreive an Authorization Code that it will then submit to the authurl in exchange for the Authorization Token

| Attribute | Type | Description |
| --- | --- | --- |
| grant | string | One of the 3 possible “authorization grant” types supported by the OAuth 2.0 protocol.<br><br>**Possible Values:**<br><br><table><thead><tr><th>Value</th><th>Description</th></thead><tbody><tr><td>AuthCode</td><td>This indicates that the client should initiate a request for an “Authorization Code”. The client will be launch in an embedded browser with the codeurl (see below) in order to allow the user to authenticate the client to the service using their credentials. The client will append information to the URL obtained when registering for the service. This includes a CLIENT_ID and a REDIRECT_URI. In response to this request, the client will  submit the returned code to the authurl in order to redeem it for an “Authorization Token”.</td></tr><tr><td>Password</td><td>This indicated that client should directly request an “Authorization Token” by providing a username and password pair. This must be supplied by the client via a user prompt or some other internal mechanism. It will also need to have a CLIENT_ID. The client will need to complete the authurl as well in this case by appending the USERNAME, PASSWORD and CLIENT_ID parameters.</td></tr><tr><td>Client</td><td>This indicates that the client should directly request an “Authorization Token” by providing its pre-registered client credentials in the form of a CLIENT_ID and CLIENT_SECRET. This will be supplied in the HTTP Authorization header field using the “Basic” authentication format.</td></tr></tbody></table>|
| code_url | string | Used for a granttype of “AuthCode’. it specifies a URL to that will be opened in an embedded web browser in order to obtain the Authorization Code. |
| redirect_uri | string | Used for a granttype of "AuthCode". This will be appended to the codeurl so the service may redirect the embedded browser and supply the Authorization Code to the client once the user has authorized it. |

#### Client ID and Secret
The client id and secret can be provided to OAuth authentication using Static Values in an Application specification via the `args` object for the auth request. This mechanism allows for a RESTful Data source to be generic in its definition and reusable by different applications as the application provides the instance values. (See Static Values for more information). 

**Example OAuth Auth**<br>
In this example, the `client_id` and `client_secret` values would be defined in the application as static values and populated by the PRL at runtime.
```
{
  "initdata": {
    "baseurl": "https://api.instagram.com/",
    "auth": {
      "servicename": "Instagram",
      "type": "OAuth",
      "granttype": "AuthCode",
      "url": "https://api.instagram.com/oauth2/token",
      "code_url": "https://api.instagram.com/oauth/authorize/",
      "redirect_uri": "http://localhost",
      "args": {
        "client_id":{
          "type":14,
          "val":"instagram_client_id"
        },
        "client_secret":{
          "type":14,
          "val":"instagram_client_secret"
        }
      }
    }
  }
  ...
}
```

### Persist Attributes
*TokenLogin* and *Login* authentication types can provide a `persist` array that defines how values returned from a successful login are to be cached securely on the client device and used in requests.

| Attribute | Type | Description |
| --- | --- | --- |
| path | string | A path to the value in the response. Supports dotted notation to navigate through object heirarchy (e.g. "results.token") |
| use | string | Describes the use of the value with regards to inclusion in requests for the data source.<br><br>**Possible Values:**<br><table><thead><tr><th>Value</th><th>Description</th></thead><tbody><tr><td>payload</td><td>the value is placed in the payload (body) of the request.</td></tr><tr><td>header</td><td>The value is included in the header of the request.</td></tr><tr><td>parameter</td><td>The value is passed as a query parameter to the request.</td></tr><tr><td>expires</td><td>The value is used to determine the expiration of any authentication</td></tr><tr><td>refreshtoken</td><td>If the authentication supports refresh tokens, this indicates which value in the data is the refreshtoken.</td></tr></tbody></table> |
| field | string | The name of the field to use for the value. When the use of the data is payload or argument, this becomes the key for the value in the key/value pair that is submitted either in the body of or the query parameters for each request. If the use is header, this is the HTTP header field to use when submitting the value with each request |
| method | string | (OPTIONAL) - the specific method of request that the persist data should be applied to. If no method is specified, the data would be applied to any method of data request. If multiple methods are needed, they can be provided as single value separated by commas.<br><br>Possible Values:<br><table><thead><tr><th>Value</th></thead><tbody><tr><td>GET</td></tr><tr><td>POST</td></tr><tr><td>PUT</td></tr><tr><td>DELETE</td></tr></tbody></table>|

### Data Schema
All requests require a schema object that defines the structure of the data returned for the request.

This example shows a request with an array collection of "user" objects. The request is called `users` in a data source named `Service Data`.

```
{
  "name": {
    "en_US": "Service Data"
  },
  "type": "REST",
  "systemtype": "service_data",
  "requests": {
    "users": {
      "request": {
        "version": 1
      },
      "schema": {
        "type": "Array",
        "index": "ident",
        "item": {
          "type": "Object",
          "item": {
            "ident": {
              "type": "String"
            },
            "firstname": {
              "type": "String"
            },
            "lastname": {
              "type": "String"
            },
            "picture": {
              "type": "String"
            },
            "password": {
              "type": "String"
            },
            "email": {
              "type": "String"
            }
          }
        }
      }
    }
  }
}
``` 

## Examples

###Twitter using OAuth
This example specification uses Twitter's RESTful API with OAuth as the authentication type. It defines a request called `search` that takes a search parameter as an argument. In this example, the argument has a default value supplied by the spec. If the application doesn't provide the argument at runtime as part of the request, this value would be used as the default argument. For authentication, the `client_id` and `client_secret` values used for OAuth are provided by the application using *Static Values*. Since `persist` is not defined in the attributes of the request, the results of the request would not be saved on the client device for offline use.
```
{
  "name": {
    "en_US": "Twitter"
  },
  "type": "REST",
  "systemtype": "twitter",
  "initdata": {
    "auth": {
      "granttype": "Client",
      "type": "OAuth",
      "servicename": "Twitter",
      "path": "oauth2/token",
      "args": {
        "client_id": {
          "type": 14,
          "var": "twitter_client_id"
        },
        "client_secret": {
          "type": 14,
          "var": "twitter_client_secret"
        }
      }
    },
    "baseurl": "https://api.twitter.com"
  },
  "requests": {
    "search": {
      "schema": {
        "item": {
          "search_metadata": {
            "item": {
              "since_id_str": {
                "type": "String"
              },
              "max_id_str": {
                "type": "String"
              },
              "query": {
                "type": "String"
              },
              "completed_in": {
                "type": "Number"
              },
              "count": {
                "type": "Number"
              },
              "next_results": {
                "type": "String"
              },
              "refresh_uri": {
                "type": "String"
              },
              "since_id": {
                "type": "Number"
              },
              "max_id": {
                "type": "Number"
              }
            },
            "type": "Object"
          },
          "statuses": {
            "item": {
              "item": {
                "user": {
                  "item": {
                    "screen_name": {
                      "type": "String"
                    },
                    "friends_count": {
                      "type": "Number"
                    },
                    "statuses_count": {
                      "type": "Number"
                    },
                    "description": {
                      "type": "String"
                    },
                    "time_zone": {
                      "type": "String"
                    },
                    "followers_count": {
                      "type": "Number"
                    },
                    "lang": {
                      "type": "String"
                    },
                    "utc_offset": {
                      "type": "Number"
                    },
                    "url": {
                      "type": "String"
                    },
                    "id_str": {
                      "type": "String"
                    },
                    "id": {
                      "type": "Number"
                    },
                    "location": {
                      "type": "String"
                    },
                    "created_at": {
                      "type": "String"
                    },
                    "profile_image_url_https": {
                      "type": "String"
                    },
                    "profile_image_url": {
                      "type": "String"
                    },
                    "name": {
                      "type": "String"
                    }
                  },
                  "type": "Object"
                },
                "in_reply_to_screen_name": {
                  "type": "String"
                },
                "in_reply_to_status_id_str": {
                  "type": "String"
                },
                "in_reply_to_status_id": {
                  "type": "Number"
                },
                "retweet_count": {
                  "type": "Number"
                },
                "retweeted": {
                  "type": "Boolean"
                },
                "id_str": {
                  "type": "String"
                },
                "id": {
                  "type": "Number"
                },
                "text": {
                  "type": "String"
                },
                "created_at": {
                  "type": "String"
                },
                "truncated": {
                  "type": "Boolean"
                },
                "favorited": {
                  "type": "Boolean"
                },
                "coordinates": {
                  "item": {
                    "type": {
                      "type": "String"
                    },
                    "coordinates": {
                      "item": {
                        "type": "Number"
                      },
                      "type": "Array"
                    }
                  },
                  "type": "Object"
                }
              },
              "type": "Object"
            },
            "type": "Array"
          }
        },
        "type": "Object"
      },
      "request": {
        "args": {
          "q": "#beef"
        },
        "format": "JSON",
        "method": "GET",
        "path": "1.1/search/tweets.json",
        "version":1
      }
    }
  }
}
```

!!! note Creating a New Data Source
    For Steps to create a new Data Source in *Builder*, refer to the [Data Sources](datasource#creating-data-sources) documentation

<div style="text-align:right"><sub><sup>Last Updated: Dec. 27, 2017<sup><sub></div>
