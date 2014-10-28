#include "Block.h"
#include "editor-support/spine/Json.h"

USING_NS_CC;

Block* Block::create(BLOCK block_type)
{
	Block *pRet = new Block();
	if (pRet && pRet->init(block_type))
	{
		pRet->autorelease();
		return pRet;
	}
	else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool Block::init(BLOCK block_type)
{
	if (!Sprite::init())
	{
		return false;
	}
	//����body
	PhysicsBody* body = PhysicsBody::create();
	//Ħ����
	float friction = 1.0f;
	//����
	float restitution = 0.0f;
	//ѡ������body������
	switch (block_type)
	{
	case L:{
		this->setTexture("Block/L.png");
		PhysicsShapePolygon* shapeL = getBlockShape("L_L");
		PhysicsShapePolygon* shapeR = getBlockShape("L_R");
		shapeL->setFriction(friction);
		shapeR->setFriction(friction);
		shapeL->setRestitution(restitution);
		shapeR->setRestitution(restitution);
		body->addShape(shapeL, true);
		body->addShape(shapeR, true);
		}
		break;
	case LB:{
		this->setTexture("Block/LB.png");
		PhysicsShapePolygon* shapeL = getBlockShape("LB_L");
		PhysicsShapePolygon* shapeR = getBlockShape("LB_R");
		shapeL->setFriction(friction);
		shapeR->setFriction(friction);
		shapeL->setRestitution(restitution);
		shapeR->setRestitution(restitution);
		body->addShape(shapeL, true);
		body->addShape(shapeR, true);
		}
		break;
	case T:{
		this->setTexture("Block/T.png");
		PhysicsShapePolygon* shapeL = getBlockShape("T_L");
		PhysicsShapePolygon* shapeR = getBlockShape("T_R");
		shapeL->setFriction(friction);
		shapeR->setFriction(friction);
		shapeL->setRestitution(restitution);
		shapeR->setRestitution(restitution);
		body->addShape(shapeL, true);
		body->addShape(shapeR, true);
		}
		break;
	case I:{
		this->setTexture("Block/I.png");
		PhysicsShapePolygon* shape = getBlockShape("I");
		shape->setFriction(friction);
		shape->setRestitution(restitution);
		body->addShape(shape, true);
		}
		break;
	case Z:{
		this->setTexture("Block/Z.png");
		PhysicsShapePolygon* shapeL = getBlockShape("Z_L");
		PhysicsShapePolygon* shapeR = getBlockShape("Z_R");
		shapeL->setFriction(friction);
		shapeR->setFriction(friction);
		shapeL->setRestitution(restitution);
		shapeR->setRestitution(restitution);
		body->addShape(shapeL, true);
		body->addShape(shapeR, true);
		}
		break;
	case ZB:{
		this->setTexture("Block/ZB.png");
		PhysicsShapePolygon* shapeL = getBlockShape("ZB_L");
		PhysicsShapePolygon* shapeR = getBlockShape("ZB_R");
		shapeL->setFriction(friction);
		shapeR->setFriction(friction);
		shapeL->setRestitution(restitution);
		shapeR->setRestitution(restitution);
		body->addShape(shapeL, true);
		body->addShape(shapeR, true);
		}
		break;
	case R:{
		this->setTexture("Block/R.png");
		PhysicsShapePolygon* shape = getBlockShape("R");
		shape->setFriction(friction);
		shape->setRestitution(restitution);
		body->addShape(shape, true);
		}
		break;
	}
	//����
	//body->setCategoryBitmask(1);
	//body->setCollisionBitmask(-1);
	body->setContactTestBitmask(-1);
	body->setMass(0);
	body->setGravityEnable(false);
	//����body
	this->setPhysicsBody(body);
	//���������
	this->getTexture()->setAntiAliasTexParameters();

	return true;
}

PhysicsShapePolygon* Block::getBlockShape(const char* name)
{
	//����json�ļ�
	ssize_t len;
	std::string fileName = FileUtils::getInstance()->fullPathForFilename("block_data.json");
	unsigned char* jsonData = FileUtils::getInstance()->getFileData(fileName, "r", &len);
	//����json��������
	Json* json = Json_create((char*)jsonData);
	//���������������ɹ�
	if (json != nullptr)
	{
		//��ȡ"L"��ǩ
		Json* postions = Json_getItem(json, name);
		int posSize = postions->size;
		Vec2 vects[10];
		//����L��ǩ��Ԫ��
		Json* j = postions->child;
		for (int i = 0; i < posSize; i++)
		{
			int x = Json_getInt(j, "x", -1);
			int y = Json_getInt(j, "y", -1);
			vects[i].set(x, y);
			j = j->next;
		}
		Json_dispose(j);
		Json_dispose(postions);
		return PhysicsShapePolygon::create(vects, posSize);
	}
	Json_dispose(json);
	return nullptr;
}
