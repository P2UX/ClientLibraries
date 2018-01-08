# Local Data Sources
Local Data Sources are collections of data that store data locally on the device where the application is installed. Since it follows the standard Data Source protocol, data from the repository can be used directly in UI and can also be parameterized using arguments for query purposes. Data is stored as JSON in a file and can optionally be encrypted. Keep in mind that Local Data Sources are modifiable as they are treated like a database. If you want read-only sources of data for an application, use [Static Data Sources](staticdatasource.md) instead.

!!! note
    As of the last version of this document, Data Sources can only be defined using a JSON editor. It is assumed that the reader understands the basics of JSON structure to be able to edit the data source. A better targeted Data Source editor is being developed to make creation of Data Sources simpiler. 

## Request Object Attributes
In addition to the basic [Request Object Attributes](datasource.md#request-object-attributes) provided by Data Sources, Local Data Sources have additional attributes used for data and parameters. 

| Attribute | Type | Description |
| --- | --- | --- |
| seed | mixed | (OPTIONAL) - The initial data for the request. When an application is newly installed, this data is used until changes are made by the application. |
| maxentries | number | (OPTIONAL) - This value is used for schema types of Array and Object. It indicates the maximum entries to be stored. If a create or update operation is performed and the array is at its max then the last entry from the end or beginning of the array will be removed.

## Data Schema
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
Local Data Source requests can be seeded with data to provide an initial data set for the application. The initial data is provided within the `request` object with the key `seed`. If the initial data changes, the `version` of the request must be incremented to force clients to reseed the database with the provided data.

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
        "seed":[
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

!!! note "Creating a New Data Source"
    For Steps to create a new Data Source in *Builder*, refer to the [Data Sources](datasource#creating-data-sources) documentation

<div style="text-align:right"><sub><sup>Last Updated: Jan. 2, 2018<sup><sub></div>
