#include "Bullet.h"
using namespace cocos2d;

/*
****************************************************
**  BulletNormal
****************************************************
*/
BulletNormal * BulletNormal::create(const Vec2 & hero_point)
{
	BulletNormal* ret = new BulletNormal();
	if (ret && ret->init(hero_point))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void BulletNormal::saveToVector(Vector<BulletBase*>& vec)
{
	vec.pushBack(_base);
}

BulletNormal::BulletNormal()
	: _base(nullptr)
{

}

BulletNormal::~BulletNormal()
{

}

bool BulletNormal::init(const Vec2 & hero_point)
{
	_base = BulletBase::create("bullet1.png", hero_point, Vec2(0, 800.0f), 1.0f);
	addChild(_base);
	_base->setPosition(_base->getStartPoint());

	return true;
}


/*
****************************************************
**  BulletNormal
****************************************************
*/
BulletSuper * BulletSuper::create(const Vec2 & hero_point)
{
	BulletSuper* ret = new BulletSuper();
	if (ret && ret->init(hero_point))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void BulletSuper::saveToVector(Vector<BulletBase*>& vec)
{
	vec.pushBack(_base_left);
	vec.pushBack(_base_right);
}

BulletSuper::BulletSuper()
	: _base_left(nullptr)
	, _base_right(nullptr)
{

}

BulletSuper::~BulletSuper()
{

}

bool BulletSuper::init(const Vec2 & hero_point)
{
	_base_left = BulletBase::create("bullet2.png", hero_point +  Vec2(-25, 0), Vec2(0, 1000.0f), 1.0f);
	addChild(_base_left);
	_base_left->setPosition(_base_left->getStartPoint());

	_base_right = BulletBase::create("bullet2.png", hero_point + Vec2(25, 0), Vec2(0, 1000.0f), 1.0f);
	addChild(_base_right);
	_base_right->setPosition(_base_right->getStartPoint());
	
	return true;
}
