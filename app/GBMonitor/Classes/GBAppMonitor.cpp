//
//  GBAppMonitor.cpp
//  Words2
//

#ifdef GB_NEW_STACK

#include "GBAppMonitor.h"

#include "core/inc/debug/debug.h"

#include "GBSIODelegate.h"
#include "GBSceneMonitor.h"

using namespace Monitor;

void AppMonitor::Initialize() {
    _sioDelegate = new GBSIODelegate();
    _sceneMonitor = new SceneMonitor();
    _sceneMonitor->Connect("10.0.0.4:3000", _sioDelegate);
}

void AppMonitor::Shutdown() {
    _sceneMonitor->Disconnect();
    delete _sceneMonitor;
    delete _sioDelegate;
}

void AppMonitor::Start() {
    _sceneMonitor->Start();
}

void AppMonitor::Stop() {
    _sceneMonitor->Stop();
}

#endif // GB_NEW_STACK
