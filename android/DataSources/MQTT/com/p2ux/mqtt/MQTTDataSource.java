package com.p2ux.mqtt;

import android.content.Context;
import android.util.Log;

import com.p2ux.core.components.datasource.P2UXDataSchema;
import com.p2ux.core.components.datasource.P2UXDataSource;
import com.p2ux.core.components.datasource.P2UXSourcedData;
import com.p2ux.core.utils.P2UXLog;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONObject;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;

/**
 * Created by P2UX on 12/22/2017.
 * Copyright © 2017 P2UX, Inc. All rights reserved.
 *
 * Adaptation of the Paho MQTT Library as a P2UXDataSource
 * This allows for use of MQTT data sources within P2UX based applications
 *
 * UI controls that are linked to a MQTTDataSource Request will subscribe to the topic associated with
 * the request referenced in the link. If the topic is already subscribed, the subscription will
 * be ignored. Any messages that are received for the specified topic will be sent to associated links
 * and cached in memory during the lifetime of the application. Persistence of the data is controlled by
 * the request attribute 'persist' which will cache the last value seend until the next message from
 * the MQTT server arrives.
 *
 * A schema is required for the data associated with a topic. The P2UXDataSource will convert any received data
 * into the appropriate type based on the Schema. If the data received doesn't match the schema, it will
 * be rejected.
 */

public class MQTTDataSource extends P2UXDataSource implements MqttCallback
{
    private static final String TAG = "MQTTDataSource";
    public enum ConnectionStatus {
        DISCONNECTED,
        CONNECTED,
        CONNECTING,
        ERROR
    }

    // Specific attributes for an individual Request.
    private static final String RequestAttribute_Topic = "topic";
    private static final String RequestAttribute_Retain = "retain";

    private static final String InitAttribute_Host = "host";
    private static final String InitAttribute_Port = "port";
    private static final String InitAttribute_Timeout = "timeout";

    private static final String RequestCommand_Subscribe = "subscribe";
    private static final String RequestCommand_Unsubscribe = "unsubscribe";
    private static final String RequestCommand_Publish = "publish";

    private static final int QOS_AtMostOnce = 0;
    private static final int QOS_AtLeastOnce = 1;
    private static final int QOS_ExactlyOnce = 2;

    private static final String ClientID = "P2UXMQTTClient";

    private String mHost = "";
    private int mPort = 1883;
    private int mTimeout = 30;
    private int mQOS = QOS_AtLeastOnce;
    private Boolean mCleanSession = true;
    private int mKeepAliveTime = 60;
    private ConnectionStatus mStatus = ConnectionStatus.DISCONNECTED;
    private String mServerURI = "";
    private final HashMap<String, MQTTDataRequest> mSubscriptions;
    private final ArrayList<MQTTDataRequest> mPendingSubscriptions;
    private final ArrayList<MQTTPublishRequest> mPendingPublishes;
    private MqttAndroidClient mMQTTClient;
    private Boolean mRunningPending = false;

    private WeakReference<UXDataSourceDelegate> mDelegate;

    public MQTTDataSource(Context context, String sourceId, String sourceType, JSONObject dataSpec)
    {
        super(context, sourceId, sourceType, dataSpec);
        mSubscriptions = new HashMap<>();
        mPendingSubscriptions = new ArrayList<>();
        mPendingPublishes = new ArrayList<>();

        try
        {
            if (mInitData != null)
            {
                mHost = mInitData.optString(InitAttribute_Host);
                mPort = mInitData.optInt(InitAttribute_Port, mPort);
                mTimeout = mInitData.optInt(InitAttribute_Timeout, mTimeout);
                // check for authentication here
                JSONObject authReq = mInitData.optJSONObject(SpecKey_Auth);
                if (authReq != null)
                {
                    // TODO
                    // Initialize auth information here
                    mServerURI = "ssl://" + mHost + ":" + mPort;
                }
                else
                {
                    mServerURI = "tcp://" + mHost + ":" + mPort;
                }

            }
            else
            {
                // No information for server connection
                mServerURI = null;
            }
        }
        catch (Exception e)
        {
            // error setting up auth
            mServerURI = null;
        }
    }
    
