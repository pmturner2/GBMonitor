//
//  GBSceneMonitor.h
//  Words2
//

#ifndef GBSceneMonitor_h
#define GBSceneMonitor_h

#include <string>

namespace cocos2d {
namespace network {
class SIOClient;
} // namespace network
} // namespaec cocos2d

namespace Monitor {
class GBSIODelegate;

class SceneMonitor {
public:
    ///  Connect to server at address with socket.io delegate
    ///
    ///  @param address  e.g. "10.0.0.4:3000" for ipv4 address 10.0.0.4 and port 3000
    ///  @param delegate socket.io delegate. Will be retained and stored in _sioDelegate
    void Connect(const std::string &address, GBSIODelegate *delegate);

    ///  Connect to server at address with socket.io delegate _sioDelegate
    ///
    ///  @param address e.g. "10.0.0.4:3000" for ipv4 address 10.0.0.4 and port 3000
    void Connect(const std::string &address);

    ///  Disconnect from server
    void Disconnect();

    ///  Sets and retains the socket.io delegate
    ///
    ///  @param delegate socket.io delegate
    void SetSIODelegate(GBSIODelegate *delegate);

    ///  Schedules a callback to execute each frame that logs the scene.
    void Start();

    ///  Unschedules a callback if registered.
    void Stop();

private:
    ///  Socket.io delegate for handling messages from the GBMonitor server
    GBSIODelegate *_sioDelegate = nullptr;

    ///  Client that manages the connection with socket.io server
    cocos2d::network::SIOClient *_sioClient = nullptr;
};
    
} // namespace Monitor
#endif
