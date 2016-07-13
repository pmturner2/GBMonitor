//
//  GBMonitorBase.h
//  Words2
//

#ifndef GBMONITORBASE_H_
#define GBMONITORBASE_H_

#include <memory>
#include <string>

#include "GBSIODelegate.h"

namespace cocos2d {
namespace network {
class SIOClient;
}  // namespace network
}  // namespaec cocos2d

namespace GBMonitor {

class MonitorBase {
public:
    virtual ~MonitorBase();

    ///  Sets the socket.io delegate
    ///
    ///  @param delegate socket.io delegate
    void SetSIODelegate(std::shared_ptr<GBSIODelegate> delegate);

    ///  Sets and retains the socket.io client
    ///
    ///  @param client socket.io client. Handles communication with the server
    void SetSIOClient(cocos2d::network::SIOClient *client);

    ///  Schedules a callback to execute each frame that logs the scene.
    virtual void Start() {}

    ///  Unschedules a callback if registered.
    virtual void Stop() {}

protected:
    ///  Socket.io delegate for handling messages from the GBMonitor server
    std::shared_ptr<GBSIODelegate> _sioDelegate;

    ///  Client that manages the connection with socket.io server
    cocos2d::network::SIOClient *_sioClient = nullptr;
};

}  // namespace GBMonitor

#endif
