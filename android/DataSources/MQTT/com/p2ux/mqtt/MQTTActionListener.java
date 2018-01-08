package com.p2ux.mqtt;

import android.content.Context;

import com.p2ux.core.utils.P2UXLog;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
/**
 * Created by P2UX on 12/22/2017.
 * Copyright © 2017 P2UX, Inc. All rights reserved.
 */

public class MQTTActionListener implements IMqttActionListener
{
    enum Action {
        CONNECT,
        DISCONNECT,
        SUBSCRIBE,
        PUBLISH
    }
    private final static String TAG = "MQTTActionListener";
    private final Action mAction;
    private final String[] mAdditionalArgs;
    private final MQTTDataSource mDataSource;
    private final String mClientHandle;
    private final Context mContext;
    private final MQTTDataRequest mRequest;

    public MQTTActionListener(final Context context, final Action action,
                          final MQTTDataSource dataSource, final MQTTDataRequest request, String... additionalArgs) {
        this.mContext = context;
        this.mAction = action;
        this.mDataSource = dataSource;
        this.mClientHandle = dataSource.getId();
        this.mAdditionalArgs = additionalArgs;
        this.mRequest = request;
    }

    /**
     * The action associated with this listener has been successful.
     *
     * @param asyncActionToken This argument is not used
     */
    @Override
    public void onSuccess(IMqttToken asyncActionToken) {
        switch (mAction) {
            case CONNECT:
                connect();
                break;
            case DISCONNECT:
                disconnect();
                break;
            case SUBSCRIBE:
                subscribe();
                break;
            case PUBLISH:
                publish(asyncActionToken.getUserContext());
                break;
        }

    }

    /**
     * A publish action has been successfully completed, update connection
     * object associated with the client this action belongs to, then notify the
     * user of success
     */
    private void publish(Object context)
    {
        mDataSource.publishComplete(true, context);
    }

    /**
     * A addNewSubscription action has been successfully completed, update the connection
     * object associated with the client this action belongs to and then notify
     * the user of success
     */
    private void subscribe() {
        mDataSource.subscribeComplete(true, mRequest);
    }

    /**
     * A disconnection action has been successfully completed, update the
     * connection object associated with the client this action belongs to and
     * then notify the user of success.
     */
    private void disconnect() {
        mDataSource.setConnectionStatus(MQTTDataSource.ConnectionStatus.DISCONNECTED);
    }

    /**
     * A connection action has been successfully completed, update the
     * connection object associated with the client this action belongs to and
     * then notify the user of success.
     */
    private void connect() {
        mDataSource.setConnectionStatus(MQTTDataSource.ConnectionStatus.CONNECTED);
    }

    /**
     * The action associated with the object was a failure
     *
     * @param token     This argument is not used
     * @param exception The exception which indicates why the action failed
     */
    @Override
    public void onFailure(IMqttToken token, Throwable exception) {
        switch (mAction) {
            case CONNECT:
                connect(exception);
                break;
            case DISCONNECT:
                disconnect(exception);
                break;
            case SUBSCRIBE:
                subscribe(exception);
                break;
            case PUBLISH:
                publish(exception, token.getUserContext());
                break;
        }

    }

    /**
     * A publish action was unsuccessful, notify user and update client history
     *
     * @param exception This argument is not used
     */
    private void publish(Throwable exception, Object context) {
        mDataSource.publishComplete(false, context);
    }

    /**
     * A addNewSubscription action was unsuccessful, notify user and update client history
     *
     * @param exception This argument is not used
     */
    private void subscribe(Throwable exception) {
        mDataSource.subscribeComplete(false, mRequest);
    }

    /**
     * A disconnect action was unsuccessful, notify user and update client history
     *
     * @param exception This argument is not used
     */
    private void disconnect(Throwable exception) {
        mDataSource.setConnectionStatus(MQTTDataSource.ConnectionStatus.DISCONNECTED);
    }

    /**
     * A connect action was unsuccessful, notify the user and update client history
     *
     * @param exception This argument is not used
     */
    private void connect(Throwable exception) {
        mDataSource.setConnectionStatus(MQTTDataSource.ConnectionStatus.ERROR);
        P2UXLog.e(TAG, "Connection failed with error: " + exception.getLocalizedMessage());
    }
}
