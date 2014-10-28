#ifndef _FINISH_LAYER_H_
#define _FINISH_LAYER_H_

#include "cocos2d.h"

class FinishLayer : public cocos2d::Layer
{
private:
	//cocos2d::Sprite* bk;
	int score;
	cocos2d::MenuItemImage* bk;
	cocos2d::Sprite* top_bk;
public:
	virtual bool init(int score);

	static FinishLayer* create(int score);

	void menuCallBackHome(Ref* sender);
	void menuCallBackPlay(Ref* sender);
	void menuCallBackShare(Ref* sender);
	void menuCallBackRank(Ref* sender);
	void menuVoid(Ref* sender);
	void updateRank(cocos2d::LabelTTF* label, int score);

	void saveScene(Node* node);
};

#endif