    public static void setup()
    {
        P2UXDataSourceManager.instance().registerDataSourceClass("mqtt", MQTTDataSource.class);
    }

    @Override
    protected RequestResult handleRequest(String name, JSONObject dataObj, JSONObject args, UXDataUpdateCallback callback)
    {
        final P2UXDataSource self = this;
        String operator = RequestCommand_Subscribe;
        if (args != null)
        {
            JSONObject cmd = args.optJSONObject(SpecKey_UCmd);
            if (cmd != null)
            {
                operator = cmd.optString(Command_Attrib_Operator, RequestCommand_Subscribe);
            }
        }

        RequestResult response = null;

        // Get the details of the request so we can use it to respond
        // out of scope later when the async service requests complete
        String topic = dataObj.optString(RequestAttribute_Topic);
        JSONObject schemaObj = null;
        JSONObject requestSpec = mRequests.optJSONObject(name);
        if (requestSpec != null)
        {
            schemaObj = requestSpec.optJSONObject(SpecKey_Schema);
        }
        Boolean persist = false;
        long validSecs = 0;
        long refresh = 0;
        JSONObject requestObj = requestSpec.optJSONObject(SpecKey_Request);
        if (requestObj != null)
        {
            JSONObject attrs = requestObj.optJSONObject(SpecKey_Attributes);
            if (attrs != null)
            {
                persist = attrs.optBoolean(SpecKey_Persist, persist);
                validSecs = attrs.optLong(SpecKey_Validity, validSecs);
                refresh = attrs.optLong(SpecKey_Refresh, refresh);
            }
        }

        if (topic != null)
        {
            synchronized (this)
            {
                switch(operator)
                {
                    case RequestCommand_Subscribe:
                    {
                        // Generic requests are treated as subscriptions to the request,
                        // If the request topic already has a subscription, this will just return success
                        // immediately
                        response = new RequestResult(ResultCode.Success, null);
                        // see if there is already a subscription to the topic, if so, just return success, don't have to do anything...
                        // the data should come from the persisted data or the next incoming message
                        if (mSubscriptions.get(topic) == null)
                        {
                            MQTTDataRequest subscription = new MQTTDataRequest(topic, mQOS, getId(), name);
                            subscription.setRequestInstanceData(dataObj, schemaObj, args, persist, refresh, validSecs);
                            if (mStatus == ConnectionStatus.DISCONNECTED)
                            {
                                // Not connected, stash the request as pending and don't return a response
                                // this will get cleared later when the subscription either fails or succeeds after
                                // the connection is established (or fails)
                                response = null;
                                if (!mRunningPending)
                                {
                                    synchronized (mPendingSubscriptions)
                                    {
                                        mPendingSubscriptions.add(subscription);
                                    }
                                    assertConnected();
                                }
                            }
                            else
                            {
                                if (mSubscriptions.get(topic) == null)
                                {
                                    // subscribe
                                    try
                                    {
                                        // Assume the subscription will succeed, data will show up later
                                        // if it fails, subsequent calls will try to subscribe again
                                        addNewSubscription(subscription);
                                    } catch (Exception e)
                                    {
                                        // handle error here
                                        response = new RequestResult(ResultCode.Failed, null);
                                    }
                                }
                            }
                        }
                    }
                    break;
                    case RequestCommand_Unsubscribe:
                    {
                        // unsubscribe to the topic for this request
                        if (mSubscriptions.get(topic) != null)
                        {
                            response = new RequestResult(ResultCode.Success, null);
                            removeSubscription(topic);
                        }
                        else
                        {
                            P2UXLog.e(TAG, "Unsubscribe request made for a subscription that doesn't exist");
                            response = new RequestResult(ResultCode.InvalidParams, null);
                        }
                    }
                    break;
                    case RequestCommand_Publish:
                    {
                        // explicitly publish new data for the topic referenced by this request
                        try
                        {
                            // Have to have valid data before trying to publish
                            Object data = args.opt(SpecKey_UData);
                            if (data != null)
                            {
                                // validate data first...
                                data = fromJSON(data);
                                P2UXDataSchema schema = createSchema(schemaObj);
                                if (schema.validateData(data, name))
                                {
                                    Boolean retain = dataObj.optBoolean(RequestAttribute_Retain, true);

                                    MQTTPublishRequest publishRequest = new MQTTPublishRequest(topic, data, retain, mQOS, getId(), name);
                                    publishRequest.setRequestInstanceData(dataObj, schemaObj, args, persist, refresh, validSecs);

                                    if (mStatus == ConnectionStatus.DISCONNECTED)
                                    {
                                        if (!mRunningPending)
                                        {
                                            synchronized (mPendingPublishes)
                                            {
                                                mPendingPublishes.add(publishRequest);
                                            }
                                            assertConnected();
                                        }
                                    }
                                    else
                                    {
                                        publishData(publishRequest);
                                    }
                                }
                                else
                                {
                                    P2UXLog.e(TAG, "Publish data request made with data that doesn't match the data schema");
                                    response = new RequestResult(ResultCode.ValidationError, null);
                                }
                            }
                            else
                            {
                                P2UXLog.e(TAG, "Publish data request made without any data");
                                response = new RequestResult(ResultCode.InvalidParams, null);
                            }
                        } catch (Exception e)
                        {
                            P2UXLog.e(TAG, "Exception while trying to publish data: " + e.getLocalizedMessage());
                            response = new RequestResult(ResultCode.Failed, null);
                        }
                    }
                    break;
                    default:
                    {
                        // unknown command
                        response = new RequestResult(ResultCode.Failed, null);
                    }
                    break;
                }
            }

        }
        else
        {
            // Specification is missing a topic, so it cannot continue
            P2UXLog.e(TAG, "No topic available for request");
            response = new RequestResult(ResultCode.InvalidParams, null);
        }
        return response;
    }

