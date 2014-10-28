#ifndef _BILL_BOARD_SCENE_H_
#define _BILL_BOARD_SCENE_H_

#include "cocos2d.h"

class BillBoardScene : public cocos2d::LayerColor
{
public:
	virtual bool init(int button_type);
	/*button_type: 0 home  , 1 back*/
	static BillBoardScene* create(int button_type);

	static cocos2d::Scene* createScene(int button_type);
	void menuHomeCallback(Ref* sender);
	void menuBackCallback(Ref* sender);
};

#endif