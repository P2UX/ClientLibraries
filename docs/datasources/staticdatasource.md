# Static Data Sources
Static Data Sources are repositories of data that are bundled into the application as a read-only resource. Since it follows the standard Data Source protocol, data from the repository can be used directly in UI and can also be parameterized using arguments for query purposes. Think of it as a catalog of data available to your application. 

!!! note
    As of the last version of this document, Data Sources can only be defined using a JSON editor. It is assumed that the reader understands the basics of JSON structure to be able to edit the data source. A better targeted Data Source editor is being developed to make creation of Data Sources simpiler. 

## Request Object Attributes
In addition to the standard [Request Object Attributes](datasource.md#request-object-attributes) provided by Data Sources, Static Data Sources have additional attributes used for data and parameters.

| Attribute | Type | Description |
| --- | --- | --- | 
| data | mixed | This item defines the actual data returned by the request when there are no parameters provided. The data type depends on the `schema` for the request. |

### Basic Data Request
This example shows a request with basic data. The request is called `info` in a data source named `Internal Data`. The `data` object in the request defines the data that will be returned when the request is called. 

This particular example request contains an object that defines information about a fictitious business. 
``` hl_lines="8"
{
  "name": {
    "en_US": "Internal Data"
  },
  "type": "STATIC",
  "systemtype": "internal_data",
  "requests": {
    "info": {
      "schema": {
        "type": "Object",
        "item": {
          "opentime": {
            "type": "String"
          },
          "closetime": {
            "type": "String"
          },
          "phonenum": {
            "type": "String"
          },
          "address": {
            "type": "String"
          },
          "state":{
            "type":"String"
          },
          "city":{
            "type":"String"
          }
        }
      },
      "request": {
        "data": {
          "address":"1234 Somewhere St.",
          "city":"Santa Clara",
          "state":"CA",
          "phonenum":"555-555-5555",
          "closetime":"7:00pm",
          "opentime":"9:30am"
        }
      }
    }
  }
}
```

### Parameterized Data Request
Static data can also include arguments to create queryable data. Parameterized data is defined in a `paramdata` object in the request. When arguments are provided to the request, the data is found by matching the arguments provided with the key value in the `paramdata`. The other required object definition is a `params` array that has objects defining each of the parameters to the request.

!!! note
    A request can have both default unparameterized as well as parameterized data.
    
#### Request Object Attributes for Parameterized Data
The following attributes are additional Request Object Attributes that are part of the `request` object definition for parameterized data:

| Attribute | Type | Description |
|---|---|---|
| params | Array | An array of objects that define each parameter available to the request. *(See Params Array Object for more Information)* |
| paramdata | Object | An object that defines each possible parameter value and its associated `data`. The `data` item beneath each parameter value follows the `data` definition for default data |
|
#### Params Array Object
Each item in the Params array for a request follow this structure:

| Attribute | Type | Description | 
| --- | --- | --- |
| name | String | The name of the parameter. This is the only required item in the specification |
| required | Boolean | (OPTIONAL) - Whether or not this parameter item is required to be provided by the requestor. If it is required, and the parameter is not supplied, the request will not complete successfully |
| default | String | (OPTIONAL) - An default value to use if the parameter is not supplied as part of the request. | 

Below is an Example of a request called `colors` in a data source named `Internal Data`. The argument to the request is named `color` which is the color name and the results are color values. `params` defines the parameter `color`. The `paramdata` object defines the results for the primary argument `color` and each possible argument value for `color` (*blue* & *red*)
```
{
  "name": {
    "en_US": "Internal Data"
  },
  "type": "STATIC",
  "systemtype": "internal_data",
  "requests": {
    "colors": {
      "schema": {
        "type": "Object",
        "item": {
          "color": {
            "type": "String"
          },
          "name":{
            "*":"String"
          }
        }
      },
      "request": {
        "data": {
          "color":"#ffffff",
          "name":{
            "en_US":"Default"
          }
        },
        "params":[
          {
            "name":"color"
           }
        ],
        "paramdata":{
          "color":{
            "blue":{
              "data":{
                "color":"#0000ff",
                "name":{
                  "en_US":"Blue",
                  "de_DE":"Blau"
                }
              }
            },
            "red":{
              "data":{
                "color":"#ff0000",
                "name":{
                  "en_US":"Red",
                  "de_DE":"Rot"
                }
              }
            }
          }
        }
      }
    }
  }
}
```

!!! note Creating a New Data Source
    For Steps to create a new Data Source in *Builder*, refer to the [Data Sources](datasource#creating-data-sources) documentation

<div style="text-align:right"><sub><sup>Last Updated: Jan. 2, 2018<sup><sub></div>
