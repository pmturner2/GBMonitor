//
//  GBSIODelegate.cpp
//  Words2
//

#ifdef GB_NEW_STACK

#include "GBSIODelegate.h"

#include "network/SocketIO.h"

#include "json/document.h"

using namespace cocos2d;
using namespace cocos2d::network;
using namespace GBMonitor;

static int unique_id;

void GBSIODelegate::onConnect(SIOClient* client) { CCLOG("PETER SIODelegate onConnect fired"); }

void GBSIODelegate::onMessage(SIOClient* client, const std::string& data) {
    CC_UNUSED_PARAM(client);
    CCLOG("PETER SIODelegate onMessage fired with data: %s", data.c_str());
}

void GBSIODelegate::onClose(SIOClient* client) {}

void GBSIODelegate::onError(SIOClient* client, const std::string& data) {}

void GBSIODelegate::fireEventToScript(SIOClient* client, const std::string& eventName,
                                      const std::string& data) {
    CC_UNUSED_PARAM(client);
    CCLOG("PETER SIODelegate event '%s' fired with data: %s", eventName.c_str(), data.c_str());
}

void GBSIODelegate::OnConnected(SIOClient* client, const std::string& data) {
    CCLOG("PETER OnConnected with data: %s", data.c_str());

    // After socket.io is connected, tell the server to register this app.
    client->emit("registerApp", "");
}

void GBSIODelegate::OnRegistered(SIOClient*, const std::string& data) {
    // After server receives our "registerApp" message, notify us with our unique_id. This is used
    // to connect monitors to view this app's activity.
    // TODO: details for connecting a monitor.

    CCLOG("PETER OnRegistered with data: %s", data.c_str());
    rapidjson::Document document;
    document.Parse(data.c_str());
    unique_id = document["uniqueId"].GetInt();
    CCLOG("PETER Parsed json for unique_id = %d", unique_id);
}

#endif  // GB_NEW_STACK