    @Override
    protected void dispatchRequest(final String request, final JSONObject requestObj, final JSONObject schemaSpec,
                                   final JSONObject args, final boolean persist, final long refresh, final long validSecs,
                                   final UXDataSourceDelegate delegate)
    {
        // Cache a handle to the delegate, will be used to handle responses to completed requests
        mDelegate = new WeakReference<UXDataSourceDelegate>(delegate);
        super.dispatchRequest(request, requestObj, schemaSpec, args, persist, refresh, validSecs, delegate);
    }

    @Override
    public void setResults(P2UXSourcedData results, String request, JSONObject args, boolean persist)
    {
        super.setResults(results, request, args, persist);

        JSONObject requestSpec = mRequests.optJSONObject(request);
        if (requestSpec == null)
        {
            return;
        }
        JSONObject requestObj = requestSpec.optJSONObject(SpecKey_Request);
        if (requestObj == null)
        {
            return;
        }
        String topic = requestObj.optString(RequestAttribute_Topic);
        if (topic != null)
        {
            Object data = results.valueForKeyPath(".");
            if (data != null)
            {
                Boolean retain = requestObj.optBoolean(RequestAttribute_Retain, true);
                MQTTPublishRequest publishRequest = new MQTTPublishRequest(topic, data, retain, mQOS, getId(), request);

                if (mStatus == ConnectionStatus.CONNECTED)
                {
                    publishData(publishRequest);
                }
                else
                {
                    // create a pending publish
                    if (!mRunningPending)
                    {
                        synchronized (mPendingPublishes)
                        {
                            mPendingPublishes.add(publishRequest);
                        }
                        assertConnected();
                    }
                }
            }
            else
            {
                P2UXLog.e(TAG, "setResults called with no data. Unable to update requested data link");
            }
        }
    }

    private void assertConnected()
    {
        // make sure we have valid URI
        if (mServerURI == null) return;

        synchronized (this)
        {
            if (mStatus != ConnectionStatus.CONNECTED && mStatus != ConnectionStatus.CONNECTING)
            {
                if (mMQTTClient == null)
                {
                    mMQTTClient = new MqttAndroidClient(mContext, mServerURI, ClientID);
                    mMQTTClient.setCallback(this);
                }
                P2UXLog.v(TAG, "Attempting to connect to MQTT server: " + mServerURI);

                final MQTTActionListener callback = new MQTTActionListener(mContext,
                        MQTTActionListener.Action.CONNECT, this, null);

                MqttConnectOptions connOpts = new MqttConnectOptions();
                connOpts.setConnectionTimeout(mTimeout);
                connOpts.setCleanSession(mCleanSession);
                connOpts.setKeepAliveInterval(mKeepAliveTime);
                setConnectionStatus(ConnectionStatus.CONNECTING);
                try
                {
                    mMQTTClient.connect(connOpts, mContext, callback);
                }
                catch(Exception e)
                {
                    // error connecting
                }
            }
        }
    }

