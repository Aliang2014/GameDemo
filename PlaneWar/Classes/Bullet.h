#pragma once
#include "BulletBase.h"

/*
****************************************************
**  BulletNormal
****************************************************
*/

class BulletNormal :public cocos2d::Node
{
public:
	static BulletNormal* create(const cocos2d::Vec2& hero_point);

	void saveToVector(cocos2d::Vector<BulletBase*>& vec);

private:
	BulletNormal();
	~BulletNormal();

	bool init(const cocos2d::Vec2& hero_point);

private:
	BulletBase* _base;
};


/*
****************************************************
**  BulletSuper
****************************************************
*/

class BulletSuper :public cocos2d::Node
{
public:
	static BulletSuper* create(const cocos2d::Vec2& hero_point);

	void saveToVector(cocos2d::Vector<BulletBase*>& vec);

private:
	BulletSuper();
	~BulletSuper();

	bool init(const cocos2d::Vec2& hero_point);

private:
	BulletBase* _base_left;
	BulletBase* _base_right;
};