#ifndef _GAME_SCENE_H
#define _GAME_SCENE_H

#include "cocos2d.h"
#include "Block.h"
//class Block;
class PauseLayer;

class GameScene : public cocos2d::LayerColor
{
private:
	cocos2d::Vec2 oldLoc;	//touch�¼�  �ϴ�����
	bool b_rotation;	//touch�¼�  �Ƿ���ת
	bool b_move;	//touch�¼�  �Ƿ��Ѿ��ƶ�

	cocos2d::Layer* gameLayer;
	PauseLayer* pauseLayer;

	Block* nowBlock;
	cocos2d::Vector<Block*> blockVector;
	cocos2d::Label* label;
	cocos2d::LabelTTF* highScoreLabel;
	cocos2d::DrawNode* drawNode;
	float laseHeight;
	int blockCount;
	int highScore;	//��ǰ��Ϸ��߷�
	bool gameover;
	bool finish;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	void myFunc(float time);
	int getMaxHeight();
	void newBlock();

	//get now block
	Block* getNowBlock();
};

#endif // __HELLOWORLD_SCENE_H__
