#ifndef _HELP_SCENE_H_
#define _HELP_SCENE_H_

#include "cocos2d.h"

class HelpScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(HelpScene);

	virtual bool init();
};

#endif