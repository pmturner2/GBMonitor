#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "network/SocketIO.h"

#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

USING_NS_CC;
using namespace cocos2d::network;

static int unique_id;

class GBSIODelegate : public SocketIO::SIODelegate {
public:
    virtual ~GBSIODelegate() {}
    
    void onConnect(SIOClient* client) override { 
        CCLOG("PETER SIODelegate onConnect fired"); 
    }

    void onMessage(SIOClient* client, const std::string& data) override { 
        CC_UNUSED_PARAM(client); CCLOG("PETER SIODelegate onMessage fired with data: %s", data.c_str());       
    }

    void onClose(SIOClient* client) override {
    }

    void onError(SIOClient* client, const std::string& data) override {
    }

    void fireEventToScript(SIOClient* client, const std::string& eventName, const std::string& data) override { 
        CC_UNUSED_PARAM(client); CCLOG("PETER SIODelegate event '%s' fired with data: %s", eventName.c_str(), data.c_str()); 
    }

    void OnConnected(SIOClient* client, const std::string &data) {
        CCLOG("PETER OnConnected with data: %s", data.c_str());
        client->emit("registerApp", "");
    }

    void OnRegistered(SIOClient*, const std::string &data) {
        CCLOG("PETER OnRegistered with data: %s", data.c_str());
        rapidjson::Document document;
        document.Parse(data.c_str());
        unique_id = document["uniqueId"].GetInt();
        CCLOG("PETER Parsed json for unique_id = %d", unique_id);
    }
};

static GBSIODelegate _delegate;
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
        name.SetString(node->getName().c_str(), node->getName().size(), allocator);
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

    Scene* HelloWorld::createScene()
    {
    // 'scene' is an autorelease object
        auto scene = Scene::create();

    // 'layer' is an autorelease object
        auto layer = HelloWorld::create();

    // add layer as a child to scene
        scene->addChild(layer);

    // return the scene
        return scene;
    }

// on "init" you need to initialize your instance
    bool HelloWorld::init()
    {
    //////////////////////////////
    // 1. super init first
        if ( !Layer::init() )
        {
            return false;
        }

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
        auto closeItem = MenuItemImage::create(
           "CloseNormal.png",
           "CloseSelected.png",
           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

        closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
            origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
        auto menu = Menu::create(closeItem, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

        auto label = Label::createWithTTF("Hello PETERS World", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
        this->addChild(label, 1);

    // add "HelloWorld" splash screen"
        auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
        this->addChild(sprite, 0);

        auto client = SocketIO::connect("10.0.0.4:3000", _delegate);
        client->retain();
        client->on("registered", CC_CALLBACK_2(GBSIODelegate::OnRegistered, _delegate));
        client->on("connected", CC_CALLBACK_2(GBSIODelegate::OnConnected, _delegate));

//void Node::schedule(const std::function<void(float)> &callback, float interval, const std::string &key)
           
        schedule([this, client](float) {
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
    
            client->emit("log", buffer.GetString());
            CCLOG("PETER Writing to log %s", buffer.GetString());
        }, 1.0f, "logWrite");

        return true;
    }

    void HelloWorld::menuCloseCallback(Ref* pSender)
    {
    //Close the cocos2d-x game scene and quit the application
        Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


    }
