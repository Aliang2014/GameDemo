#include "PathMoveController.h"
#include "MoveController.h"
#include "MapHelper.h"
using namespace cocos2d;

PathMoveController * PathMoveController::create(cocos2d::Node * object, MapHelper* mapHelper)
{
	PathMoveController* ret = new PathMoveController();
	if (ret && ret->init(object, mapHelper))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

PathMoveController::PathMoveController()
	: _object(nullptr)
	, _mapHelper(nullptr)
	, _listener(nullptr)
	, _moveController(nullptr)
	, _speed(100.0f)
	, _curPathUnitIndex(0)
	, _presentDirection(en_Direction_None)
{
}

PathMoveController::~PathMoveController()
{
	_pathVec.clear();
}

bool PathMoveController::init(cocos2d::Node * object, MapHelper* mapHelper)
{
	if (!Node::init())
		return false;

	_object = object;
	_mapHelper = mapHelper;

	_moveController = MoveController::createWithObject(_object);
	_moveController->setMoveControllerListener(this);
	addChild(_moveController);

	return true;
}

void PathMoveController::startMove(cocos2d::Vector<AStarPathUnit*>& pathVec)
{
	_pathVec = pathVec;
	_curPathUnitIndex = 0;
	moveOneUnitOnThePath();

	if (_listener)
		_listener->onStartMoving();
}

void PathMoveController::stopMove()
{
	_moveController->stopMove();
}

void PathMoveController::pauseMove()
{
	_moveController->pauseMove();
}

void PathMoveController::resumeMove()
{
	_moveController->resumeMove();
}

void PathMoveController::setMoveSpeed(float speed)
{
	_speed = speed;
	_moveController->setMoveSpeed(speed);
}

void PathMoveController::onArrived()
{
	++_curPathUnitIndex;

	if (_listener)
		_listener->onArrivedTiledCenter();

	moveOneUnitOnThePath();
}

void PathMoveController::removeListener()
{
	if (_listener)
		_listener = nullptr;
}

void PathMoveController::moveOneUnitOnThePath()
{
	if (_curPathUnitIndex < _pathVec.size())
	{
		AStarPathUnit* curUnit = _pathVec.at(_curPathUnitIndex);
		if (!_lastPathUnit)
			_lastPathUnit = _pathVec.at(0);

		Point destPos = _mapHelper->getPositionFromTileCoord(curUnit->getTileCoord());
		_moveController->startMove(destPos);

		// 判断移动中有无方向的改变
		bool changeDirection = false;
		if (_listener)
		{
			if (destPos.x > _object->getPosition().x && _presentDirection != en_Direction_Right)
			{
				_presentDirection = en_Direction_Right;
				changeDirection = true;
			}
			else if (destPos.x < _object->getPosition().x && _presentDirection != en_Direction_Left)
			{
				_presentDirection = en_Direction_Left;
				changeDirection = true;
			}
			else if (destPos.y > _object->getPosition().y && _presentDirection != en_Direction_Up)
			{
				_presentDirection = en_Direction_Up;
				changeDirection = true;
			}
			else if (destPos.y < _object->getPosition().y && _presentDirection != en_Direction_Down)
			{
				_presentDirection = en_Direction_Down;
				changeDirection = true;
			}
			if (_listener && changeDirection)
				_listener->onChangeDirection(_presentDirection);

			_lastPathUnit = curUnit;
		}
	}
	// 到达整个路径的终点
	else
	{
		if (_listener)
			_listener->onArrived();
	}
}
