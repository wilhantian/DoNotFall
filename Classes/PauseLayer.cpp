#include "PauseLayer.h"
#include "Block.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
//友盟统计
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MobClickCpp.h"
#endif
using namespace CocosDenshion;

#define BUTTON_PLAY 0
#define BUTTON_REPLAY 1
#define BUTTON_HOME 2

USING_NS_CC;

bool PauseLayer::init(GameScene* gameScene)
{
	if (!Layer::init())
		return false;

	PauseLayer::gameScene = gameScene;
	
	//background
	backGround = Sprite::create("Pause/bk.png");
	backGround->setAnchorPoint(Vec2(0, 0));
	backGround->setPosition(0, 0);
	backGround->setOpacity(0);
	this->addChild(backGround);
	//pause background
	pauseBk = Sprite::create("Pause/pause_bk.png");
	pauseBk->setPosition(640/2, 960+278/2);
	this->addChild(pauseBk);
	//menu
	Menu* menu = Menu::create();
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(0, 0);
	pauseBk->addChild(menu);
	//home button 
	MenuItemImage* homeButton = MenuItemImage::create(
		"Pause/home1.png", "Pause/home2.png", "", 
		CC_CALLBACK_1(PauseLayer::buttonEvent, this));
	homeButton->setPosition(515 / 2 - 81 * 2 + 30, 85);	//position
	homeButton->setTag(BUTTON_HOME);
	menu->addChild(homeButton);
	//play button
	MenuItemImage* playButton = MenuItemImage::create(
		"Pause/play1.png", "Pause/play2.png", "",
		CC_CALLBACK_1(PauseLayer::buttonEvent, this));
	playButton->setPosition(515 / 2, 85);	//position
	playButton->setTag(BUTTON_PLAY);
	menu->addChild(playButton);
	//replay button
	MenuItemImage* replayButton = MenuItemImage::create(
		"Pause/replay1.png", "Pause/replay2.png", "",
		CC_CALLBACK_1(PauseLayer::buttonEvent, this));
	replayButton->setPosition(515 / 2 + 81 * 2 - 30, 85);	//position
	replayButton->setTag(BUTTON_REPLAY);
	menu->addChild(replayButton);

	this->setVisible(false);

	return true;
}

PauseLayer* PauseLayer::create(GameScene* gameScene)
{
	PauseLayer* layer = new PauseLayer();
	if (layer && layer->init(gameScene))
	{
		layer->autorelease();
		return layer;
	}
	else
	{
		delete layer;
		layer = NULL;
		return NULL;
	}
}

void PauseLayer::pauseGame()
{
	gameScene->getNowBlock()->pause();
	this->setVisible(true);
	//run background action
	FadeTo* fadeTo = FadeTo::create(0.1, 255);
	backGround->runAction(fadeTo);
	//run pause background action
	EaseBackOut* pauseEase = EaseBackOut::create(MoveTo::create(0.3, Vec2(640 / 2, 960 / 2)));
	pauseBk->runAction(pauseEase);
}

void PauseLayer::resumeGame()
{
	gameScene->getNowBlock()->resume();
	//run background action
	FadeTo* fadeTo = FadeTo::create(0.1, 0);
	backGround->runAction(fadeTo);
	//run pause background action
	EaseBackOut* pauseEase = EaseBackOut::create(MoveTo::create(0.3, Vec2(640 / 2, 960 + 278 / 2)));
	Sequence* actions = Sequence::createWithTwoActions(pauseEase, CallFuncN::create(CC_CALLBACK_1(PauseLayer::layerVisibleFlase, this)));
	pauseBk->runAction(actions);
}

void PauseLayer::layerVisibleFlase(Node* time)
{
	this->setVisible(false);
}

void PauseLayer::buttonEvent(Ref* ref)
{
	MenuItem* item = (MenuItem*)ref;
	int itemTag = item->getTag();

	if (itemTag == BUTTON_HOME)
	{
		Scene* scene = TransitionSlideInL::create(0.15, HelloWorld::createScene());
		Director::getInstance()->replaceScene(scene);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		//友盟统计
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		umeng::MobClickCpp::failLevel("_classic_");
#endif
	}
	else if (itemTag == BUTTON_PLAY)
	{
		resumeGame();
	}
	else if (itemTag == BUTTON_REPLAY)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		//todo
		Scene* scene = TransitionSlideInR::create(0.15, GameScene::createScene());
		Director::getInstance()->replaceScene(scene);
		//友盟统计
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		umeng::MobClickCpp::failLevel("_classic_");
#endif
	}
}

