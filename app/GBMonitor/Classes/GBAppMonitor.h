//
//  GBAppMonitor.h
//  Words2
//

#ifndef GBAPPMONITOR_H_
#define GBAPPMONITOR_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "GBMonitorBase.h"

namespace GBMonitor {
class GBSIODelegate;
class SceneMonitor;

class AppMonitor : public MonitorBase {
public:
    void Initialize();
    void Shutdown();

    ///  Connect to server at address with socket.io delegate _sioDelegate
    ///
    ///  @param address e.g. "10.0.0.4:3000" for ipv4 address 10.0.0.4 and port 3000
    void Connect(const std::string &address);

    ///  Disconnect from server
    void Disconnect();

    ///  Adds a monitor to the app, indexed by name
    ///
    ///  @param name    name of the monitor to add
    ///  @param monitor monitor
    void Add(const std::string &name, std::shared_ptr<MonitorBase>);

    ///  Removes a monitor with specified name
    ///
    ///  @param name name of monitor to remove
    void Remove(const std::string &name);

    ///  Start sending data to the server. Specific logic is implemented by each registered monitor.
    void Start() override;

    ///  Stop sending data to the server.
    void Stop() override;

private:
    enum class State {
        kDisconnected,
        kStarted,
        kStopped
    } _state = State::kDisconnected;

    ///  Collection of monitors currently attached.
    std::unordered_map<std::string, std::shared_ptr<MonitorBase>> _monitors;
};

} // namespace Monitor

#endif