    public void setConnectionStatus(ConnectionStatus status)
    {
        Boolean set = false;
        synchronized (this)
        {
            if (mStatus != status)
            {
                mStatus = status;
                set = true;
            }
        }
        if (set)
        {
            if (status == ConnectionStatus.CONNECTED)
            {
                triggerPendingRequests();
            }
            else if (status == ConnectionStatus.DISCONNECTED || status == ConnectionStatus.ERROR)
            {
                clearPendingRequests();
                mStatus = ConnectionStatus.DISCONNECTED;
            }
        }
    }

    public void publishComplete(Boolean success, Object context)
    {
        if (context == null) return;

        MQTTPublishRequest publishRequest = (MQTTPublishRequest)context;

        // if the request parameters are part of the publish data, then resolve the pending request with the result of the
        // publish, otherwise this was just a link update w/o a pending request
        if (publishRequest.hasRequestData())
        {
            UXDataSourceDelegate delegate = null;
            if (mDelegate != null)
                delegate = mDelegate.get();
            RequestResult response = new RequestResult((success?ResultCode.Success:ResultCode.Failed), null);
            handleResponse(response, publishRequest.getRequest(), publishRequest.getRequestSpec(), publishRequest.getSchemaSpec(),
                    publishRequest.getArgs(), publishRequest.getPersist(), publishRequest.getRefresh(),
                    publishRequest.getValidSec(), delegate);
        }
    }

    public void subscribeComplete(Boolean success, MQTTDataRequest subscription)
    {
        if (subscription == null) return;
        if (!success)
        {
            // remove from list of subscriptions since its not valid
            synchronized (mSubscriptions)
            {
                mSubscriptions.remove(subscription.getTopic());
            }
            // TODO: determine if we should put it into the pending list or retry?
        }
    }

