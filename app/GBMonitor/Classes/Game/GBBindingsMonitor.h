//
//  GBBindingsMonitor.h
//  Words2
//

#ifndef GBBINDINGSMONITOR_H_
#define GBBINDINGSMONITOR_H_

#include "GBMonitorBase.h"

#include "json/document.h"

namespace Engine {
    class BindingSource;
    class BindingTarget;
}

namespace GBMonitor {

/// Monitor to show all active bindingSource -> bindingTarget
class BindingsMonitor : public MonitorBase {
public:
    BindingsMonitor();
    ~BindingsMonitor() override;
    
    ///  Schedules a callback to execute each frame that logs the scene.
    void Start() override;

    ///  Unschedules a callback if registered.
    void Stop() override;

    void AddBinding(const Engine::BindingSource *source, const Engine::BindingTarget *target);
    void RemoveBinding(const Engine::BindingSource *source, const Engine::BindingTarget *target);

private:
    void Log();

    rapidjson::Document _json;
    rapidjson::Value _bindingsJson{rapidjson::kObjectType};

    bool _started = false;
};

}  // namespace GBMonitor

#endif
