#ifndef _PAUSE_LAYER_H_
#define _PAUSE_LAYER_H_

#include "cocos2d.h"

class GameScene;

class PauseLayer : public cocos2d::Layer
{
private:
	GameScene* gameScene;
	cocos2d::Sprite* backGround;
	cocos2d::Sprite* pauseBk;
public:
	virtual bool init(GameScene* gameScene);
	static PauseLayer* create(GameScene* gameScene);

	void buttonEvent(Ref* ref);
	void pauseGame();
	void resumeGame();
	void layerVisibleFlase(Node* time);
};

#endif