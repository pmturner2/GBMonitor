//
//  GBSIODelegate.h
//  Words2
//

#ifndef GBSIODELEGATE_H_
#define GBSIODELEGATE_H_

#include "network/SocketIO.h"

namespace GBMonitor {

/// Socket.io delegate used to interact with the GBMonitor server.
class GBSIODelegate : public cocos2d::network::SocketIO::SIODelegate {
public:
    virtual ~GBSIODelegate() {}

    void onConnect(cocos2d::network::SIOClient* client) override;
    void onMessage(cocos2d::network::SIOClient* client, const std::string& data) override;

    void onClose(cocos2d::network::SIOClient* client) override;
    void onError(cocos2d::network::SIOClient* client, const std::string& data) override;

    void fireEventToScript(cocos2d::network::SIOClient* client, const std::string& eventName,
                           const std::string& data) override;

    void OnConnected(cocos2d::network::SIOClient* client, const std::string& data);

    void OnRegistered(cocos2d::network::SIOClient*, const std::string& data);
};

} // namespace GBMonitor

#endif
