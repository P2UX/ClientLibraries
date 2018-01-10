//
//  MQTTDataSource.swift
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 1/9/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

import P2UXApp
import MQTTFramework

class MQTTDataSource: P2UXDataSource, MQTTSessionDelegate {
    private static let DataSourceType = "MQTT";
    private let Attrib_Host:String = "host";
    private let Attrib_Port:String = "port";
    private let Attrib_Timeout:String = "timeout";

    private let RequestCommand_Subscribe:String = "subscribe"
    private let RequestCommand_Unsubscribe:String = "unsubscribe"
    private let RequestCommand_Publish:String = "publish"

    private let RequestAttribute_Topic:String = "topic"
    private let RequestAttribute_Retain:String = "retain"
    private let Command_Operator:String = "operator"
    
    enum MQTTConnectionState: Int {
        case Disconnected = 0,
        Connecting,
        Connected,
        Error
    }
    
    private var _auth : P2UXDataAuth!
    private var _port : UInt32 = 1883
    private var _host : String?
    private var _qos : MQTTQosLevel = MQTTQosLevel.atLeastOnce
    private var _transport : MQTTCFSocketTransport!
    private var _session : MQTTSession!
    private var _timeout : TimeInterval = 30
    private var _pendingSubscriptions = [MQTTDataRequest]()
    private var _pendingPublishes = [MQTTPublishRequest]()
    private var _inprogressPublishes = [UInt16: MQTTPublishRequest]()
    private var _subscriptions = [String: MQTTDataRequest]()
    private var _runningPending : Bool = false
    private var _state : MQTTConnectionState = MQTTConnectionState.Disconnected
    
    // thread safety
    fileprivate let stateQueue = DispatchQueue(label: "com.p2ux.MQTTDataSource.stateQueue", attributes: .concurrent)
    fileprivate let subscriptionQueue = DispatchQueue(label: "com.p2ux.MQTTDataSource.subscriptionQueue", attributes: .concurrent)
    fileprivate let pendingSubscriptionQueue = DispatchQueue(label: "com.p2ux.MQTTDataSource.pendingSubscriptionQueue", attributes: .concurrent)
    fileprivate let pendingPublishQueue = DispatchQueue(label: "com.p2ux.MQTTDataSource.publishQueue", attributes: .concurrent)
    fileprivate let inprogressPublishQueue = DispatchQueue(label: "com.p2ux.MQTTDataSource.inprogressPublishQueue", attributes: .concurrent)

    class func setup() {
        P2UXDataSourceManager.instance().registerDataSourceClass(MQTTDataSource.self, forType: DataSourceType);
    }
    
    override init!(id sourceId: String!, type sourceType: String!, andSpec dataSpec: [AnyHashable : Any]!) {
        super.init(id: sourceId, type: sourceType, andSpec: dataSpec);
        if let initdata = self.value(forKey: "_initdata") as? [String: Any] {
            _host = initdata[Attrib_Host] as? String
            if let port : UInt32 = initdata[Attrib_Port] as? UInt32 {
                _port = port
            }
            if let timeout : TimeInterval = initdata[Attrib_Timeout] as? TimeInterval {
                _timeout = timeout
            }
        }
    }
    
