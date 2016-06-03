#pragma once
#include "cocos2d.h"

class MapHelper :public cocos2d::Node
{
public:
	static MapHelper* createWithName(const char* name);

	// 网格道具判断
	bool isWallAtTileCoord(const cocos2d::Point& tileCoord) const;
	bool isBoneAtTileCoord(const cocos2d::Point& tileCoord) const;
	bool isDogAtTileCoord(const cocos2d::Point& tileCoord) const;
	bool isCanCrossAtTileCoord(const cocos2d::Point& tileCoord) const;
	bool isExitAtTileCoord(const cocos2d::Point& tileCoord) const;
	bool isValidAtTileCoord(const cocos2d::Point& tileCoord) const;

	// 坐标转换
	cocos2d::Point getTileCoordFromPosition(const cocos2d::Point& position) const;
	cocos2d::Point getPositionFromTileCoord(const cocos2d::Point& tileCoord) const;

	// 道具移除
	void removeObjectAtTileCoord(const cocos2d::Point& tileCoord);

private:
	MapHelper();
	virtual ~MapHelper();

private:
	bool initWithName(const char* name);

	bool isPropAtTileCoordForLayer(const char* prop, const cocos2d::Point& tileCoord, cocos2d::TMXLayer* layer) const;

private:
	cocos2d::TMXTiledMap* _map;
	cocos2d::TMXLayer* _bgLayer;
	cocos2d::TMXLayer* _objectLayer;

	CC_SYNTHESIZE_READONLY(cocos2d::Size, _tileSize, TileSize);
	CC_SYNTHESIZE_READONLY(cocos2d::Size, _tileCount, TileCount);
	CC_SYNTHESIZE_READONLY(cocos2d::Size, _mapSize, MapSize);

};