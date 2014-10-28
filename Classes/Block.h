#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "cocos2d.h"

#define BLOCK_L 1;
#define BLOCK_LR 2;
#define BLOCK_T 3;
#define BLOCK_I 4;
#define BLOCK_Z 5;
#define BLOCK_ZR 6;
#define BLOCK_R 7;

enum BLOCK
{
	L,
	LB,
	T,
	I,
	Z,
	ZB,
	R
};

class Block : public cocos2d::Sprite
{
private:
public:
	virtual bool init(BLOCK block_type);
	static Block* create(BLOCK block_type);
	cocos2d::PhysicsShapePolygon* getBlockShape(const char* name);
};

#endif