    override func handleRequest(_ name: String!, withObject requestObj: [AnyHashable : Any]!, args: [AnyHashable : Any]!, andCallback callback: P2UXCallbackResponse!) {
        
        // Read the command operator if present,
        // default is to subscribe if the command is not supplied
        var requestOperator:String = RequestCommand_Subscribe
        if let argsval = args as? [String: Any] {
            if let cmd = argsval[P2UXSpecKey_UCmd] as? [String: Any] {
                if let op = cmd[Command_Operator] as? String {
                    requestOperator = op
                }
            }
        }
        
        let topic = requestObj[AnyHashable(RequestAttribute_Topic)] as? String
        var schemaObj : [String: Any]?
        var requestSpec : [String: Any]?
        if let request = self.value(forKey: "_requests") as? [String: Any] {
            requestSpec = request[name] as? [String: Any]
            schemaObj = requestSpec?[P2UXSpecKey_Schema] as? [String: Any]
        }
        
/*        var persist : Bool = false
        var validSec : Int = 0
        var refresh : Int = 0
        if let requestDetail = requestSpec?[P2UXSpecKey_Attributes] as? [String: Any] {
            if let persistval = requestDetail[P2UXSpecKey_Persist] as? Bool {
                persist = persistval
            }
            if let validsecval = requestDetail[P2UXSpecKey_Validity] as? Int {
                validSec = validsecval
            }
            if let refreshval = requestDetail[P2UXSpecKey_Refresh] as? Int {
                refresh = refreshval
            }
        } */
        
        var response : Int = -1
        if topic != nil {
            stateQueue.sync(flags: .barrier) {
                if requestOperator == RequestCommand_Subscribe {
                    response = P2UXDataRequestResult.requestResult_Success.rawValue
                    
                    if self._subscriptions[name] == nil {
                        let subscription : MQTTDataRequest = MQTTDataRequest(withTopic: topic!, qos: self._qos, request: name, callback: callback)
                        subscription.setRequestInstanceData(requestSpec: requestSpec! as NSDictionary, schemaSpec: schemaObj! as NSDictionary, args: args! as NSDictionary)
                        
                        if self._state == MQTTConnectionState.Disconnected {
                            response = -1
                            if !self._runningPending {
                                pendingSubscriptionQueue.sync(flags: .barrier) {
                                    _pendingSubscriptions.append(subscription)
                                }
                                DispatchQueue.global(qos: .userInitiated).async {
                                    self.assertConnected()
                                }
                            } else {
                                NSLog("MQTTDataSource: Error trying to create a pending request while its already executing them")
                                response = P2UXDataRequestResult.requestResult_Success.rawValue
                            }
                        } else {
                            self.addNewSubscription(subscription)
                        }
                    }
                } else if requestOperator == RequestCommand_Unsubscribe {
                    removeSubscription(topic)
                } else if requestOperator == RequestCommand_Publish {
                    response = -1;
                    if let data = args[P2UXSpecKey_UData] {
                        let schema :P2UXDataSchema = self.createSchema(withSpec: schemaObj);
                        if schema.validateData(data, atContext: name) {
                            var retainData = true
                            if let retainVal = requestObj[RequestAttribute_Retain] as? Bool {
                                retainData = retainVal
                            }
                            
                            let publishRequest:MQTTPublishRequest = MQTTPublishRequest(withTopic: topic!, qos: _qos, request: name, callback: callback, data: data, retainData: retainData);
                            publishRequest.setRequestInstanceData(requestSpec: requestObj! as NSDictionary, schemaSpec: schemaObj! as NSDictionary, args: args! as NSDictionary)
                            
                            if self._state == MQTTConnectionState.Disconnected {
                                if !self._runningPending {
                                    pendingPublishQueue.sync(flags: .barrier) {
                                        _pendingPublishes.append(publishRequest)
                                    }
                                    DispatchQueue.global(qos: .userInitiated).async {
                                        self.assertConnected()
                                    }
                                } else {
                                    NSLog("MQTTDataSource: Error trying to create a pending request while its already executing them")
                                    response = P2UXDataRequestResult.requestResult_Success.rawValue
                                }
                            } else {
                                publishData(publishRequest)
                            }
                        } else {
                            NSLog("MQTTDataSource: Publish data request made with data that doesn't match the data schema");
                            response = P2UXDataRequestResult.requestResult_ValidationError.rawValue
                        }
                    } else {
                        NSLog("MQTTDataSource: Publish data request made without any data");
                        response = P2UXDataRequestResult.requestResult_InvalidParams.rawValue
                    }
                } else {
                    // invalid command
                    response = P2UXDataRequestResult.requestResult_Failed.rawValue
                }
            }
        } else {
            // Specification is missing a topic, so it cannot continue
            NSLog("MQTTDataSource: No topic available for request");
            response = P2UXDataRequestResult.requestResult_InvalidParams.rawValue
        }
        
        if response != -1 {
            callback(P2UXDataRequestResult(rawValue:response)!, nil)
        }
        
    }
    
