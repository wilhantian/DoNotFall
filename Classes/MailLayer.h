#ifndef _MAIL_LAYER_H_
#define _MAIL_LAYER_H_

#include "cocos2d.h"

class Block;

class MailLayer : public cocos2d::Layer
{
private:
	//cocos2d::Sprite* bk;
	cocos2d::MenuItemImage* bk;
	cocos2d::Sprite* top_bk;
	cocos2d::LabelTTF* ttf_title;
	cocos2d::LabelTTF* ttf_context;
public:
	virtual bool init();
	CREATE_FUNC(MailLayer);

	void pauseGame();
	void resumeGame();

	void layerVisibleFlase(Node* time);
	void menuEventCallBack(Ref* sender);
};

#endif