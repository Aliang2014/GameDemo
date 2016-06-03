#pragma once
#include "cocos2d.h"

class GameLayer;
class AStarPathUnit :public cocos2d::Ref
{
public:
	static AStarPathUnit* createWithTileCoord(const cocos2d::Point& tileCoord);

	int computHScoreFromThisUnitToDestUnit(AStarPathUnit* destUnit) const;
	int costMoveToAdjacentUnit() const;
	int computFScore() const;

	bool isEqual(AStarPathUnit* other) const;

	void printUnitInfo() const;

private:
	AStarPathUnit();
	virtual ~AStarPathUnit();

private:
	bool initWithTileCoord(const cocos2d::Point& tileCoord);

private:
	CC_SYNTHESIZE_READONLY(cocos2d::Point, _tileCoord, TileCoord);
	CC_SYNTHESIZE(AStarPathUnit*, _parent, Parent);
	CC_SYNTHESIZE(int, _gScore, GScore);
	CC_SYNTHESIZE(int, _hScore, HScore);

};


class AStarPathHelper
{
public:
	AStarPathHelper(const cocos2d::Point& originTile, const cocos2d::Point& destTile, GameLayer* layer);
	~AStarPathHelper();

	cocos2d::Vector<AStarPathUnit*>& getPathVec();

	void printPathInfo() const;

private:
	void findPath(AStarPathUnit* originUnit, AStarPathUnit* destUnit);

	cocos2d::PointArray* checkFourDirection(AStarPathUnit* unit);

	void insertOpenVecByOrder(AStarPathUnit* unit);

	int getIndexInVec(AStarPathUnit* unit, const cocos2d::Vector<AStarPathUnit*>& vec);

private:
	AStarPathUnit* _origin;
	AStarPathUnit* _dest;
	CC_SYNTHESIZE_READONLY(bool, _isFound, FoundPath);
	cocos2d::Vector<AStarPathUnit*> _pathVec;
	cocos2d::Vector<AStarPathUnit*> _openVec;
	cocos2d::Vector<AStarPathUnit*> _closeVec;
	CC_SYNTHESIZE_READONLY(GameLayer*, _layer, Layer);

};