//
//  GBMonitorBase.cpp
//  Words2
//

#ifdef GB_NEW_STACK

#include "GBMonitorBase.h"

#include "core/inc/debug/debug.h"

using namespace GBMonitor;

MonitorBase::~MonitorBase() {
    CC_SAFE_RELEASE_NULL(_sioClient);
}

void MonitorBase::SetSIODelegate(std::shared_ptr<GBSIODelegate> delegate) {
    _sioDelegate = delegate;
}

void MonitorBase::SetSIOClient(cocos2d::network::SIOClient *client) {
    CC_SAFE_RELEASE_NULL(_sioClient);
    _sioClient = client;
    CC_SAFE_RETAIN(_sioClient);
}

#endif // GB_NEW_STACK
