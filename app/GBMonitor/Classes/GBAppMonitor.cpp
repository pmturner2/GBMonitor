//
//  GBAppMonitor.cpp
//  Words2
//

#ifdef GB_NEW_STACK

#include "GBAppMonitor.h"

#include "core/inc/debug/debug.h"

#include "network/SocketIO.h"

#include "GBSIODelegate.h"

//TEMP
#include "GBSceneMonitor.h"
#include "Game/GBBindingsMonitor.h"

using namespace cocos2d;
using namespace cocos2d::network;
using namespace GBMonitor;

// Not sure if this should be a singleton. Maybe temp.
AppMonitor *AppMonitor::_sInstance = nullptr;

AppMonitor::AppMonitor() {
    _sInstance = this;
}

AppMonitor::~AppMonitor() {
    _sInstance = nullptr;
}

void AppMonitor::Initialize() {
    SetSIODelegate(std::shared_ptr<GBSIODelegate>(new GBSIODelegate()));
    Add("scene", std::shared_ptr<MonitorBase>(new SceneMonitor()));
    Add("bindings", std::shared_ptr<MonitorBase>(new BindingsMonitor()));
}

void AppMonitor::Shutdown() {

}

void AppMonitor::Connect(const std::string &address) {
    ASSERT_NOT_NULL(_sioDelegate);
    ASSERT_NULL(_sioClient);
    _sioClient = SocketIO::connect(address, *_sioDelegate);
    CC_SAFE_RETAIN(_sioClient);
    _sioClient->on("registered", CC_CALLBACK_2(GBSIODelegate::OnRegistered, *_sioDelegate));
    _sioClient->on("connected", CC_CALLBACK_2(GBSIODelegate::OnConnected, *_sioDelegate));

    for (auto &monitor : _monitors) {
        monitor.second->SetSIODelegate(_sioDelegate);
        monitor.second->SetSIOClient(_sioClient);
    }
}

void AppMonitor::Disconnect() {
    _sioClient->disconnect();
    CC_SAFE_RELEASE_NULL(_sioClient);
}

void AppMonitor::Start() {
    for (auto &monitor : _monitors) {
        monitor.second->Start();
    }
}

void AppMonitor::Stop() {
    for (auto &monitor : _monitors) {
        monitor.second->Stop();
    }
}

void AppMonitor::Add(const std::string &name, std::shared_ptr<MonitorBase> monitor) {
    if (_state != State::kDisconnected) {
        monitor->SetSIODelegate(_sioDelegate);
        monitor->SetSIOClient(_sioClient);
    }

    if (_state == State::kStarted) {
        monitor->Start();
    }

    _monitors[name] = monitor;
}

void AppMonitor::Remove(const std::string &name) {
    _monitors.erase(name);
}

std::shared_ptr<MonitorBase> AppMonitor::Get(const std::string &name) {
    ASSERT_NOT_NULL(_sInstance);
    CORE_ASSERT(_sInstance->_monitors.find(name) != _sInstance->_monitors.end(), "Error finding monitor '%s'", name.c_str());
    return _sInstance->_monitors.at(name);
}

#endif // GB_NEW_STACK
