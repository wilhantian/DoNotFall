#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Block.h"
//class Block;

class HelloWorld : public cocos2d::LayerColor
{
private:
	Block* nowBlock;
	cocos2d::Vector<Block*> blockVector;
	cocos2d::Label* label;
	cocos2d::DrawNode* drawNode;
	cocos2d::MenuItemImage* bt_sound;
	float laseHeight;
	int blockCount;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuClassicCallback(cocos2d::Ref* pSender);
    void menuSurviveCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	void menuMailCallback(Ref* pSender);
	void menuSoundCallback(Ref* pSender);
	void menuBillBoardCallback(Ref* pSender);
};

#endif // __HELLOWORLD_SCENE_H__
