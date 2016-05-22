#include "BulletBase.h"
using namespace cocos2d;

BulletBase * BulletBase::create(const char * spriteFramePath, const cocos2d::Vec2 & start_point, const cocos2d::Vec2 & speed, float attack)
{
	BulletBase* ret = new BulletBase();
	if (ret && ret->init(spriteFramePath, start_point, speed, attack))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool BulletBase::isOutSide() const
{
	Size bullet_size = getContentSize();
	Size screen_size = Director::getInstance()->getVisibleSize();
	Rect acitvity_scope(-bullet_size.width / 2, -bullet_size.height / 2, screen_size.width + bullet_size.width, screen_size.height + bullet_size.height);
	if (acitvity_scope.containsPoint(getPosition()))
	{
		return false;
	}
	return true;
}

BulletBase::BulletBase()
{
}


BulletBase::~BulletBase()
{
}

bool BulletBase::init(const char * spriteFramePath, const cocos2d::Vec2 & start_point, const cocos2d::Vec2 & speed, float attack)
{
	if (!initWithSpriteFrameName(spriteFramePath))
		return false;

	_start_point = start_point;
	_speed = speed;
	_attack = attack;
	_out_side = false;

	scheduleUpdate();

	return true;
}

void BulletBase::update(float dt)
{
	if(isOutSide())
	{
		_out_side = true;
		removeFromParent();
	}
	else
	{
		this->setPosition(this->getPosition() + _speed * dt);
	}
}
