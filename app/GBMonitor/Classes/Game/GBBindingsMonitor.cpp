//
//  GBBindingsMonitor.cpp
//  Words2
//

#ifdef GB_NEW_STACK

#include "GBBindingsMonitor.h"

#include "core/inc/debug/debug.h"

#include "Gameboard/Engine/GBBindingSource.h"
#include "Gameboard/Engine/GBBindingTarget.h"

#include "network/SocketIO.h"

#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace GBMonitor;

BindingsMonitor::BindingsMonitor() {
    _json.SetObject();

    // must pass an allocator when the object may need to allocate memory
    rapidjson::Document::AllocatorType &allocator = _json.GetAllocator();

    // TODO: should be in another object (NetworkWrapper object)
    _json.AddMember("room", "room_a", allocator);

    rapidjson::Value message(rapidjson::kObjectType);
    message.AddMember("bindings", _bindingsJson, allocator);

    _json.AddMember("message", message, allocator);
}

BindingsMonitor::~BindingsMonitor() {

}

void BindingsMonitor::Log() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    _json.Accept(writer);

    ASSERT_NOT_NULL(_sioClient);
    _sioClient->emit("log", buffer.GetString());
}

void BindingsMonitor::Start() {
    _started = true;
}

void BindingsMonitor::Stop() {
    _started = false;
}

void BindingsMonitor::AddBinding(const Engine::BindingSource *source,
                                 const Engine::BindingTarget *target) {
    // TODO
    // finalize data format for proper representation
    
    rapidjson::Value *sourceJson = nullptr;

    auto iter = _bindingsJson.FindMember(source->GetName().c_str());
    if (iter != _bindingsJson.MemberEnd()) {
        // Source exists in json already
        sourceJson = &(iter->value);
    } else {
        rapidjson::Value name;
        name.SetString(source->GetName().c_str(),
                       static_cast<unsigned int>(source->GetName().size()),
                       _json.GetAllocator());
        rapidjson::Value value(rapidjson::kObjectType);
        _bindingsJson.AddMember(name, value, _json.GetAllocator());
        sourceJson = &(_bindingsJson.FindMember(source->GetName().c_str())->value);
    }
    ASSERT_NOT_NULL(sourceJson);

    auto targetIter = sourceJson->FindMember(target->GetName().c_str());
    if (targetIter != sourceJson->MemberEnd()) {
        // Already exists.
        // TODO: Update if needed.
    } else {
        rapidjson::Value name;
        name.SetString(target->GetName().c_str(),
                       static_cast<unsigned int>(target->GetName().size()),
                       _json.GetAllocator());
        rapidjson::Value value(rapidjson::kObjectType);
        sourceJson->AddMember(name, value, _json.GetAllocator());
    }
    Log();
}

void BindingsMonitor::RemoveBinding(const Engine::BindingSource *source,
                                    const Engine::BindingTarget *target) {
    // TODO
    // once AddBinding is finalized (data format)
    Log();
}

#endif // GB_NEW_STACK
