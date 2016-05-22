#include "EnemyBase.h"

void EnemyBase::setStartPoint(float x, float y)
{
	_start_point.x = x;
	_start_point.y = y;
}

cocos2d::Vec2 EnemyBase::getStartPoint() const
{
	return _start_point;
}

EnemyBase::EnemyBase(float hp, float speed, float point)
	: _hp(hp)
	, _speed_y(speed)
	, _points(point)
	, _start_point(cocos2d::Vec2::ZERO)
	, _type(enNone)
	, _out_side(false)
	, _dead(false)
{	

}

EnemyBase::~EnemyBase()
{
}
