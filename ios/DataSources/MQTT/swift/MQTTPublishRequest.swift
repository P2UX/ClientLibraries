//
//  MQTTPublishRequest.swift
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 1/9/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

import MQTTFramework
import P2UXApp

class MQTTPublishRequest: MQTTDataRequest {

    var data : Any?
    var retainData : Bool
    
    init(withTopic topic: String, qos: MQTTQosLevel, request: String, callback: @escaping P2UXCallbackResponse, data: Any, retainData: Bool ) {
        self.data = data
        self.retainData = retainData
        super.init(withTopic: topic, qos: qos, request: request, callback: callback)
    }

    init(withTopic topic: String, qos: MQTTQosLevel, request: String, data: Any, retainData: Bool ) {
        self.data = data
        self.retainData = retainData
        super.init(withTopic: topic, qos: qos, request: request)
    }
}
