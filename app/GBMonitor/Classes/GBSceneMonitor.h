//
//  GBSceneMonitor.h
//  Words2
//

#ifndef GBSceneMonitor_h
#define GBSceneMonitor_h

#include <string>

#include "GBMonitorBase.h"

namespace GBMonitor {

class SceneMonitor : public MonitorBase {
public:
    ///  Schedules a callback to execute each frame that logs the scene.
    void Start() override;

    ///  Unschedules a callback if registered.
    void Stop() override;

private:
    // Long poll interval now (every 4 seconds) due to renderer being overloaded on the monitor
    // web client.
    // TODO: make this a variable and update when renderer is fixed.
    static constexpr float _pollingInterval = 0.25f;//4.0f;
};
    
} // namespace GBMonitor

#endif
