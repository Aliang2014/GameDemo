#include "MoveController.h"
#include "PathMoveController.h"
using namespace cocos2d;

MoveController * MoveController::createWithObject(cocos2d::Node * object)
{
	MoveController* ret = new MoveController();
	if (ret && ret->initWithObject(object))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

MoveController::MoveController()
	: _listener(nullptr)
	, _moveSpeed(100.0f)
	, _speedUnitVectore(Vec2::ZERO)
	, _object(nullptr)
	, _startPos(Point::ZERO)
	, _destPos(Point::ZERO)
	, _totalDistance(0.0f)
	, _isPause(false)
	, _isEnable(false)
{
}

MoveController::~MoveController()
{
}

bool MoveController::initWithObject(cocos2d::Node * object)
{
	_object = object;

	this->scheduleUpdate();

	return true;
}

void MoveController::startMove(const cocos2d::Point & dest)
{
	_destPos = dest;
	updateSpeedUnitVector();

	_isEnable = true;
}

void MoveController::stopMove()
{
	_isEnable = false;
}

void MoveController::pauseMove()
{
	_isPause = true;
}

void MoveController::resumeMove()
{
	_isPause = false;
}

void MoveController::updateSpeedUnitVector()
{
	_startPos = _object->getPosition();
	_speedUnitVectore = ccpNormalize(_destPos - _startPos);
	_totalDistance = ccpDistance(_startPos, _destPos);
}

void MoveController::updateDestPosition(const cocos2d::Point & pos)
{
	if (_destPos.equals(pos))
		return;

	_destPos = pos;
	updateSpeedUnitVector();
}

void MoveController::removeListener()
{
	if (_listener)
		_listener = nullptr;
	else
		log("MoveController can't found listener!");
}

void MoveController::update(float dt)
{
	// 开启使用和不在暂停中时，调用
	if (_isEnable && !_isPause)
	{
		Point curPos = _object->getPosition();
		float disTance = ccpDistance(curPos, _destPos);
        
        // 差一步到达目的地
		if (disTance <= _moveSpeed*dt)
		{
			_object->setPosition(_destPos);
			_isEnable = false;

			if (_listener)
				_listener->onArrived();
				
		}
		else
		{
			if (disTance > _totalDistance)
				updateSpeedUnitVector();
			
			Vec2 delta = _speedUnitVectore*_moveSpeed*dt;
			_object->setPosition(curPos + delta);
		}
	}
}
