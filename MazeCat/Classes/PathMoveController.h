#pragma once

#include "cocos2d.h"
#include "AStarPathHelper.h"

enum PathDirection
{
	en_Direction_Up,
	en_Direction_Down,
	en_Direction_Left,
	en_Direction_Right,
	en_Direction_None
};

class MapHelper;
class MoveController;

class MoveControllerListener
{
public:
	// 从起点开始移动
	virtual void onStartMoving() {}
	// 移动中方向改变
	virtual void onChangeDirection(PathDirection dir) {};
	// 到达移动路径的中点
	virtual void onArrivedCenter() {};
	// 到达经过的网格的中点
	virtual void onArrivedTiledCenter() {};
	// 到达移动路径的终点
	virtual void onArrived() = 0;
};

class PathMoveController :public cocos2d::Node, public MoveControllerListener
{
public:
	static PathMoveController* create(cocos2d::Node* object, MapHelper* mapHelper);

	void startMove(cocos2d::Vector<AStarPathUnit*>& pathVec);
	void stopMove();
	void pauseMove();
	void resumeMove();
	void setMoveSpeed(float speed);

	virtual void onArrived();

	void removeListener();

protected:
	PathMoveController();
	virtual ~PathMoveController();

	virtual bool init(cocos2d::Node* object, MapHelper* mapHelper);

	void moveOneUnitOnThePath();

private:
	MapHelper* _mapHelper;
	CC_SYNTHESIZE_READONLY(cocos2d::Node*, _object, Object);
	CC_SYNTHESIZE(MoveControllerListener*, _listener, PathMoveControllerListener);

	MoveController* _moveController;

	CC_SYNTHESIZE(float, _speed, Speed);

	cocos2d::Vector<AStarPathUnit*> _pathVec;
	CC_SYNTHESIZE(int, _curPathUnitIndex, CurPathUnitIndex);

	AStarPathUnit* _lastPathUnit;
	CC_SYNTHESIZE(PathDirection, _presentDirection, PresentDirection);

};