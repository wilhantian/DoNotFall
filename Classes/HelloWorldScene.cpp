#include "HelloWorldScene.h"
#include "GameScene.h"
#include "MailLayer.h"
#include "ToolBox.h"
#include "BillBoardScene.h"
#include "HelpScene.h"
//友盟统计
#include "MobClickCpp.h"
//#include "Block.h"
USING_NS_CC;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	//设置物理世界
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0, -120));
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
	if (!LayerColor::initWithColor(Color4B(156, 205, 217, 255)))
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size size = Size(640, 960);
	//LOGO
	Sprite* logo = Sprite::create("UI/logo.png");
	logo->setPosition(size.width / 2, size.height - 300);
	this->addChild(logo);
	//down line
	Sprite* line = Sprite::create("UI/down_line.png");
	line->setPosition(size.width / 2, size.height - 820);
	this->addChild(line);
	
	//menu
	Menu* menu = Menu::create();
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(0, 0);
	this->addChild(menu);
	
	//classic button
	MenuItemImage* bt_classic = MenuItemImage::create("UI/classic.png", "UI/classic2.png", "", CC_CALLBACK_1(HelloWorld::menuClassicCallback, this));
	bt_classic->setPosition(220, size.height - 530);
	menu->addChild(bt_classic);
	//survive button
	MenuItemImage* bt_survive = MenuItemImage::create("UI/survive.png", "UI/survive2.png", "", CC_CALLBACK_1(HelloWorld::menuSurviveCallback, this));
	bt_survive->setPosition(420, size.height - 530);
	//bt_survive->setEnabled(false);
	menu->addChild(bt_survive);

	//rank button
	MenuItemImage* bt_rank = MenuItemImage::create("UI/rank.png", "UI/rank2.png", "", CC_CALLBACK_1(HelloWorld::menuBillBoardCallback, this));
	bt_rank->setPosition(170, size.height - 880);
	menu->addChild(bt_rank);
	//sound button
	if (UserDefault::getInstance()->getBoolForKey("sound", true))
	{
		bt_sound = MenuItemImage::create("UI/sound.png", "UI/sound2.png", "", CC_CALLBACK_1(HelloWorld::menuSoundCallback, this));
		bt_sound->setPosition(280, size.height - 880);
		menu->addChild(bt_sound);
	}
	else
	{
		bt_sound = MenuItemImage::create("UI/sound_no.png", "UI/sound_no2.png", "", CC_CALLBACK_1(HelloWorld::menuSoundCallback, this));
		bt_sound->setPosition(280, size.height - 880);
		menu->addChild(bt_sound);
	}
	//mail button
	MenuItemImage* bt_mail = MenuItemImage::create("UI/mail.png", "UI/mail2.png", "", CC_CALLBACK_1(HelloWorld::menuMailCallback, this));
	bt_mail->setPosition(390, size.height - 880);
	menu->addChild(bt_mail);
	//help button
	MenuItemImage* bt_help = MenuItemImage::create("UI/help.png", "UI/help2.png", "", CC_CALLBACK_1(HelloWorld::menuHelpCallback, this));
	bt_help->setPosition(500, size.height - 880);
	menu->addChild(bt_help);

	//时间监听
	EventListenerKeyboard* keyEvent = EventListenerKeyboard::create();
	keyEvent->onKeyReleased = [=](EventKeyboard::KeyCode kcode, Event* event){
		if (kcode == EventKeyboard::KeyCode::KEY_BACK || kcode == EventKeyboard::KeyCode::KEY_BACKSPACE)
		{
			//友盟统计
			umeng::MobClickCpp::end();
			//结束游戏
			Director::getInstance()->end();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyEvent, this);

	//友盟检测更新
	ToolBox::autoUpdate();

	return true;
}

void HelloWorld::menuClassicCallback(Ref* pSender)
{
	Scene* scene = TransitionSlideInL::create(0.15, GameScene::createScene());
	Director::getInstance()->replaceScene(scene);
}

void HelloWorld::menuSurviveCallback(Ref* pSender)
{
	const char* title = ToolBox::getStringWithJsonFile("messageBox.json", "title", "null");
	const char* context = ToolBox::getStringWithJsonFile("messageBox.json", "context", "null");
	MessageBox(title, context);
}

void HelloWorld::menuHelpCallback(Ref* pSender)
{
	Scene* scene = TransitionSlideInL::create(0.15, HelpScene::createScene());
	Director::getInstance()->replaceScene(scene);
}

/*公告*/
void HelloWorld::menuMailCallback(Ref* pSender)
{
	MailLayer* layer = MailLayer::create();
	this->addChild(layer);
	layer->pauseGame();
}

/*音效*/
void HelloWorld::menuSoundCallback(Ref* pSender)
{

	if (UserDefault::getInstance()->getBoolForKey("sound", true))
	{
//		//暂停音乐
//		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("sound", false);
		bt_sound->setNormalSpriteFrame(SpriteFrame::create("UI/sound_no.png", Rect(0, 0, 51,51)));
		bt_sound->setSelectedSpriteFrame(SpriteFrame::create("UI/sound_no2.png", Rect(0, 0, 51, 51)));
	}
	else
	{
		UserDefault::getInstance()->setBoolForKey("sound", true);
		bt_sound->setNormalSpriteFrame(SpriteFrame::create("UI/sound.png", Rect(0, 0, 51, 51)));
		bt_sound->setSelectedSpriteFrame(SpriteFrame::create("UI/sound2.png", Rect(0, 0, 51, 51)));
	}
	
}

/*排行榜*/
void HelloWorld::menuBillBoardCallback(Ref* pSender)
{
	//跳转到排行榜场景
	TransitionFadeDown* scene = TransitionFadeDown::create(0.7, BillBoardScene::createScene(0));
	Director::getInstance()->replaceScene(scene);
}
