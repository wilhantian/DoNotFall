#include "HelpScene.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

Scene* HelpScene::createScene()
{
	Scene* scene = Scene::create();
	HelpScene* layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}

bool HelpScene::init()
{
	if (!LayerColor::initWithColor(Color4B()))
	{
		return false;
	}

	Sprite* s = Sprite::create("box.png");
	s->setPosition(100, 100);

	ScrollView* view = ScrollView::create();
	view->addChild(s);

	this->addChild(view);

	return true;
}