    override func setResults(_ results: P2UXSourcedData!, forRequest request: String!, withArgs args: [AnyHashable : Any]!, andPersist persist: Bool) {
        super.setResults(results, forRequest: request, withArgs: args, andPersist: persist)
        
        var requestSpec : [String: Any]?
        var requestObj: [String: Any]?
        if let requestItems = self.value(forKey: "_requests") as? [String: Any] {
            requestSpec = requestItems[request] as? [String: Any]
            if requestSpec != nil {
                requestObj = requestSpec![P2UXSpecKey_Request] as? [String: Any]
            }
        }
        
        if requestObj == nil {
            return;
        }
        
        if let topic:String = requestObj![RequestAttribute_Topic] as? String {
            if let value : Any = results.value(forKeyPath: ".") as? [String: Any] {
                var retainData : Bool = true;
                if let retainVal : Bool = requestObj![RequestAttribute_Retain] as? Bool {
                    retainData = retainVal
                }
                let publishRequest: MQTTPublishRequest = MQTTPublishRequest(withTopic: topic, qos: self._qos, request: request, data: value, retainData: retainData)
                stateQueue.sync (flags: .barrier) {
                    if _state == MQTTConnectionState.Disconnected {
                        if !_runningPending {
                            self.pendingPublishQueue.sync(flags: .barrier) {
                                self._pendingPublishes.append(publishRequest)
                            }
                            DispatchQueue.global(qos: .userInitiated).async {
                                self.assertConnected()
                            }
                        } else {
                            NSLog("MQTTDataSource: Error trying to create a pending publish while its already executing them");
                        }
                    } else {
                        // Already connected, so just publish the request
                        publishData(publishRequest)
                    }
                }
            } else {
                NSLog("MQTTDataSource: setResults called with no data. Unable to update requested data link");
            }

        }
    }
    
    func assertConnected() {
        stateQueue.sync(flags: .barrier) {
            if self._state == MQTTConnectionState.Disconnected {
                if self._session == nil {
                    if self._transport == nil && self._host != nil {
                        self._transport = MQTTCFSocketTransport()
                        self._transport.host = self._host
                        self._transport.port = self._port
                    }
                    self._session = MQTTSession()
                    self._session.transport = self._transport
                    self._session.delegate = self;
                }
                NSLog("MQTTDataSource: Requesting Connection to : %@ on port: %d", self._host ?? "unknown host", self._port)
                self._state = MQTTConnectionState.Connecting
                DispatchQueue.global(qos: .userInitiated).async {
                    self._session.connectAndWaitTimeout(self._timeout);
                }
            }
        }
    }

    func triggerPendingRequests()
    {
        if _state == MQTTConnectionState.Connected {
            _runningPending = true;
            // add all pending subscriptions
            pendingSubscriptionQueue.sync(flags: .barrier) {
                for subscription in self._pendingSubscriptions {
                    self.addNewSubscription(subscription)
                }
                self._pendingSubscriptions.removeAll()
            }
            _runningPending = false;
        }
    }
    
    func addNewSubscription(_ subscription: MQTTDataRequest!) {
        subscriptionQueue.sync(flags: .barrier) {
            self._subscriptions[subscription.topic] = subscription
        }
        
        _session.subscribe(toTopic: subscription.topic, at: subscription.qos) { (error: Error?, qoss : [NSNumber]?) in
            if error != nil {
                self.subscriptionQueue.sync(flags: .barrier) {
                    _ = self._subscriptions.removeValue(forKey: subscription.topic)
                }
                NSLog("MQTTDataSource: Subscription failed %@", error!.localizedDescription)
            } else {
                NSLog("MQTTDataSource: Subscription sucessfull!")
            }
        }
    }
    
    func removeSubscription(_ topic: String!) {
        self.subscriptionQueue.sync(flags: .barrier) {
            _ = self._subscriptions.removeValue(forKey: topic)
        }
    }
    
