package com.p2ux.mqtt;

import org.json.JSONObject;

/**
 * Created by steve on 1/7/18.
 */

public class MQTTDataRequest
{
    private String mTopic;
    private int mQOS;
    private String mClientHandle;

    // P2UXDataSource Request information
    private final String mRequest;
    private JSONObject mRequestSpec;
    private JSONObject mArgs;
    private JSONObject mSchemaSpec;
    private boolean mPersist;
    private long mRefresh;
    private long mValidSecs;
    private boolean mHasRequestData = false;


    public MQTTDataRequest(String topic, int qos, String clientHandle, final String request){
        this.mTopic = topic;
        this.mQOS = qos;
        this.mRequest = request;
        this.mClientHandle = clientHandle;
    }

    // This data is relevant to the Request made that corresponds to the subscription, It allows for the
    // the subscription result to be mapped back into the P2UX request that was made and adapts the
    // the subscription the the P2UXDataSource protocol
    public void setRequestInstanceData(JSONObject requestSpec, JSONObject schemaSpec, JSONObject args, boolean persist, long refresh, long validSecs)
    {
        mHasRequestData = true;
        mRequestSpec = requestSpec;
        mSchemaSpec = schemaSpec;
        mArgs = args;
        mPersist = persist;
        mRefresh = refresh;
        mValidSecs = validSecs;
    }

    public String getTopic() {
        return mTopic;
    }

    public int getQos() {
        return mQOS;
    }

    public void setQos(int qos) {
        this.mQOS = qos;
    }

    public String getClientHandle() {
        return mClientHandle;
    }

    public String getRequest()
    {
        return mRequest;
    }

    public JSONObject getRequestSpec()
    {
        return mRequestSpec;
    }

    public JSONObject getSchemaSpec()
    {
        return mSchemaSpec;
    }

    public JSONObject getArgs()
    {
        return mArgs;
    }

    public boolean getPersist()
    {
        return mPersist;
    }

    public long getRefresh()
    {
        return mRefresh;
    }

    public long getValidSec()
    {
        return mValidSecs;
    }

    public boolean hasRequestData()
    {
        return mHasRequestData;
    }
}
