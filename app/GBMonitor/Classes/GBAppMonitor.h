//
//  GBAppMonitor.h
//  Words2
//

#ifndef GBAPPMONITOR_H_
#define GBAPPMONITOR_H_

namespace Monitor {
class GBSIODelegate;
class SceneMonitor;

class AppMonitor {
public:
    void Initialize();
    void Shutdown();

    void Start();
    void Stop();
private:
    GBSIODelegate *_sioDelegate = nullptr;
    SceneMonitor *_sceneMonitor = nullptr;
};

} // namespace Monitor

#endif
