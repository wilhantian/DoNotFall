#include "GameScene.h"
#include "PauseLayer.h"
#include "FinishLayer.h"
#include "ToolBox.h"
//#include "Block.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

#define MAX_BOX 3

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	//设置物理世界
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0, -120));
	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(156, 205, 217, 255)))
	{
		return false;
	}
	ToolBox::rank_label = NULL;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	gameover = false;
	finish = false;
	//音效
	if(UserDefault::getInstance()->getBoolForKey("sound", true))
	{
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM/BackMusic.mp3");
		SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM/BackMusic.mp3", true);
	}
	//游戏图层
	gameLayer = Layer::create();
	this->addChild(gameLayer, 1);
	//////////////////////////////////////////
	//按钮图层
	Layer* buttonLayer = Layer::create();
	this->addChild(buttonLayer, 2);

	Menu* menu = Menu::create();
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(0, 0);
	buttonLayer->addChild(menu);

	MenuItemImage* button = MenuItemImage::create("menu1.png", "menu2.png", "menu1.png", CC_CALLBACK_1(GameScene::menuCloseCallback, this));
	button->setPosition(640-50, 960-50);
	menu->addChild(button);
	//////////////////////////////////////////
	//粒子
	ParticleSystemQuad* parQuad = ParticleSystemQuad::create("1.plist");
	parQuad->setPosition(640/2, 960/2);
	this->addChild(parQuad);
	//black top
	Sprite* bTop = Sprite::create("top.png");
	bTop->setAnchorPoint(Vec2(0, 1));
	bTop->setPosition(0, 960);
	this->addChild(bTop);
	//当前最高分数
	highScore = 0;
	//方块计数器
	blockCount = 0;
	//上次高度
	laseHeight = 0;
	//分数
	label = Label::createWithSystemFont("0", "PYRITE.TTF", 24);
	label->setPosition(50, 0 + 25);
	gameLayer->addChild(label);
	//左侧分数条
	drawNode = DrawNode::create();
	gameLayer->addChild(drawNode);
	//右侧最大值标签
	highScoreLabel = LabelTTF::create("0", "PYRITE.TTF", 300);
	highScoreLabel->setAnchorPoint(Vec2(1, 1));
	highScoreLabel->setPosition(600, 920);
	highScoreLabel->setFontSize(240);
	highScoreLabel->setColor(Color3B(183, 174, 200));
	highScoreLabel->setString("0");
	this->addChild(highScoreLabel);
	/////////////////////////////////底座///////////////////////////////////
	Sprite* edge = Sprite::create("Block/floot.png");
	edge->setPhysicsBody(PhysicsBody::createBox(Size(288, 32)));
	//静态
	edge->getPhysicsBody()->setDynamic(false);
	//弹性
	edge->getPhysicsBody()->getShape(0)->setRestitution(0);
	//摩擦
	edge->getPhysicsBody()->getShape(0)->setFriction(1.0f);
	//掩码
	edge->getPhysicsBody()->setContactTestBitmask(-1);
	//坐标
	edge->setPosition(640 / 2, 32);
	gameLayer->addChild(edge);
	
	//初始化第一个块
	nowBlock = Block::create(ZB);
	nowBlock->setPosition(640 / 2, 960);
	gameLayer->addChild(nowBlock);
	nowBlock->runAction(MoveTo::create(10 - this->getPositionY() / 600, Vec2(640 / 2, 0)));
	//添加到集合
	blockVector.pushBack(nowBlock);
	
	//暂停图层
	pauseLayer = PauseLayer::create(this);
	this->addChild(pauseLayer, 10);

	//碰撞监听
	auto phyEvent = EventListenerPhysicsContact::create();
	phyEvent->onContactBegin = [=](PhysicsContact& contact){
		Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

		if (spriteA == nowBlock || spriteB == nowBlock)
		{
			if (!finish)
			{
				if (gameover)
				{
					finish = true;
					nowBlock->stopAllActions();
					nowBlock->getPhysicsBody()->setGravityEnable(true);
					nowBlock->pause();
					//stop music
					SimpleAudioEngine::getInstance()->stopBackgroundMusic();
					//todo
					FinishLayer* finishLayer = FinishLayer::create(highScore);
					this->addChild(finishLayer, 3);
				}
				else
				{
					//随机添加块
					newBlock();
				}
			}
		}

		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(phyEvent, this);
	//按键监听
	EventListenerKeyboard* keyEvent = EventListenerKeyboard::create();
//	keyEvent->onKeyPressed = [=](EventKeyboard::KeyCode kcode, Event* event){
	keyEvent->onKeyReleased = [=](EventKeyboard::KeyCode kcode, Event* event){
		/*
		if (kcode == EventKeyboard::KeyCode::KEY_A)
		{
			nowBlock->setPositionX(nowBlock->getPositionX() - 16);
		}
		if (kcode == EventKeyboard::KeyCode::KEY_D)
		{
			nowBlock->setPositionX(nowBlock->getPositionX() + 16);
		}
		if (kcode == EventKeyboard::KeyCode::KEY_W)
		{
			nowBlock->setRotation(nowBlock->getRotation() + 90);
		}
		if (kcode == EventKeyboard::KeyCode::KEY_S)
		{
			nowBlock->getPhysicsBody()->setGravityEnable(true);
		}
		if (kcode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			//Director::getInstance()->replaceScene(FinishScene::createScene(highScore));
			nowBlock->resume();
		}
		*/
		if (kcode == EventKeyboard::KeyCode::KEY_BACK || kcode == EventKeyboard::KeyCode::KEY_BACKSPACE)
		{
			//如果没有结束  那么暂停游戏
			if(!gameover)
			{
				pauseLayer->pauseGame();
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyEvent, this);

	/////////////////////触屏START///////////////////////////
	EventListenerTouchOneByOne* touchEvent = EventListenerTouchOneByOne::create();
	//按下时
	touchEvent->onTouchBegan = [=](Touch* touch, Event* event){
		b_rotation = false;	//是否旋转初始化
		b_move = false;		//是否已经移动过 初始化
		oldLoc = touch->getLocation();
		return true;
	};
	//移动时
	touchEvent->onTouchMoved = [=](Touch* touch, Event* event){
		Vec2 nowLoc = touch->getLocation();
		//向上滑动
		if(nowLoc.y - oldLoc.y > 20 && !b_move){
			b_rotation = true;	//旋转标识为true
			oldLoc = nowLoc;
		}
		//向右滑动
		else if(nowLoc.x - oldLoc.x > 14 && !b_rotation){
			nowBlock->setPositionX(nowBlock->getPositionX() + 16);
			b_move = true;
			oldLoc = nowLoc;
		}
		//向左滑动
		else if(nowLoc.x - oldLoc.x < -14 && !b_rotation){
			nowBlock->setPositionX(nowBlock->getPositionX() - 16);
			b_move = true;
			oldLoc = nowLoc;
		}
	};
	//抬起时
	touchEvent->onTouchEnded = [=](Touch* touch, Event* event){
		if(b_rotation){
			nowBlock->setRotation(nowBlock->getRotation() + 90);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, this);
	/////////////////////触屏END///////////////////////////

	//定时器
	this->schedule(schedule_selector(GameScene::myFunc));

	//加载插屏广告
	ToolBox::loadSpotAds();

	return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
//	log("menuClose is touch");
	//Director::getInstance()->stopAnimation();
	pauseLayer->pauseGame();
}

void GameScene::myFunc(float time)
{
	//当前有效块最高数
	float height = getMaxHeight();
	//更新游戏最大分数
	if (height > highScore)
	{
		highScore = height;
		//更新分数标签
		char scoreStr[10];
		sprintf(scoreStr, "%d", highScore);
		highScoreLabel->setString(scoreStr);
	}
	//log("highScore = %d", highScore);

	//移动镜头操作
	if (height > 600 && abs(laseHeight - height) > 50)
	{
		MoveTo* action = MoveTo::create(2, Vect(0, -height + 480));
		EaseBackOut* inOut = EaseBackOut::create(action);

		gameLayer->runAction(inOut);
		laseHeight = height;
	}
	else if (height <= 600)
	{
		MoveTo* action = MoveTo::create(1.5, Vect(0, 0));
		EaseOut* inOut = EaseOut::create(action, 5);
		gameLayer->runAction(inOut);
		laseHeight = 0;
	}
	//log("height==%f,  laseHeight==%f", height, laseHeight);
	//如果当前快越界 那么制造新块
	if (nowBlock->getPositionY() <= 0 && !gameover)
	{
		newBlock();
		log("new block");
	}
}

int GameScene::getMaxHeight()
{
	int height = 0;
	//遍历储存器 得到当前最大值
	for (int i = 0; i < blockVector.size(); i++)
	{
		if (blockVector.at(i) != nowBlock)
		{
			int h = blockVector.at(i)->getPositionY();
			if (h > height)
			{
				height = h;
			}
		}
	}

	//绘制分数条
	drawNode->clear();
	Vec2 vec[4];
	vec[0].set(50, 50);
	vec[1].set(50, height);
	vec[2].set(53, height);
	vec[3].set(53, 50);
	drawNode->drawPolygon(vec, 4, Color4F(183.0f / 255.0f, 174.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f), 0, Color4F(183.0f / 255.0f, 174.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f));
	//分数位置
	char score[20];
	sprintf(score, "%d", height);
	label->setString(score);
	label->setPosition(50, height + 25);

	return height;
}

void GameScene::newBlock()
{
	nowBlock->stopAllActions();
	BLOCK block_type;
	//随机生成块类型
	switch ((int)(CCRANDOM_0_1() * 6))
	//switch (0)
	{
	case 0:
		block_type = I;
		break;
	case 1:
		block_type = L;
		break;
	case 2:
		block_type = LB;
		break;
	case 3:
		block_type = R;
		break;
	case 4:
		block_type = T;
		break;
	case 5:
		block_type = Z;
		break;
	case 6:
		block_type = ZB;
		break;
	}
	Block* buf = Block::create(block_type);
	//已经出现方块数++
	blockCount++;
	//log("blockCount == %d", blockCount);
	//设置新块位置
	buf->setPosition(640 / 2, laseHeight + 960);
	gameLayer->addChild(buf);
	//添加到集合
	blockVector.pushBack(buf);
	//开启重力
	nowBlock->getPhysicsBody()->setGravityEnable(true);
	//替换当前
	nowBlock = buf;
	//运动
	nowBlock->runAction(MoveTo::create(10 - this->getPositionY() / 600, Vec2(640 / 2, 0)));

	//如果大于MAXBOX
	if (blockCount >= MAX_BOX)
	{
		gameover = true;
	}
}

Block* GameScene::getNowBlock()
{
	return nowBlock;
}
