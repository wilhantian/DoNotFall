#include "MailLayer.h"
#include "ToolBox.h"

USING_NS_CC;

bool MailLayer::init()
{
	if (!Layer::init())
		return false;

	Size size(640, 960);
	//bk
// 	bk = Sprite::create("UI/Mail/bk.png");
// 	bk->setPosition(size.width / 2, size.height / 2);
// 	bk->setOpacity(0);
// 	this->addChild(bk);
	//menu
	Menu* menu = Menu::create();
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(0, 0);
	this->addChild(menu);
	//bk
	bk = MenuItemImage::create("UI/Mail/bk.png", "UI/Mail/bk.png", "", CC_CALLBACK_1(MailLayer::menuEventCallBack, this));
	bk->setPosition(size.width / 2, size.height / 2);
	bk->setOpacity(0);
	menu->addChild(bk);

	//top bk
	top_bk = Sprite::create("UI/Mail/top_bk.png");
	top_bk->setPosition(size.width / 2, size.height + top_bk->getContentSize().height/2);
	this->addChild(top_bk);
	
	//读取标题 
//	const char* str_title = ToolBox::getStringWithJsonFile("mail.json", "title", "NULL");
	std::string str_title = UserDefault::getInstance()->getStringForKey("title", "DoNotFall"); //ToolBox::getStringWithJsonFile("mail.json", "title", "NULL");
	//ttf_title
	ttf_title = LabelTTF::create();
	ttf_title->setAnchorPoint(Vec2(0.5, 1));
	ttf_title->setString(str_title);
	ttf_title->setColor(Color3B(26, 26, 26));
	ttf_title->setFontSize(36);
	ttf_title->setPosition(640 / 2, 960 + ttf_title->getContentSize().height); //960 - 340
	this->addChild(ttf_title);
	//读取公告 
//	const char* str_context = ToolBox::getStringWithJsonFile("mail.json", "context", "NULL");
	std::string str_context = UserDefault::getInstance()->getStringForKey("context", "context is null :(");
	//替换换行符
	std::string a = str_context;
	std::string b = "\\\\n";
	std::string c = "\n";
	int pos;
	pos = a.find(b);////查找指定的串
	while (pos != -1)
	{
		a.replace(pos,b.length(),c);////用新的串替换掉指定的串
		pos = a.find(b);//////继续查找指定的串，直到所有的都找到为止
	}
	//ttf_context
	ttf_context = LabelTTF::create();
	ttf_context->setAnchorPoint(Vec2(0.5, 1));
//	ttf_context->setString(str_context);
	ttf_context->setString(a);
	ttf_context->setColor(Color3B(26, 26, 26));
	ttf_context->setFontSize(26);
	ttf_context->setPosition(640 / 2, 960 + ttf_context->getContentSize().height); //960 - 340
	this->addChild(ttf_context);
	//事件监听
// 	EventListenerTouchOneByOne* touchEvent = EventListenerTouchOneByOne::create();
// 	touchEvent->onTouchBegan = [=](Touch* touch, Event* event){
// 		if (!top_bk->getBoundingBox().containsPoint( touch->getLocation())){
// 			resumeGame();
// 		}
// 		return true;
// 	};
// 	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, this);

	this->setVisible(false);

	return true;
}

void MailLayer::pauseGame()
{
	this->setVisible(true);
	//run background action
	FadeTo* fadeTo = FadeTo::create(0.2, 255);
	bk->runAction(fadeTo);
	//run ttf action
	EaseElasticOut* ttf_title_Ease = EaseElasticOut::create(MoveTo::create(1, Vec2(640 / 2, 960 - 320)));
	ttf_title->runAction(ttf_title_Ease);
	//run ttf action
	EaseElasticOut* ttfEase = EaseElasticOut::create(MoveTo::create(1, Vec2(640 / 2, 960-370)));
	ttf_context->runAction(ttfEase);
	//run pause background action
	EaseElasticOut* pauseEase = EaseElasticOut::create(MoveTo::create(1, Vec2(640 / 2, 960 / 2)));
	top_bk->runAction(pauseEase);
}

void MailLayer::resumeGame()
{
	//run background action
	FadeTo* fadeTo = FadeTo::create(0.2, 0);
	bk->runAction(fadeTo);
	//run ttf action
	EaseBackOut* ttf_title_Ease = EaseBackOut::create(MoveTo::create(0.5, Vec2(640 / 2, 960 + ttf_title->getContentSize().height)));
	ttf_title->runAction(ttf_title_Ease);
	//run ttf action
	EaseBackOut* ttfEase = EaseBackOut::create(MoveTo::create(0.5, Vec2(640 / 2, 960 + ttf_context->getContentSize().height)));
	ttf_context->runAction(ttfEase);
	//run pause background action
	EaseBackOut* pauseEase = EaseBackOut::create(MoveTo::create(0.5, Vec2(640 / 2, 960 + top_bk->getContentSize().height / 2)));
	Sequence* actions = Sequence::createWithTwoActions(pauseEase, CallFuncN::create(CC_CALLBACK_1(MailLayer::layerVisibleFlase, this)));
	top_bk->runAction(actions);
}

void MailLayer::layerVisibleFlase(Node* time)
{
	this->setVisible(false);
}

void MailLayer::menuEventCallBack(Ref* sender)
{
	resumeGame();
}
