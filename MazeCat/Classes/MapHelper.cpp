#include "MapHelper.h"
using namespace cocos2d;

MapHelper * MapHelper::createWithName(const char * name)
{
	MapHelper* ret = new MapHelper();
	if (ret && ret->initWithName(name))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

MapHelper::MapHelper()
	: _map(nullptr)
    , _bgLayer(nullptr)
	, _objectLayer(nullptr)
	, _tileSize(Size::ZERO)
	, _tileCount(Size::ZERO)
	, _mapSize(Size::ZERO)
{
}

MapHelper::~MapHelper()
{

}

bool MapHelper::initWithName(const char* name)
{
	bool bRet = false;
	do
	{
		_map = TMXTiledMap::create(name);
		_bgLayer = _map->getLayer("Background");
		_objectLayer = _map->getLayer("Objects");
		addChild(_map);

		_tileSize = _map->getTileSize();
		_tileCount = _map->getMapSize();
		_mapSize = Size(_tileSize.width * _tileCount.width, _tileSize.height * _tileCount.height);

		bRet = true;
	} while (0);
	return bRet;
}

bool MapHelper::isWallAtTileCoord(const cocos2d::Point & tileCoord) const
{
	return isPropAtTileCoordForLayer("Wall", tileCoord, _bgLayer);
}

bool MapHelper::isBoneAtTileCoord(const cocos2d::Point & tileCoord) const
{
	return isPropAtTileCoordForLayer("Bone", tileCoord, _objectLayer);
}

bool MapHelper::isDogAtTileCoord(const cocos2d::Point & tileCoord) const
{
	return isPropAtTileCoordForLayer("Dog", tileCoord, _objectLayer);
}

bool MapHelper::isCanCrossAtTileCoord(const cocos2d::Point & tileCoord) const
{
	return isValidAtTileCoord(tileCoord) && !isWallAtTileCoord(tileCoord);
}

bool MapHelper::isExitAtTileCoord(const cocos2d::Point & tileCoord) const
{
	return isPropAtTileCoordForLayer("Exit", tileCoord, _objectLayer);
}

bool MapHelper::isValidAtTileCoord(const cocos2d::Point & tileCoord) const
{
	// 判断是否在map地图范围内
	if (tileCoord.x < 0 ||
		tileCoord.y < 0 ||
		tileCoord.x >= _tileCount.width ||
		tileCoord.y >= _tileCount.height)
		return false;
	return true;
}

cocos2d::Point MapHelper::getTileCoordFromPosition(const cocos2d::Point & position) const
{
	int x = position.x / _tileSize.width;
	int y = (_mapSize.height - position.y) / _tileSize.height;
	return Point(x, y);
}

cocos2d::Point MapHelper::getPositionFromTileCoord(const cocos2d::Point & tileCoord) const
{
	// 返回格子正中间的坐标
	int x = tileCoord.x * _tileSize.width + _tileSize.width / 2;
	int y = _mapSize.height - (tileCoord.y + 1)*_tileSize.height + _tileSize.height / 2;
	return Point(x, y);
}

void MapHelper::removeObjectAtTileCoord(const Point & tileCoord)
{
	_objectLayer->removeTileAt(tileCoord);
}

bool MapHelper::isPropAtTileCoordForLayer(const char * prop, const cocos2d::Point & tileCoord, cocos2d::TMXLayer * layer) const
{
	if (!isValidAtTileCoord(tileCoord))
		return false;

	int gid = layer->getTileGIDAt(tileCoord);
	Value properties = _map->getPropertiesForGID(gid);
	if (properties.isNull())
		return false;
	return properties.asValueMap().find(prop) != properties.asValueMap().end();
}
