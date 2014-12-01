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
	//������������
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
	//��Ч
	if(UserDefault::getInstance()->getBoolForKey("sound", true))
	{
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM/BackMusic.mp3");
		SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM/BackMusic.mp3", true);
	}
	//��Ϸͼ��
	gameLayer = Layer::create();
	this->addChild(gameLayer, 1);
	//////////////////////////////////////////
	//��ťͼ��
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
	//����
	ParticleSystemQuad* parQuad = ParticleSystemQuad::create("1.plist");
	parQuad->setPosition(640/2, 960/2);
	this->addChild(parQuad);
	//black top
	Sprite* bTop = Sprite::create("top.png");
	bTop->setAnchorPoint(Vec2(0, 1));
	bTop->setPosition(0, 960);
	this->addChild(bTop);
	//��ǰ��߷���
	highScore = 0;
	//���������
	blockCount = 0;
	//�ϴθ߶�
	laseHeight = 0;
	//����
	label = Label::createWithSystemFont("0", "PYRITE.TTF", 24);
	label->setPosition(50, 0 + 25);
	gameLayer->addChild(label);
	//��������
	drawNode = DrawNode::create();
	gameLayer->addChild(drawNode);
	//�Ҳ����ֵ��ǩ
	highScoreLabel = LabelTTF::create("0", "PYRITE.TTF", 300);
	highScoreLabel->setAnchorPoint(Vec2(1, 1));
	highScoreLabel->setPosition(600, 920);
	highScoreLabel->setFontSize(240);
	highScoreLabel->setColor(Color3B(183, 174, 200));
	highScoreLabel->setString("0");
	this->addChild(highScoreLabel);
	/////////////////////////////////����///////////////////////////////////
	Sprite* edge = Sprite::create("Block/floot.png");
	edge->setPhysicsBody(PhysicsBody::createBox(Size(288, 32)));
	//��̬
	edge->getPhysicsBody()->setDynamic(false);
	//����
	edge->getPhysicsBody()->getShape(0)->setRestitution(0);
	//Ħ��
	edge->getPhysicsBody()->getShape(0)->setFriction(1.0f);
	//����
	edge->getPhysicsBody()->setContactTestBitmask(-1);
	//����
	edge->setPosition(640 / 2, 32);
	gameLayer->addChild(edge);
	
	//��ʼ����һ����
	nowBlock = Block::create(ZB);
	nowBlock->setPosition(640 / 2, 960);
	gameLayer->addChild(nowBlock);
	nowBlock->runAction(MoveTo::create(10 - this->getPositionY() / 600, Vec2(640 / 2, 0)));
	//��ӵ�����
	blockVector.pushBack(nowBlock);
	
	//��ͣͼ��
	pauseLayer = PauseLayer::create(this);
	this->addChild(pauseLayer, 10);

	//��ײ����
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
					//�����ӿ�
					newBlock();
				}
			}
		}

		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(phyEvent, this);
	//��������
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
			//���û�н���  ��ô��ͣ��Ϸ
			if(!gameover)
			{
				pauseLayer->pauseGame();
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyEvent, this);

	/////////////////////����START///////////////////////////
	EventListenerTouchOneByOne* touchEvent = EventListenerTouchOneByOne::create();
	//����ʱ
	touchEvent->onTouchBegan = [=](Touch* touch, Event* event){
		b_rotation = false;	//�Ƿ���ת��ʼ��
		b_move = false;		//�Ƿ��Ѿ��ƶ��� ��ʼ��
		oldLoc = touch->getLocation();
		return true;
	};
	//�ƶ�ʱ
	touchEvent->onTouchMoved = [=](Touch* touch, Event* event){
		Vec2 nowLoc = touch->getLocation();
		//���ϻ���
		if(nowLoc.y - oldLoc.y > 20 && !b_move){
			b_rotation = true;	//��ת��ʶΪtrue
			oldLoc = nowLoc;
		}
		//���һ���
		else if(nowLoc.x - oldLoc.x > 14 && !b_rotation){
			nowBlock->setPositionX(nowBlock->getPositionX() + 16);
			b_move = true;
			oldLoc = nowLoc;
		}
		//���󻬶�
		else if(nowLoc.x - oldLoc.x < -14 && !b_rotation){
			nowBlock->setPositionX(nowBlock->getPositionX() - 16);
			b_move = true;
			oldLoc = nowLoc;
		}
	};
	//̧��ʱ
	touchEvent->onTouchEnded = [=](Touch* touch, Event* event){
		if(b_rotation){
			nowBlock->setRotation(nowBlock->getRotation() + 90);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, this);
	/////////////////////����END///////////////////////////

	//��ʱ��
	this->schedule(schedule_selector(GameScene::myFunc));

	//���ز������
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
	//��ǰ��Ч�������
	float height = getMaxHeight();
	//������Ϸ������
	if (height > highScore)
	{
		highScore = height;
		//���·�����ǩ
		char scoreStr[10];
		sprintf(scoreStr, "%d", highScore);
		highScoreLabel->setString(scoreStr);
	}
	//log("highScore = %d", highScore);

	//�ƶ���ͷ����
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
	//�����ǰ��Խ�� ��ô�����¿�
	if (nowBlock->getPositionY() <= 0 && !gameover)
	{
		newBlock();
		log("new block");
	}
}

int GameScene::getMaxHeight()
{
	int height = 0;
	//���������� �õ���ǰ���ֵ
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

	//���Ʒ�����
	drawNode->clear();
	Vec2 vec[4];
	vec[0].set(50, 50);
	vec[1].set(50, height);
	vec[2].set(53, height);
	vec[3].set(53, 50);
	drawNode->drawPolygon(vec, 4, Color4F(183.0f / 255.0f, 174.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f), 0, Color4F(183.0f / 255.0f, 174.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f));
	//����λ��
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
	//������ɿ�����
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
	//�Ѿ����ַ�����++
	blockCount++;
	//log("blockCount == %d", blockCount);
	//�����¿�λ��
	buf->setPosition(640 / 2, laseHeight + 960);
	gameLayer->addChild(buf);
	//��ӵ�����
	blockVector.pushBack(buf);
	//��������
	nowBlock->getPhysicsBody()->setGravityEnable(true);
	//�滻��ǰ
	nowBlock = buf;
	//�˶�
	nowBlock->runAction(MoveTo::create(10 - this->getPositionY() / 600, Vec2(640 / 2, 0)));

	//�������MAXBOX
	if (blockCount >= MAX_BOX)
	{
		gameover = true;
	}
}

Block* GameScene::getNowBlock()
{
	return nowBlock;
}
