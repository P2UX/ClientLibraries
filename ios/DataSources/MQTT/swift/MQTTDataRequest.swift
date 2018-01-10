//
//  MQTTDataRequest.swift
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 1/9/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

import MQTTFramework
import P2UXApp

class MQTTDataRequest {
    var request : String
    var requestSpec : NSDictionary?
    var schemaSpec : NSDictionary?
    var args : NSDictionary?
    var topic : String
    var qos : MQTTQosLevel
    var callback : P2UXCallbackResponse?
    
    var hasRequestData : Bool {
        get {
            return requestSpec != nil
        }
    }
    
    init(withTopic topic: String, qos: MQTTQosLevel, request: String, callback: @escaping P2UXCallbackResponse) {
        self.topic = topic
        self.qos = qos
        self.request = request
        self.callback = callback
    }

    init(withTopic topic: String, qos: MQTTQosLevel, request: String) {
        self.topic = topic
        self.qos = qos
        self.request = request
    }

    func setRequestInstanceData(requestSpec: NSDictionary, schemaSpec: NSDictionary, args: NSDictionary) {
        self.requestSpec = requestSpec
        self.schemaSpec = schemaSpec
        self.args = args
    }
    
}
