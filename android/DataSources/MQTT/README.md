## P2UX Paho MQTT Data Source
This is an adaptation of the Paho MQTT Library as a P2UXDataSource. This allows for use of MQTT data sources within P2UX based applications. Like all P2UX Data Sources, there needs to be an associated Data Source defined in Builder for your application that defines the requests (topics) and the data schemas that go along with the data. See the [P2UX developer site](http://developer.p2ux.com) for details on how to create an MQTT Data Source in Builder.

UI controls that are linked to a MQTTDataSource Request will subscribe to the topic associated with the request referenced in the data link for the control. If the topic is already subscribed, the subscription will be ignored. Any messages that are received for the specified topic will be sent to associated links and cached in memory during the lifetime of the application. 

Persistence of the data from a MQTT topic is controlled by the request attribute 'persist' which will cache the last value seen until the next message from the MQTT server arrives.
 
A schema is required for the data associated with a topic. The P2UXDataSource will convert any received data into the appropriate type based on the Schema. If the data received doesn't match the schema, it will be rejected.

## Project setup in Android Studio
The first requirement is to get your P2UX application setup in Android Studio. See the [P2UX Developer Site](http://developer.p2ux.com/android/quickstart/) for an explanation of how to get started. There is also a [sample application](https://github.com/P2UX/P2UX-Sample-App-Android) for Android on GitHub that can be used as a starting point for your application.

### Add Paho MQTT Service
Once you have your basic application setup and running. You'll need to get the Paho MQTT Service for Android. The library can be found [here](https://github.com/eclipse/paho.mqtt.android). Follow the directions for `Gradle` when adding the Service to your project. 
One missing step from the directions is to add the service to your `AndroidManifest.xml` file for your project. To do this:

* Edit the `AndroidManifest.xml` file in your project.
* In the `<application>...</application>` add the line:

```
        <service android:name="org.eclipse.paho.android.service.MqttService">
        </service>
```
This enables the service for your application.

Example:
```
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.p2ux.mqttmobile">

    <uses-feature android:name="android.hardware.camera" android:required="false"/>
    <uses-feature android:name="android.hardware.telephony" android:required="false" />

    <uses-permission android:name="android.permission.WAKE_LOCK" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />

    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.INTERNET" />

    <supports-screens
        android:xlargeScreens="true"
        android:largeScreens="true"
        android:normalScreens="false"
        android:smallScreens="false"
        android:requiresSmallestWidthDp="600" />

    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:roundIcon="@mipmap/ic_launcher_round"
        android:supportsRtl="true"
        android:theme="@style/AppTheme">
        <activity android:name=".MainActivity">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>

        <service android:name="org.eclipse.paho.android.service.MqttService">
        </service>
    </application>

</manifest>
```

### Add P2UX MQTTDataSource code
The last step is to add the P2UX MQTTDataSource code into your project. In AndroidStudio:

* Copy entire directory `com` and its contents from this repository into your projects `app/src/main/java` folder. (Eventually this will be hosted in a different location) and can be added as a Gradle build dependency.
* In the `MainActivity.java` file for your project, add the line from the example code below to setup the use of the `MQTTDataSource` the class for the `MQTT` data source type. The `setup()` method takes care of registering the class as the Data Source instance for MQTT.

```
public class MainActivity extends P2UXAppFragmentActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        // Register MQTT Data Source.
        MQTTDataSource.setup();

        // Setup P2UX Rendering Library
        String appKey = "xxxx.xxxx";
        HashMap<String, Object> options = new HashMap<>();
        options.put(P2UXAppCreator.P2UXAppCreator_Opt_Env, P2UXAppCreator.P2UXAppCreator_Opt_Env_Recent);
        options.put(P2UXAppCreator.P2UXAppCreator_Opt_LogLevel, P2UXLog.P2UXCoreLogFlagVerbose);
        setupApp(savedInstanceState, appKey, options, null);
    }
}
```

For documentation and help, visit [http://developer.p2ux.com](http://developer.p2ux.com)
