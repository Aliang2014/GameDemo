#include "AStarPathHelper.h"
#include "GameLayer.h"
using namespace cocos2d;

/*
** AStarPathUnit
*/

AStarPathUnit * AStarPathUnit::createWithTileCoord(const cocos2d::Point & tileCoord)
{
	AStarPathUnit* ret = new AStarPathUnit();
	if (ret && ret->initWithTileCoord(tileCoord))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

AStarPathUnit::AStarPathUnit()
	: _tileCoord(Point::ZERO)
	, _parent(nullptr)
	, _gScore(0)
	, _hScore(0)
{

}

AStarPathUnit::~AStarPathUnit()
{

}

bool AStarPathUnit::initWithTileCoord(const cocos2d::Point & tileCoord)
{
	_tileCoord = tileCoord;

	return true;
}

int AStarPathUnit::computHScoreFromThisUnitToDestUnit(AStarPathUnit * destUnit) const
{
	// 返回直角坐标系上的距离
	return abs(destUnit->getTileCoord().y - _tileCoord.y) + abs(destUnit->getTileCoord().x - _tileCoord.x);
}

int AStarPathUnit::costMoveToAdjacentUnit() const
{
	// 当前格子到相邻格子的移动代价设置为1
	return 1;
}

int AStarPathUnit::computFScore() const
{
	return _gScore + _hScore;
}

bool AStarPathUnit::isEqual(AStarPathUnit * other) const
{
	return _tileCoord == other->getTileCoord();
}

void AStarPathUnit::printUnitInfo() const
{
	log("Unit Information : ");
	log("tile_coord : (%f, %f)", _tileCoord.x, _tileCoord.y);
	log("G = %d, H = %d, F = %d", _gScore, _hScore, _gScore + _hScore);
}

/*
** AStarPathHelper
*/

AStarPathHelper::AStarPathHelper(const cocos2d::Point & originTile, const cocos2d::Point & destTile, GameLayer * layer)
	: _origin(AStarPathUnit::createWithTileCoord(originTile))
	, _dest(AStarPathUnit::createWithTileCoord(destTile))
	, _layer(layer)
	, _isFound(false)
{
	// 开始寻路
	findPath(_origin, _dest);
}

AStarPathHelper::~AStarPathHelper()
{
	_openVec.clear();
	_closeVec.clear();
	_pathVec.clear();
}

cocos2d::Vector<AStarPathUnit*>& AStarPathHelper::getPathVec()
{
	return _pathVec;
}

void AStarPathHelper::printPathInfo() const
{
	log("from (%f, %f) to (%f, %f)", _origin->getTileCoord().x, _origin->getTileCoord().y, _dest->getTileCoord().x, _dest->getTileCoord().y);
	if (_isFound)
	{
		int step = 0;
		for (auto i = _pathVec.begin(); i != _pathVec.end(); ++i)
		{
			log("step %d : x = %f, y = %f", ++step, (*i)->getTileCoord().x, (*i)->getTileCoord().y);
		}
	}
	else
	{
		log("Can't found available path");
	}
}

void AStarPathHelper::findPath(AStarPathUnit * originUnit, AStarPathUnit * destUnit)
{
	if (originUnit->isEqual(destUnit))
		return;

	insertOpenVecByOrder(originUnit);
	do
	{
		AStarPathUnit* curUnit = _openVec.at(0);
		// 先添加到closeVec，再从openVec中移除
		_closeVec.pushBack(curUnit);
		_openVec.erase(0);

		// 判断是否到达目的地
		if (curUnit->isEqual(destUnit))
		{
			_isFound = true;
			// 将找到的路径添加到pathVec中
			do
			{
				_pathVec.insert(0, curUnit);
				curUnit = curUnit->getParent();
			} while (curUnit->getParent() != nullptr);
			break;
		}

		PointArray* arr = checkFourDirection(curUnit);
		for (int i = 0; i < arr->count(); ++i)
		{
			AStarPathUnit* nextUnit = AStarPathUnit::createWithTileCoord(arr->getControlPointAtIndex(i));
			// 如果nextUnit已经在关闭列表中，跳过
			if (-1 != getIndexInVec(nextUnit, _closeVec))
				continue;

			int index = getIndexInVec(nextUnit, _openVec);
			// index等于-1， 表示nextUnit不在开启列表中
			if (-1 == index)
			{
				nextUnit->setParent(curUnit);
				nextUnit->setGScore(curUnit->getGScore() + curUnit->costMoveToAdjacentUnit());
				nextUnit->setHScore(nextUnit->computHScoreFromThisUnitToDestUnit(destUnit));
				// 按F值的大小顺序添加进开启列表
				insertOpenVecByOrder(nextUnit);
			}
			else
			{
				nextUnit = _openVec.at(index);
				int newGSocre = curUnit->getGScore() + curUnit->costMoveToAdjacentUnit();
				// 如果当前路径得到的G值小于从openVec中得到的该路径G值
				if (newGSocre < nextUnit->getGScore())
				{
					nextUnit->setParent(curUnit);
					nextUnit->setGScore(newGSocre);
					// nextUnit的G值改变后导致F值改变，其在openVec中的顺序需要设置
					nextUnit->retain();
					_openVec.erase(index);
					insertOpenVecByOrder(nextUnit);
					nextUnit->release();
				}
			}
		}
	} while (!_openVec.empty());
}

cocos2d::PointArray * AStarPathHelper::checkFourDirection(AStarPathUnit * unit)
{
	PointArray* arr = PointArray::create(4);
	Point tileCoord = unit->getTileCoord();

	// right
	Point nextPoint = Point(tileCoord.x + 1, tileCoord.y);
	if (_layer->getMapHelper()->isCanCrossAtTileCoord(nextPoint))
		arr->addControlPoint(nextPoint);
	// up
	nextPoint = Point(tileCoord.x, tileCoord.y - 1);
	if (_layer->getMapHelper()->isCanCrossAtTileCoord(nextPoint))
		arr->addControlPoint(nextPoint);
	// left
	nextPoint = Point(tileCoord.x - 1, tileCoord.y);
	if (_layer->getMapHelper()->isCanCrossAtTileCoord(nextPoint))
		arr->addControlPoint(nextPoint);
	// down
	nextPoint = Point(tileCoord.x, tileCoord.y + 1);
	if (_layer->getMapHelper()->isCanCrossAtTileCoord(nextPoint))
		arr->addControlPoint(nextPoint);

	return arr;
}

void AStarPathHelper::insertOpenVecByOrder(AStarPathUnit * unit)
{
	// by order
	int f = unit->computFScore();
	int index = 0;
	for (; index < _openVec.size(); ++index)
	{
		if (f < _openVec.at(index)->computFScore())
			break;
	}
	_openVec.insert(index, unit);
}

int AStarPathHelper::getIndexInVec(AStarPathUnit * unit, const cocos2d::Vector<AStarPathUnit*>& vec)
{
	int index = -1;
	for (int i = 0; i < vec.size(); ++i)
	{
		if (unit->isEqual(vec.at(i)))
		{
			index = i;
			break;
		}
	}
	// 找到返回索引值， 没找到返回-1
	return index;
}
