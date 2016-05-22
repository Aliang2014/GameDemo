#pragma once
#include "cocos2d.h"

class EnemyBase:public cocos2d::Sprite
{
public:
	enum enEnemyType
	{
		enSmall,
		enMiddle,
		enLarge,
		enBoss,
		enNone
	};

	void setType(enEnemyType type) { _type = type; }

	void setHp(float hp) { _hp = hp; }
	float getHp() const { return _hp; }

	void setSpeedY(float speedY) { _speed_y= speedY; }
	float getSpeedY() const { return _speed_y; }

	void setPoints(float points) { _points = points; }
	float getPoints() const { return _points; }

	void setStartPoint(float x, float y);
	cocos2d::Vec2 getStartPoint() const;

	void setOutSide(bool out) { _out_side = out; }
	bool isOutSide() const { return _out_side; }

	void setDeadState(bool state) { _dead = state; }
	bool isDead() const { return _dead; }

	virtual void enemyHit() = 0;
	virtual void enemyDead() = 0;

protected:
	EnemyBase(float hp, float speed, float point);
	virtual ~EnemyBase();

private:
	enEnemyType _type;
	float _hp;
	float _speed_y;
	float _points;
	cocos2d::Vec2 _start_point;
	bool _out_side;
	bool _dead;
};