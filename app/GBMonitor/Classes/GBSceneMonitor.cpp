//
//  GBSceneMonitor.cpp
//  Words2
//

#ifdef GB_NEW_STACK

#include "GBSceneMonitor.h"

#include "core/inc/debug/debug.h"

#include "2d/CCNode.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"

#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "GBSIODelegate.h"

using namespace Monitor;
using namespace cocos2d;
using namespace cocos2d::network;

static const char *kSceneWriteKey = "sceneWrite";

void SceneMonitor::Connect(const std::string &address, GBSIODelegate *delegate) {
    SetSIODelegate(delegate);
    Connect(address);
}

void SceneMonitor::Connect(const std::string &address) {
    ASSERT_NOT_NULL(_sioDelegate);
    ASSERT_NULL(_sioClient);
    _sioClient = SocketIO::connect("10.0.0.4:3000", *_sioDelegate);
    CC_SAFE_RETAIN(_sioClient);
    _sioClient->on("registered", CC_CALLBACK_2(GBSIODelegate::OnRegistered, *_sioDelegate));
    _sioClient->on("connected", CC_CALLBACK_2(GBSIODelegate::OnConnected, *_sioDelegate));
}

void SceneMonitor::Disconnect() {
    _sioClient->disconnect();
    CC_SAFE_RELEASE_NULL(_sioClient);
}

void SceneMonitor::SetSIODelegate(GBSIODelegate *delegate) {
    _sioDelegate = delegate;
}

static Rect GetBoundingBoxInWorld(cocos2d::Node *node) {
    Rect localBB(0, 0, node->getContentSize().width, node->getContentSize().height);
    return RectApplyAffineTransform(localBB, node->getNodeToWorldAffineTransform());
}

static void LogNode(cocos2d::Node *node, rapidjson::Value &parent, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value domNode(rapidjson::kObjectType);

    domNode.AddMember("x", node->getPositionX(), allocator);
    domNode.AddMember("y", node->getPositionY(), allocator);
    domNode.AddMember("z", node->getLocalZOrder(), allocator);
    domNode.AddMember("width", node->getContentSize().width, allocator);
    domNode.AddMember("height", node->getContentSize().height, allocator);
    domNode.AddMember("scaleX", node->getScaleX(), allocator);
    domNode.AddMember("scaleY", node->getScaleY(), allocator);
    Rect rect = GetBoundingBoxInWorld(node);
    domNode.AddMember("bb_x", rect.origin.x, allocator);
    domNode.AddMember("bb_y", rect.origin.y, allocator);
    domNode.AddMember("bb_w", rect.size.width, allocator);
    domNode.AddMember("bb_h", rect.size.height, allocator);

    rapidjson::Value name;
    name.SetString(node->getName().c_str(), static_cast<unsigned int>(node->getName().size()), allocator);
    domNode.AddMember("name", name, allocator);
    // domNode.AddMember("color", node->getColor(), allocator);

    rapidjson::Value domChildren(rapidjson::kArrayType);
    for (auto child : node->getChildren()) {
        LogNode(child, domChildren, allocator);
    }
    domNode.AddMember("children", domChildren, allocator);

    if (parent.IsArray()) {
        parent.PushBack(domNode, allocator);
    } else if (parent.IsObject()) {
        // Maybe assert and only allow array.
        parent.AddMember("child", domNode, allocator);
    } else {
        // ERROR.
    }
}

void SceneMonitor::Start() {
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([this](float) {
        rapidjson::Document output;
        output.SetObject();

        // must pass an allocator when the object may need to allocate memory
        rapidjson::Document::AllocatorType &allocator = output.GetAllocator();
        output.AddMember("room", "room_a", allocator);

        rapidjson::Value message(rapidjson::kObjectType);

        // Add cocos "global" data
        message.AddMember("sceneWidth", Director::getInstance()->getWinSize().width, allocator);
        message.AddMember("sceneHeight", Director::getInstance()->getWinSize().height, allocator);

        // Add scene hierarchy
        rapidjson::Value scene(rapidjson::kObjectType);
        LogNode(Director::getInstance()->getRunningScene(), scene, allocator);
        message.AddMember("scene", scene, allocator);

        output.AddMember("message", message, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        output.Accept(writer);

        ASSERT_NOT_NULL(_sioClient);
        _sioClient->emit("log", buffer.GetString());
        CCLOG("PETER Writing to log %s", buffer.GetString());
    }, this, 10.0f, -1, 0, false,  kSceneWriteKey);
}

void SceneMonitor::Stop() {
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(kSceneWriteKey, this);
}

#endif // GB_NEW_STACK
