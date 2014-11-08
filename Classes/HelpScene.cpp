#include "HelpScene.h"
#include "extensions/cocos-ext.h"
#include "HelloWorldScene.h"
USING_NS_CC;
USING_NS_CC_EXT;

Scene* HelpScene::createScene()
{
	Scene* scene = Scene::create();
	HelpScene* layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}

bool HelpScene::init()
{
	if (!LayerColor::initWithColor(Color4B(156, 205, 217, 255)))
	{
		return false;
	}

	Size size(640, 960);

	ScrollView* view = ScrollView::create(size);
	view->setAnchorPoint(Vec2(0 ,0));
	view->setPosition(0, 0);
	view->setContentSize(Size(640*3, 960));
	view->setBounceable(false);	//µ¯ÐÔÐ§¹û
	this->addChild(view);

	Sprite* help1 = Sprite::create("UI/Help/1.png");
	help1->setPosition(size.width / 2, size.height / 2);
	view->addChild(help1);

	Sprite* help2 = Sprite::create("UI/Help/2.png");
	help2->setPosition(size.width / 2 + 640, size.height / 2);
	view->addChild(help2);

	Sprite* help3 = Sprite::create("UI/Help/3.png");
	help3->setPosition(size.width / 2 + 640*2, size.height / 2);
	view->addChild(help3);

	Menu* menu = Menu::create();
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(0, 0);
	view->addChild(menu);

	MenuItemImage* nextButton = MenuItemImage::create("UI/Help/next.png", "UI/Help/next2.png", "", CC_CALLBACK_1(HelpScene::menuNextCallback, this));
	nextButton->setPosition(640*2 + 640/2, 260);
	menu->addChild(nextButton);

	//¼àÌý
	EventListenerKeyboard* keyEvent = EventListenerKeyboard::create();
	keyEvent->onKeyReleased = [=](EventKeyboard::KeyCode kcode, Event* event){
		if (kcode == EventKeyboard::KeyCode::KEY_BACK || kcode == EventKeyboard::KeyCode::KEY_BACKSPACE)
		{
			Scene* scene = TransitionSlideInR::create(0.15, HelloWorld::createScene());
			Director::getInstance()->replaceScene(scene);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyEvent, this);

	return true;
}

void HelpScene::menuNextCallback(Ref* sender)
{
	Scene* scene = TransitionSlideInR::create(0.15, HelloWorld::createScene());
	Director::getInstance()->replaceScene(scene);
}
