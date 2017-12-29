# RESTful Data Sources
RESTful Data Sources are collections of data are fetched via HTTP/HTTPS. Since it follows the standard Data Source protocol, data from the repository can be used directly in UI. Data can be persisted locally when fetched from a REST service to provide an offline experience. Data persisted can also be expired based on a time validity period.

!!! note
    As of the last version of this document, Data Sources can only be defined using a JSON editor. It is assumed that the reader understands the basics of JSON structure to be able to edit the data source. A better targeted Data Source editor is being developed to make creation of Data Sources simpiler. 

## REST Specific Attributes

The `request` for REST Data Sources support the general attributes as well as the following attributes:

| Attribute | Type | Description |
| --- | --- | --- |
| persist | boolean | (OPTIONAL) - Whether or not data retrieved from the REST service for a request is persisted locally. If this attribute not specified, no data will be persisted and network connectivity will be required to use any data from the request. |


## Creating RESTful Data Sources
RESTful Data Sources follow the standard model of Data Sources in the P2UX environment (*[see Basic Data Sources for more details](datasource.md)*). Data is separated into logical requests. Each request has a schema that defines the data structure and request parameters.

To Create a new RESTful Data Source in *Builder*:

* Make sure you are in the Application or Screen/Panel Editor
* Click on the Data Source Icon from the top tool bar<br>
![Data Source Icon](../images/datasources/datasourceicon.png)
* In the ==Manage Data== dialog, click the ==+== button to create a new data source.
* From the ==Add Data Source== dialog, click the ==+== to the right of the ==Select Data Source== drop down.
* In the ==New Data Source== dialog, type in a name for the data source and then select ==Local Device Database== from the ==Select Type== drop down.
![Data Source Icon](../images/datasources/selectstaticdata.png)
* Click ==OK== to save the new data source.

The newly created data source is now part of the application.

## Creating a Request
Once a Local Data Source has been created, *requests* can be added that represent logical repositories of data. Each request requires a *schema* and a *request* object that define the request details. The *schema* defines the structure of the data. The *request* object defines the version of the request schema and any other attributes about the request (See Data Source Requests for more information).

To Create a new Request in *Builder*:

* Make sure you are in the Application or Screen/Panel Editor
* Click on the Data Source Icon from the top tool bar<br>
![Data Source Icon](../images/datasources/datasourceicon.png)
* In the ==Manage Data== dialog, click the ==edit== button overlay (or double click) on the Local Data Source from the list.

In the ==Edit JSON== dialog, there will be a JSON node named ==requests==.

Create a new key under the requests object that is the name of the request. The object associated with the key will contain the *schema* and the *request* attributes.

### Data Schema
All requests require a schema object that defines the structure of the data repository. For Local Data Sources, the top level object needs to be either an Array or an Object. If the top level object is an Object, it is used like an array where the key for the object is the unique identifier of an object instance and the value is an instance of the data Object.

This example shows a request with an array collection of "user" objects. The request is called `users` in a data source named `Internal Data`.

```
{
  "name": {
    "en_US": "Local Data"
  },
  "type": "LOCAL",
  "systemtype": "local_data",
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

### Default Data
Local Data Source requests can be seeded with data to provide an initial data set for the application. The initial data is provided within the `request` object with the key `data`. If the initial data changes, the `version` of the request must be incremented to force clients to reseed the database with the provided data.

!!! note
    Changing the seed data and incrementing the version number will only replace local data stored for the request on a client device if the 'reseed' attribute is provided in the `request` object and is set to `true`.

!!! note Version Migration
    As of version 1.1 of the P2UX Rendering Libraries *(PRL)*, Local Data Sources don't support a version migration schema. This is planned for version 1.2 of the client libraries.
    
This example shows a request with seeded data. The request is called `messages` and contains a default message.

```
{
  "name": {
    "en_US": "Local Data"
  },
  "type": "LOCAL",
  "systemtype": "local_data",
  "requests": {
    "messages": {
      "request": {
        "version": 1,
        "data":[
          {
            "ident":"ff-default",
            "message": "Welcome to the app, you'll find important messages here.",
            "from":"System",
            "type":0
          }
        ]
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
            "message": {
              "type":"String"
            },
            "timestamp": {
              "type": "Date"
            },
            "from": {
              "type": "String"
            },
            "type":{
              "type": "Number"
            }
          }
        }
      }
    }
  }
}
```    

<div style="text-align:right"><sub><sup>Last Updated: Dec. 27, 2017<sup><sub></div>