    func publishData(_ publishRequest: MQTTPublishRequest!) {
        // Publish the data associated with the publish Request
        if publishRequest.data != nil {
            if let data : Data = dataFromValue(publishRequest.data) {
                DispatchQueue.global().async {
                    let msgId:UInt16 = self._session.publishData(data, onTopic: publishRequest.topic, retain: publishRequest.retainData, qos: publishRequest.qos)
                    if msgId == 0 {
                        // the message was either dropped or the QOS is 0 in which case we won't know if it succeeds.
                        // Don't keep the publishRequest around at this point
                        // as we'll never get a callback via the delegate that it succeeded or not
                        if publishRequest.callback != nil {
                            publishRequest.callback!(((publishRequest.qos == MQTTQosLevel.atMostOnce) ? P2UXDataRequestResult.requestResult_Success : P2UXDataRequestResult.requestResult_Failed), nil);
                        }
                    } else {
                        // stash the publish till it can be resolved later as success or failure
                        self.inprogressPublishQueue.sync(flags: .barrier) {
                            self._inprogressPublishes[msgId] = publishRequest
                        }
                    }
                }
            }
        } else {
            // for this case, we just roll with local data since there is no explicit request
            // model for MQTT. Whatever we have is whatever
            NSLog("MQTTDataSource: Trying to publish new data for MQTT that is nil... topic: %@", publishRequest.topic);
            if publishRequest.hasRequestData {
                cancelPendingRequest(publishRequest)
            }
        }
    }

    func clearPendingRequests() {
        pendingSubscriptionQueue.sync(flags: .barrier) {
            for subscription in self._pendingSubscriptions {
                self.cancelPendingRequest(subscription)
            }
            self._pendingSubscriptions.removeAll()
        }
        pendingPublishQueue.sync(flags: .barrier) {
            for publishRequest in self._pendingPublishes {
                self.cancelPendingRequest(publishRequest)
            }
            self._pendingPublishes.removeAll()
        }
    }
    
    func cancelPendingRequest(_ request :MQTTDataRequest!) {
        // cancel the pending request by notifying the P2UXDataSource layer that the request
        // failed to complete
        if request.callback != nil {
            request.callback!(P2UXDataRequestResult.requestResult_Failed, nil);
        }
    }
    
    func valueFromData(_ data: Data?) -> Any? {
        if data == nil {
            return nil;
        }
        do {
            let value = try JSONSerialization.jsonObject(with: data!, options: JSONSerialization.ReadingOptions.mutableContainers)
            return value;
        } catch {
            // not valid json, convert to string
        }
        let value : String? = String(data: data!, encoding: .utf8)
        return value
    }
    
    func dataFromValue(_ value:Any! ) -> Data?
    {
        var data: Data?
        if value is NSDictionary || value is NSArray {
            do {
                data = try JSONSerialization.data(withJSONObject: value)
            } catch {
                NSLog("MQTTDataSource: Unable to turn convert from JSON to Data")
            }
        } else {
            let result:String = String(describing: value)
            data = result.data(using: .utf8)
        }
        return data
    }
    
    // MQTTSessionDelegate methods
    func newMessage(_ session: MQTTSession!, data: Data!, onTopic topic: String!, qos: MQTTQosLevel, retained: Bool, mid: UInt32) {
        DispatchQueue.global().async {
            // handle in background
            if let value = self.valueFromData(data) {
                if let subscription = self._subscriptions[topic]{
                    if subscription.callback != nil {
                        subscription.callback!(P2UXDataRequestResult.requestResult_Success, value)
                    }
                }
            }
        }
    }
    
    func messageDelivered(_ session: MQTTSession!, msgID: UInt16, topic: String!, data: Data!, qos: MQTTQosLevel, retainFlag: Bool) {
        inprogressPublishQueue.sync(flags: .barrier) {
            if let publishRequest : MQTTPublishRequest = self._inprogressPublishes[msgID] {
                if publishRequest.hasRequestData {
                    publishRequest.callback!(P2UXDataRequestResult.requestResult_Success, nil);
                }
                _ = _inprogressPublishes.removeValue(forKey: msgID);
            }
        }
    }
    
    func connected(_ session: MQTTSession!) {
        if _session == session {
            NSLog("MQTTDataSource: Connected")
            stateQueue.sync(flags: .barrier) {
                _state = MQTTConnectionState.Connected
            }
            triggerPendingRequests()
        }
    }
    
    func connectionError(_ session: MQTTSession!, error: Error!) {
        if _session == session {
            NSLog("MQTTDataSource: Connection error: %@", error.localizedDescription);
            stateQueue.sync(flags: .barrier) {
                _state = MQTTConnectionState.Disconnected
            }

        }
    }
    
    func connectionRefused(_ session: MQTTSession!, error: Error!) {
        if _session == session {
            NSLog("MQTTDataSource: Connection refused: %@", error.localizedDescription);
            stateQueue.sync(flags: .barrier) {
                _state = MQTTConnectionState.Disconnected
            }
        }
    }
}
