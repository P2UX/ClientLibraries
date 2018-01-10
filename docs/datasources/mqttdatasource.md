# MQTT Data Sources
MQTT Data Sources allow P2UX application to subscribe to and publish data with MQTT brokers. Since it follows the standard Data Source protocol, data from the repository can be used directly in UI. Each request in an MQTT Data Source is associated with a MQTT topic and requires a schema that defines the structure of the data stored in the topic. Data can be either primitive data types (String, Boolean, Integer, Float) or JSON. Data linked to controls that are defined to update the link (controls such as a Text Input) will automatically publish changes to the data with the broker. Manual changes are accomplished by triggering a "Request Data" action with a command type of "update". 

!!! note
    As of the last version of this document, Data Sources can only be defined using a JSON editor. It is assumed that the reader understands the basics of JSON structure to be able to edit the data source. A better targeted Data Source editor is being developed to make creation of Data Sources simpiler. 

!!! note "Player Support"
    Since there are different libraries that implement MQTT broker integration for Android and iOS, support for MQTT data sources is not included by the P2UX Player application. Support for use of MQTT data sources is provided using a custom Data Source using that encapsulates a third party library for MQTT. A native application must be created for each platform supported by the application and include the additional code as well as the third party libraries to interface between P2UX and MQTT. The [P2UX Client Libraries GitHub](https://github.com/P2UX/ClientLibraries) project contains the sources and instructions for integrating support for MQTT in your native application for both Android and iOS.

!!! note "Authentication"
    The current version of MQTT data sources don't yet support authentication with MQTT brokers. This document will be updated with authentication information once its supported.

## InitData Object Attributes
The `initdata` object defines the MQTT broker information for MQTT Data Sources and includes the following attributes:

| Attribute | Type | Description |
| --- | --- | --- |
| host | string | The host URL/IP address of the MQTT Broker for this datasource. |
| port | number | The port number on the host broker. If not provided, the default is 1883 |
| timeout | number | The number of seconds to wait before a timeout of a connection request. If not provided, the default is 30 seconds |

## Request Object Attributes
In addition to the basic [Request Object Attributes](datasource.md#request-object-attributes) provided by Data Sources, MQTT Data Sources have additional attributes for each request specific to the subscription and publishing of MQTT data. 

| Attribute | Type | Description |
| --- | --- | --- |
| topic | String | The name of the topic associated with the request. |
| retain | Boolean | Whether or not the messages published for the specified topic are retained by the MQTT Broker. Support of this feature is specific to the MQTT broker implementation. |

### Attributes Object
The `request` object can have additional `attributes` that control how its data is handled.

| Attribute | Type | Description |
| --- | --- | --- |
| persist | boolean | (OPTIONAL) - Whether or not data retrieved from the MQTT broker for a request is persisted locally. If this attribute not specified, no data will be persisted and network connectivity will be required to use any data from the request. |
| encrypt | boolean | (OPTIONAL) - When `persist` is set to `true`, setting `encrypt` to `true` will encrypt the data persisted on the client device. |

## Data Schema
All requests require a schema object that defines the structure of the data referenced by the 'topic'. 

This example shows a request with an object of "temperature" data. The request is called `temp` in a data source named `home_data`.

```
{
  "name": {
    "en_US": "Home Data"
  },
  "type": "MQTT",
  "systemtype": "home_data",
  "requests": {
    "temperature": {
      "request": {
        "topic": "home/temp",
        "retain": true
      },
      "schema": {
        "type": "Object",
        "item": {
          "current":{
            "type":"Number"
          },
          "set":{
            "type":"Number"
          }
        }
      }
    }
  }
}
```

!!! note "Creating a New Data Source"
    For Steps to create a new Data Source in *Builder*, refer to the [Data Sources](datasource#creating-data-sources) documentation. Select ==Custom Data Source== for the data source type and set the ==Custom Type== to `MQTT`

<div style="text-align:right"><sub><sup>Last Updated: Jan. 8, 2018<sup><sub></div>
