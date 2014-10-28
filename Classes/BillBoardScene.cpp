#include "BillBoardScene.h"
#include "HelloWorldScene.h"
#include "ToolBox.h"
#include "editor-support/spine/Json.h"

USING_NS_CC;

Scene* BillBoardScene::createScene(int button_type)
{
	Scene* scene = Scene::create();
	BillBoardScene* layer = BillBoardScene::create(button_type);
	scene->addChild(layer);
	return scene;
}

BillBoardScene* BillBoardScene::create(int button_type)
{
	BillBoardScene* bb = new BillBoardScene();
	if (bb->init(button_type))
	{
		bb->autorelease();
		return bb;
	}
	else
	{
		delete bb;
		bb = NULL;
		return NULL;
	}
}

bool BillBoardScene::init(int button_type)
{
	if (!LayerColor::initWithColor(Color4B(156, 205, 217, 255)))
	{
		return false;
	}
	Size size(640, 960);
	ToolBox::updateFillBoard();
	//背景	
	Sprite* bk = Sprite::create("UI/BillBoardScene/bb_bk.png");
	bk->setPosition(size.width/2, size.height/2);
	this->addChild(bk);

	//home按钮
	Menu* menu = Menu::create();
	menu->setAnchorPoint( Vec2(0, 0) );
	menu->setPosition(0, 0);
	this->addChild(menu);

	if (button_type == 0)
	{
		MenuItemImage* button_home = MenuItemImage::create("UI/BillBoardScene/bb_but_home.png", "UI/BillBoardScene/bb_but_home2.png", "", CC_CALLBACK_1(BillBoardScene::menuHomeCallback, this));
		button_home->setPosition(75, size.height - 56);
		menu->addChild(button_home);
	}
	else if (button_type == 1)
	{
		//back
		MenuItemImage* button_back = MenuItemImage::create("UI/BillBoardScene/board_back.png", "UI/BillBoardScene/board_back2.png", "", CC_CALLBACK_1(BillBoardScene::menuBackCallback, this));
		button_back->setPosition(75, size.height - 56);
		menu->addChild(button_back);
	}

	/*********************************************************************************/
	std::string str_rankJson = UserDefault::getInstance()->getStringForKey("str_rankJson", " ");

	Json* json = Json_create(str_rankJson.c_str());

	//如果json有内容
	if(json)
	{
		Json* items = Json_getItem(json, "results");
		int posSize = items->size;

		Json* j = items->child;
		for (int i = 0; i < posSize; i++)
		{
			std::string name = Json_getString(j, "name", "NLL");
			name = name.substr(8, 14);
			name = "***" + name;
			int score = Json_getInt(j, "score", 1);
			//todo
			log("FillBoard : %s %d", name.c_str(), score);

			/*START*/
			int y = i * 70;
			//排名
			char str_rank[4];
			sprintf(str_rank, "%d", i+1);

			LabelTTF* text_mc = LabelTTF::create();
			text_mc->setAnchorPoint(Vec2(0.5, 0.5));
			text_mc->setColor(Color3B(255, 255, 255));
			text_mc->setString(str_rank);
			text_mc->setFontSize(28);
			text_mc->setPosition(117, size.height + 14 - 235 - y);
			this->addChild(text_mc);

			//昵称
			LabelTTF* text_name = LabelTTF::create();
			text_name->setAnchorPoint(Vec2(0.5, 0.5));
			text_name->setColor(Color3B(255, 255, 255));
			text_name->setString(name);
			text_name->setFontSize(28);
			text_name->setPosition(330, size.height + 14 - 235 - y);
			this->addChild(text_name);

			//分数
			char str_score[20];
			sprintf(str_score, "%d", score);

			LabelTTF* text_score = LabelTTF::create();
			text_score->setAnchorPoint(Vec2(0.5, 0.5));
			text_score->setColor(Color3B(255, 255, 255));
			text_score->setString(str_score);
			text_score->setFontSize(28);
			text_score->setPosition(526, size.height + 14 - 235 - y);
			this->addChild(text_score);
			/*END*/
			j = j->next;
		}
	}
	/*********************************************************************************/
/*
	for (int i = 0; i < 10; i++)
	{
		int y = i * 70;
		//排名
		LabelTTF* text_mc = LabelTTF::create();
		text_mc->setAnchorPoint(Vec2(0.5, 0.5));
		text_mc->setColor(Color3B(255, 255, 255));
		text_mc->setString("10");
		text_mc->setFontSize(28);
		text_mc->setPosition(117, size.height + 14 - 235 - y);
		this->addChild(text_mc);

		//昵称
		LabelTTF* text_name = LabelTTF::create();
		text_name->setAnchorPoint(Vec2(0.5, 0.5));
		text_name->setColor(Color3B(255, 255, 255));
		text_name->setString("wilhan.tian");
		text_name->setFontSize(28);
		text_name->setPosition(330, size.height + 14 - 235 - y);
		this->addChild(text_name);

		//分数
		LabelTTF* text_score = LabelTTF::create();
		text_score->setAnchorPoint(Vec2(0.5, 0.5));
		text_score->setColor(Color3B(255, 255, 255));
		text_score->setString("380064");
		text_score->setFontSize(28);
		text_score->setPosition(526, size.height + 14 - 235 - y);
		this->addChild(text_score);
	}
*/
	return true;
}

void BillBoardScene::menuHomeCallback(Ref* sender)
{
	TransitionSlideInL* scene = TransitionSlideInL::create(0.15, HelloWorld::createScene());
	Director::getInstance()->replaceScene(scene);
}

void BillBoardScene::menuBackCallback(Ref* sender)
{
	Director::getInstance()->popScene();
}