    public void unsubscribe (String topic) throws MqttException
    {
        if (topic == null) return;

        if(mSubscriptions.containsKey(topic))
        {
            this.mMQTTClient.unsubscribe(topic);
            mSubscriptions.remove(topic);
        }
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception
    {
        MQTTDataRequest subscription = mSubscriptions.get(topic);
        if (subscription != null && mDelegate != null)
        {
            // Data comes in string form, will get converted
            UXDataSourceDelegate delegate = null;
            if (mDelegate != null)
                delegate = mDelegate.get();

            // Forward the data on as a response to the P2UXDataSource layer, the
            // data will get cached (and possibly persisted) as well as forwarded on
            // to any UI bound this this request.
            String messageData = new String(message.getPayload());
            RequestResult response = new RequestResult(ResultCode.Success, messageData);
            handleResponse(response, subscription.getRequest(), subscription.getRequestSpec(), subscription.getSchemaSpec(),
                    subscription.getArgs(), subscription.getPersist(), subscription.getRefresh(),
                    subscription.getValidSec(), delegate);

        }
    }

    public void deliveryComplete(IMqttDeliveryToken token) {
        // Do nothing
    }

    public void connectionLost(Throwable cause)
    {
        if (cause != null)
        {
            setConnectionStatus(ConnectionStatus.DISCONNECTED);
            // When disconnected, move any current subscriptions back into the pending state,
            // this will allow them to be recreated on the next successful connection
            synchronized (mPendingSubscriptions)
            {
                Set<String> keys = mSubscriptions.keySet();
                for (String key : keys)
                {
                    MQTTDataRequest subscription = mSubscriptions.get(key);
                    mPendingSubscriptions.add(subscription);
                }
                mSubscriptions.clear();
            }
            Log.d("MQTT Data Source", "Connection Lost: " + cause.getMessage());
        }
    }

    private void publishData(MQTTPublishRequest publishRequest)
    {
        if (publishRequest == null) return;

        // Publish the data associated with the publishRequest
        String topic = publishRequest.getTopic();
        Object data = publishRequest.getData();
        boolean retain = publishRequest.getRetain();
        try
        {
            String msg = toJSON(data).toString();
            // convert data into a string to publish.
            String[] actionArgs = new String[2];
            actionArgs[0] = msg;
            actionArgs[1] = topic;
            final MQTTActionListener callback = new MQTTActionListener(mContext,
                    MQTTActionListener.Action.PUBLISH, this, null, actionArgs);
            mMQTTClient.publish(topic, msg.getBytes(), mQOS, retain, publishRequest, callback);
        }
        catch(Exception ex)
        {
            P2UXLog.e(TAG, "Exception occurred during publish: " + ex.getMessage());
            // if this was a request based publish, then notify the request that it failed
            if (publishRequest.hasRequestData())
            {
                cancelPendingRequest(publishRequest);
            }
        }
    }

    private void removeSubscription(String topic)
    {
        if (topic == null) return;

        synchronized (mSubscriptions)
        {
            if (mSubscriptions.containsKey(topic))
            {
                mSubscriptions.remove(topic);
                try
                {
                    mMQTTClient.unsubscribe(topic);
                }
                catch (Exception e)
                {
                    P2UXLog.e(TAG, "Error trying to unsubscribe to topic: " + topic);
                }
            }
        }
    }

    private void addNewSubscription(MQTTDataRequest subscription) throws MqttException
    {
        if (subscription == null) return;

        synchronized (mSubscriptions)
        {
            if (!mSubscriptions.containsKey(subscription.getTopic()))
            {
                try
                {
                    String[] actionArgs = new String[1];
                    actionArgs[0] = subscription.getTopic();
                    final MQTTActionListener callback = new MQTTActionListener(mContext,
                            MQTTActionListener.Action.SUBSCRIBE, this, subscription, actionArgs);
                    this.mMQTTClient.subscribe(subscription.getTopic(), subscription.getQos(), null, callback);
                    mSubscriptions.put(subscription.getTopic(), subscription);
                } catch (Exception e)
                {
                    throw new MqttException(e);
                }
            }
        }
    }

    private void clearPendingRequests()
    {
        synchronized (mPendingSubscriptions)
        {
            for (MQTTDataRequest subscription : mPendingSubscriptions)
            {
                cancelPendingRequest(subscription);
            }
            mPendingSubscriptions.clear();
        }
        synchronized (mPendingPublishes)
        {
            for (MQTTPublishRequest publishRequest : mPendingPublishes)
            {
                // notify any pending publishes that they have failed
                publishComplete(false, publishRequest);
            }
            mPendingPublishes.clear();
        }
    }

    private void cancelPendingRequest(MQTTDataRequest request)
    {
        if (request == null) return;
        // cancel the pending request by notifying the P2UXDataSource layer that the request
        // failed to complete
        UXDataSourceDelegate delegate = null;
        if (mDelegate != null)
            delegate = mDelegate.get();
        RequestResult response = new RequestResult(ResultCode.Failed, null);
        handleResponse(response, request.getRequest(), request.getRequestSpec(), request.getSchemaSpec(),
                request.getArgs(), request.getPersist(), request.getRefresh(),
                request.getValidSec(), delegate);
    }

    private void triggerPendingRequests()
    {
        // IF there is a valid connection, trigger any pending subscription and publish requests
        if (mStatus == ConnectionStatus.CONNECTED)
        {
            mRunningPending = true;
            synchronized (mPendingSubscriptions)
            {
                for (MQTTDataRequest subscription : mPendingSubscriptions )
                {
                    try
                    {
                        addNewSubscription(subscription);
                    }
                    catch(Exception e)
                    {
                        // handle error here
                    }
                }
                mPendingSubscriptions.clear();
            }
            synchronized (mPendingPublishes)
            {
                for (MQTTPublishRequest publishRequest : mPendingPublishes)
                {
                    publishData(publishRequest);
                }
                mPendingPublishes.clear();
            }
            mRunningPending = false;
        }
    }
}
