package com.p2ux.mqtt;

import org.json.JSONObject;

/**
 * Created by P2UX on 12/22/2017.
 * Copyright © 2017 P2UX, Inc. All rights reserved.
 */

public class MQTTPublishRequest  extends MQTTDataRequest
{
    private final Object mData;
    private final boolean mRetain;

    public MQTTPublishRequest(String topic, Object data, boolean retain, int qos, String clientHandle, final String request)
    {
        super(topic, qos, clientHandle, request);
        mData = data;
        mRetain = retain;
    }

    public Object getData()
    {
        return mData;
    }

    public boolean getRetain()
    {
        return mRetain;
    }
}