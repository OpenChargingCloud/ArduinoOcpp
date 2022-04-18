#include "ArduinoOcpp_c.h"
#include "ArduinoOcpp.h"

#include <ArduinoOcpp/Debug.h>

ArduinoOcpp::OcppSocket *ocppSocket = nullptr;

extern "C" void ao_initialize(AOcppSocket *osock) {
    //OCPP_initialize("echo.websocket.events", 80, "ws://echo.websocket.events/");
    if (!osock) {
        AO_DBG_ERR("osock is null");
    }

    ocppSocket = reinterpret_cast<ArduinoOcpp::OcppSocket*>(osock);

    OCPP_initialize(*ocppSocket);
}

extern "C" void ao_loop() {
    OCPP_loop();
}

extern "C" void ao_bootNotification(const char *chargePointModel, const char *chargePointVendor, OnOcppMessage onConfirmation, OnOcppAbort onAbort, OnOcppTimeout onTimeout, OnOcppError onError) {
    bootNotification("model", "vendor");
}

#ifndef AO_RECEIVE_PAYLOAD_BUFSIZE
#define AO_RECEIVE_PAYLOAD_BUFSIZE 1024
#endif

char ao_recv_payload_buff [AO_RECEIVE_PAYLOAD_BUFSIZE] = {'\0'};

extern "C" void ao_onResetRequest(OnOcppMessage onRequest) {
    OnReceiveReqListener cb = [onRequest] (JsonObject payload) {
        auto len = serializeJson(payload, ao_recv_payload_buff, AO_RECEIVE_PAYLOAD_BUFSIZE);
        if (len <= 0) {
            AO_DBG_WARN("Received payload buffer exceeded. Continue without payload");
        }
        onRequest(len > 0 ? ao_recv_payload_buff : nullptr, len);
    };
    setOnResetReceiveReq(cb);
}

