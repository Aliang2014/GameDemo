#pragma once
#include "cocos2d.h"

class MoveControllerListener;
class MoveController :public cocos2d::Node
{
public:
	static MoveController* createWithObject(cocos2d::Node* object);

	void startMove(const cocos2d::Point& dest);
	void stopMove();
	void pauseMove();
	void resumeMove();

	void updateSpeedUnitVector();
	void updateDestPosition(const cocos2d::Point& pos);

	void removeListener();

private:
	MoveController();
	virtual ~MoveController();

private:
	virtual bool initWithObject(cocos2d::Node* object);

	virtual void update(float dt);

private:
	CC_SYNTHESIZE(MoveControllerListener*, _listener, MoveControllerListener);
	CC_SYNTHESIZE(float, _moveSpeed, MoveSpeed);
	cocos2d::Vec2 _speedUnitVectore;

	cocos2d::Node* _object;
	cocos2d::Point _startPos;
	cocos2d::Point _destPos;
	float _totalDistance;

	bool _isPause;
	bool _isEnable;

};